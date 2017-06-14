/*
 * This file is part of libnica.
 *
 * Copyright © 2016-2017 Intel Corporation
 *
 * libnica is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */

#define _GNU_SOURCE

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nc-string.h"

nc_string *nc_string_dup(const char *str)
{
        if (!str) {
                return NULL;
        }
        struct nc_string_t *st = calloc(1, sizeof(struct nc_string_t));
        if (!st) {
                return NULL;
        }
        st->len = asprintf(&st->str, "%s", str);
        if (st->len < 0 || !st->str) {
                free(st);
                return NULL;
        }
        return st;
}

nc_string *nc_string_dup_printf(const char *ptn, ...)
{
        if (!ptn) {
                return NULL;
        }

        struct nc_string_t *st = calloc(1, sizeof(struct nc_string_t));
        if (!st) {
                return NULL;
        }

        va_list va;
        va_start(va, ptn);

        st->len = vasprintf(&st->str, ptn, va);
        if (st->len < 0 || !st->str) {
                free(st);
                st = NULL;
                goto end;
        }
end:
        va_end(va);

        return st;
}

nc_string *nc_string_append_printf(nc_string *st, const char *ptn, ...)
{
        char *newstr;
        char *newstr2;
        int ret;

        if (!ptn) {
                return NULL;
        }

        va_list va;
        va_start(va, ptn);

        ret = vasprintf(&newstr, ptn, va);
        if (ret <= 0) {
                newstr = strdup("");
        }

        st->len = asprintf(&newstr2, "%s%s", st->str, newstr);
        free(st->str);
        st->str = newstr2;
        free(newstr);
        va_end(va);

        return st;
}


bool nc_string_cat(nc_string *s, const char *append)
{
        char *p = NULL;
        int len = 0;

        if (!s || !append) {
                return false;
        }
        if (!s->str) {
                return false;
        }
        len = asprintf(&p, "%s%s", s->str, append);
        if (!p || len < s->len) {
                return false;
        }
        free(s->str);
        s->str = p;
        s->len = len;
        return true;
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
