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

#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "util.h"

NcArray *nc_array_new(void)
{
        NcArray *ret = NULL;
        /* If this fails, we simply return NULL and allow the user
         * to deal with the error */
        ret = calloc(1, sizeof(NcArray));
        return ret;
}

bool nc_array_add(NcArray *array, void *data)
{
        uint16_t new_len;
        size_t curr, new_size;

        if (!array || !data) {
                return false;
        }
        if (!array->data) {
                array->data = calloc(1, sizeof(void *));
                if (!array->data) {
                        return false;
                }
        }

        new_len = (uint16_t)(array->len + 1);
        if (!new_len) {
                return false;
        }
        curr = (size_t)(array->len * sizeof(void *));
        new_size = curr + sizeof(void *);
        if (new_len >= array->len) {
                /* Resize the array to hold one more pointer */
                array->data = greedy_realloc((void **)&array->data, &curr, new_size);
                if (!array->data) {
                        return false;
                }
        }
        /* Store the pointer at the end of the array */
        array->len = new_len;
        array->data[array->len - 1] = data;
        return true;
}

void *nc_array_get(NcArray *array, uint16_t index)
{
        if (!array) {
                return NULL;
        }
        if (index >= array->len) {
                return NULL;
        }
        return array->data[index];
}

void nc_array_free(NcArray **array, array_free_func free_method)
{
        int i;
        if (!array || !*array) {
                return;
        }

        if (free_method) {
                /* Call the free_method on all members */
                for (i = 0; i < (*array)->len; i++) {
                        free_method((*array)->data[i]);
                }
        }
        /* Ensure this array is indeed gone. */
        free((*array)->data);
        free(*array);
        *array = NULL;
}

void nc_array_qsort(NcArray *self, __compar_fn_t func)
{
        qsort(self->data, self->len, sizeof(void *), func);
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
