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
 * Hold each section in the map
 */
typedef struct NcIniFileSection {
        const char *name;
        NcHashmap *values;
} NcIniFileSection;

/**
 * Our opaque NcIniFile
 */
struct NcIniFile {
        NcHashmap *sections;
};

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

/**
 * Munch both ends of the string
 */
static char *string_strip(char *str, size_t len, size_t *out_len)
{
        size_t mylen = len;

        char *c = lstrip(str, len, &mylen);
        c = rstrip(c, mylen, &mylen);
        *out_len = mylen;
        return c;
}

NcIniFile *nc_ini_file_parse(const char *path)
{
        NcIniFile *ret = NULL;
        autofree(FILE) *file = NULL;
        char *buf = NULL;
        size_t r = -1;
        size_t sn = -1;
        int line_count = 1;

        file = fopen(path, "r");
        if (!file) {
                return NULL;
        }

        /* This is all TODO stuff - not yet fully implemented */
        while ((r = getline(&buf, &sn, file)) > 0) {
                char *ch = NULL;
                size_t str_len;

                buf = string_strip(buf, sn, &str_len);

                /* Empty lines are fine */
                if (streq(buf, "")) {
                        goto next;
                }

                if (buf[0] == '[') {
                        /* Validate section start */
                        if (buf[str_len] != ']') {
                                /* Throw error */
                                fprintf(stderr, "[inifile] Expected closing ']' on line %d\n", line_count);
                        }
                        /* Grab the section name, and "close" last section */
                        goto next;
                } else if (buf[0] == '#' || buf[0] == ';') {
                        /* Skip comment */;
                        goto next;
                }

                /* Look for key = value */
                ch = memchr(buf, '=', sn);
                if (!ch) {
                        /* Throw error? */
                        fprintf(stderr, "[inifile] Expected key=value notation on line %d\n", line_count);
                        continue;
                }
next:
                free(buf);
                buf = NULL;
                ++line_count;
        }
        if (buf) {
                free(buf);
                buf = NULL;
        }

        /* Return the object */
        ret = calloc(1, sizeof(struct NcIniFile));
        if (!ret) {
                return NULL;
        }

        return ret;
}

void nc_ini_file_free(NcIniFile *file)
{
        if (!file) {
                return;
        }
        free(file);
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
