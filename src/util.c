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

#include <assert.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

#include "util.h"

/**
 * Taken from swupd
 */
void nc_dump_file_descriptor_leaks(void)
{
        DIR *dir = NULL;
        struct dirent *entry = NULL;

        dir = opendir("/proc/self/fd");
        if (!dir) {
                return;
        }

        while ((entry = readdir(dir)) != NULL) {
                autofree(char) *filename = NULL;
                char buffer[PATH_MAX + 1] = { 0 };
                size_t size;

                if (streq(entry->d_name, ".")) {
                        continue;
                }
                if (streq(entry->d_name, "..")) {
                        continue;
                }
                /* skip stdin/out/err */
                if (streq(entry->d_name, "0")) {
                        continue;
                }
                if (streq(entry->d_name, "1")) {
                        continue;
                }
                if (streq(entry->d_name, "2")) {
                        continue;
                }

                /* we hold an fd open, the one from opendir above */
                sprintf(buffer, "%i", dirfd(dir));
                if (streq(entry->d_name, buffer)) {
                        continue;
                }

                if (asprintf(&filename, "/proc/self/fd/%s", entry->d_name) <=
                    0) {
                        abort();
                }

                memset(&buffer, 0, sizeof(buffer));
                size = readlink(filename, buffer, PATH_MAX);
                if (size) {
                        fprintf(stderr,
                                "Possible filedescriptor leak : %s (%s)\n",
                                entry->d_name,
                                buffer);
                }
        }

        closedir(dir);
}

/**
 * Ported from Buxton, in turn inspired by systemd
 */
void *greedy_realloc(void **p, size_t *allocated, size_t need)
{
        size_t a;
        void *q;

        assert(p);
        assert(allocated);

        if (*allocated >= need) {
                return *p;
        }

        a = MAX(64u, need * 2);
        q = realloc(*p, a);
        if (!q) {
                return NULL;
        }

        *p = q;
        *allocated = a;
        return q;
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
