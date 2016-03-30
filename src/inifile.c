/*
 * This file is part of libnica.
 *
 * Copyright (C) 2016 Intel Corporation
 *
 * libnica is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>

#include "inifile.h"
#include "hashmap.h"

/**
 * Strip leading whitespace from string (left)
 */
static char *lstrip(char *str, size_t len, size_t *out_len)
{
        size_t skip_len = 0;

        for (size_t i = 0; i < len; i++) {
                if (str[i] == ' ' || str[i] == '\t') {
                        ++skip_len;
                        continue;
                }
                break;
        }
        *out_len = len-skip_len;
        if (skip_len > 0) {
                char *c = strdup(str+skip_len);
                free(str);
                return c;
        }
        return str+skip_len;
}

/**
 * Strip trailing whitespace from string (right)
 */
static char *rstrip(char *str, size_t len, size_t *out_len)
{
        size_t skip_len = 0;

        for (int i = len; i > 0; i--) {
                if (str[i] == ' ' || str[i] == '\t') {
                        ++skip_len;
                        continue;
                } else if (str[i] == '\0') {
                        continue;
                }
                break;
        }
        *out_len = len-skip_len;
        if (len == skip_len) {
                return str;
        }
        str[(len-skip_len)] = '\0';
        return str;
}


static char *string_chew_terminated(char *inp)
{
        int skip_offset = 0;
        int len = 0;
        int end_len = 0;
        bool count = true;

        if (!inp) {
                return NULL;
        }

        for (char *c = inp; *c; c++) {
                if (count && (*c == ' ' || *c == '\t')) {
                        ++skip_offset;
                        continue;
                } else {
                        count = false;
                }
                ++len;
        }
        for (int i = len; i>skip_offset; i--) {
                if (inp[i] == ' ' || inp[i] == '\t') {
                        ++end_len;
                        continue;
                } else if (inp[i] == '\0') {
                        continue;
                }
                break;
        }

        if (end_len > 0) {
                inp[(len-end_len)] = '\0';
        }
        char *c = strdup(inp+skip_offset);
        free(inp);
        return c;
}

/**
 * Munch both ends of the string
 */
static char *string_strip(char *str, size_t len, size_t *out_len)
{
        size_t mylen = len;

        char *c = lstrip(str, len, &mylen);
        c = rstrip(c, mylen, &mylen);
        if (out_len) {
                *out_len = mylen;
        }
        return c;
}

NcHashmap *nc_ini_file_parse(const char *path)
{
        autofree(FILE) *file = NULL;
        char *buf = NULL;
        ssize_t r = 0;
        size_t sn = 0;
        int line_count = 1;
        char *current_section = NULL;
        NcHashmap *root_map = NULL;
        NcHashmap *section_map = NULL;
        bool failed = false;

        file = fopen(path, "r");
        if (!file) {
                return NULL;
        }

        root_map = nc_hashmap_new_full(nc_string_hash, nc_string_compare, free, (nc_hash_free_func)nc_hashmap_free);

        /* This is all TODO stuff - not yet fully implemented */
        while ((r = getline(&buf, &sn, file)) != -1) {
                char *ch = NULL;
                size_t str_len = r;

                /* Fix newline */
                if (buf[r-1] == '\n') {
                        buf[r-1] = '\0';
                        --r;
                }
                str_len = r;

                buf = string_strip(buf, r, &str_len);
                /* Empty lines are fine */
                if (streq(buf, "")) {
                        goto next;
                }

                if (buf[0] == '[') {
                        /* Validate section start */
                        if (buf[str_len-1] != ']') {
                                /* Throw error */
                                fprintf(stderr, "[inifile] Expected closing ']' on line %d\n", line_count);
                                goto fail;
                        }
                        /* Grab the section name, and "close" last section */
                        buf[str_len-1] = '\0';
                        if (current_section) {
                                free(current_section);
                        }
                        current_section = strdup(buf+1);
                        section_map = nc_hashmap_get(root_map, current_section);
                        if (!section_map) {
                                /* Create a new section dynamically */
                                section_map = nc_hashmap_new_full(nc_string_hash, nc_string_compare, free, free);
                                nc_hashmap_put(root_map, strdup(current_section), section_map);
                        }
                        goto next;
                } else if (buf[0] == '#' || buf[0] == ';') {
                        /* Skip comment */
                        goto next;
                }

                /* Look for key = value */
                ch = memchr(buf, '=', sn);
                if (!ch) {
                        /* Throw error? */
                        fprintf(stderr, "[inifile] Expected key=value notation on line %d\n", line_count);
                        goto fail;
                }

                /* Can't have sectionless k->v */
                if (!current_section) {
                        fprintf(stderr, "[inifile] Encountered key=value mapping without valid section on line %d\n", line_count);
                        goto fail;
                }

                int offset = ch-buf;

                char *value = strdup((buf+offset)+1);
                buf[offset] = '\0';
                char *key = strdup(buf);
                key = string_chew_terminated(key);
                value = string_chew_terminated(value);

                /* Ensure a section mapping exists */
                section_map = nc_hashmap_get(root_map, current_section);
                if (!section_map) {
                        fprintf(stderr, "Fatal! No section map for named section: %s\n", current_section);
                        abort();
                }

                /* Insert these guys into the map */
                if (!nc_hashmap_put(section_map, key, value)) {
                        fprintf(stderr, "Fatal! Out of memory\n");
                        abort();
                }
next:
                if (buf) {
                        free(buf);
                        buf = NULL;
                }
                ++line_count;
                sn = 0;
                continue;
fail:
                failed = true;
                break;
        }

        if (buf) {
                free(buf);
                buf = NULL;
        }

        if (current_section) {
                free(current_section);
                current_section = NULL;
        }

        if (failed) {
                if (root_map) {
                        nc_hashmap_free(root_map);
                }
                return NULL;
        }
        return root_map;
}

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */
