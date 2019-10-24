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

#pragma once

#define _GNU_SOURCE

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include <nica/macros.h>

/**
 * Safely represent and store a buffer as a string
 */
typedef struct nc_string_t {
        char *str; /**<Buffer holding a NUL-terminated string */
        int len;   /**<Current length of the string */
} nc_string;

/**
 * Construct a new string using printf style syntax
 *
 * @param format Printf-style format string
 * @param ... Variable arguments
 *
 * @return a new nc_string if allocation succeeded, otherwise NULL
 */
_nica_public_ nc_string *nc_string_dup_printf(const char *format, ...)
    __attribute__((format(printf, 1, 2)));


_nica_public_ nc_string *nc_string_append_printf(nc_string *st, const char *ptn, ...)
	__attribute__((format(printf, 2, 3)));

/**
 * Duplicate a string into a new NUL-terminated nc_string
 *
 * @param str Pointer to a string
 * @return a new nc_string if allocation succeeded, otherwise NULL
 */
_nica_public_ nc_string *nc_string_dup(const char *str);

/**
 * Free an existing string and its contents
 *
 * @param str Pointer to an existing string
 */
static inline void nc_string_free(nc_string *str)
{
        if (!str) {
                return;
        }
        if (str->str) {
                free(str->str);
        }
        free(str);
}

/**
 * Append the contents of 'append' into the given nc_string
 *
 * @param str Pointer to a valid nc_string
 * @param append Text to append into the nc_string
 *
 * @return a boolean value indicating success
 */
_nica_public_ bool nc_string_cat(nc_string *str, const char *append);

_nica_public_ bool nc_string_prepend(nc_string *s, const char *prepend);


/**
 * Determine if string A is equal to string B
 *
 * @note This function will not check beyond the length of string A
 *
 * @param a string to check
 * @param b string to check against
 * @return a boolean value, true if the strings match, otherwise false
 */
static inline bool nc_string_equal(nc_string *a, nc_string *b)
{
        if (!a || !b) {
                return false;
        }
        if (!a->str || !b->str) {
                return false;
        }
        return strncmp(a->str, b->str, (size_t)a->len) == 0;
}

/**
 * Determine if string A is equal to string B
 *
 * @note This function will not check beyond the length of string A
 *
 * @param a string to check
 * @param b const char* string to check against
 * @return a boolean value, true if the strings match, otherwise false
 */
static inline bool nc_string_const_equal(nc_string *a, const char *b)
{
        if (!a || !b) {
                return false;
        }
        if (!a->str) {
                return false;
        }
        return strncmp(a->str, b, (size_t)a->len) == 0;
}

/**
 * Convenience wrapper to obtain string length
 *
 * @note This will assert the string is not NULL, ensuring a termination.
 *
 * @return length of the given string
 */
static inline int ncstrlen(nc_string *str)
{
        assert(str != NULL);
        return str->len;
}

/**
 * Determine if a string has the given suffix
 *
 * @param str Valid nc_string
 * @param suffix A suffix to check
 * @param len Length of the suffix
 *
 * @return True if @str has the given suffix
 */
static inline bool nc_string_has_suffix_const(nc_string *str, char *suffix, ssize_t len)
{
        if (!str || !suffix) {
                return false;
        }
        if (len > str->len || len <= 0) {
                return false;
        }
        return (strncmp(str->str + (str->len - (len)), suffix, (size_t)len) == 0);
}

/**
 * Determine if a string has the given prefix
 *
 * @param str Valid nc_string
 * @param prefix A prefix to check
 * @param len Length of the prefix
 *
 * @return True if @str has the given prefix
 */
static inline bool nc_string_has_prefix_const(nc_string *str, char *prefix, ssize_t len)
{
        if (!str || !prefix) {
                return false;
        }
        if (len > str->len || len <= 0) {
                return false;
        }
        return (strncmp(str->str, prefix, (size_t)len) == 0);
}

/**
 * Perform inplace modification of a strip to return
 * a stripped pointer, that is, without start and end whitespace.
 *
 * This does not modify the length of the string.
 */
static inline char *nc_string_strip(nc_string *str)
{
        int i;

        if (!str || !str->str) {
                return NULL;
        }
        for (i = 0; i < str->len; i++) {
                if (str->str[i] != ' ') {
                        break;
                }
        }
        for (int j = str->len - 1; j > i; j--) {
                if (str->str[j] != ' ') {
                        str->str[j + 1] = '\0';
                        break;
                }
        }

        return str->str + i;
}
/**
 * To be used only with compile time constants.
 */
#define nc_string_has_suffix(a, suff) nc_string_has_suffix_const(a, suff, (sizeof(suff) - 1))
#define nc_string_has_prefix(a, pref) nc_string_has_prefix_const(a, pref, (sizeof(pref) - 1))

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
