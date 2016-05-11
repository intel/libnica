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

#pragma once

#define _GNU_SOURCE
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <nica/macros.h>

#define _cleanup_list_ __attribute__((cleanup(nc_list_free)))
#define _cleanup_list_all_ __attribute__((cleanup(nc_list_free_all)))

/**
 * A singly-linked list
 */
typedef struct NcList {
        void *data;          /**<Data for this item in the list */
        struct NcList *next; /**<Next item in the list */
        struct NcList *tail; /**<Tail of the list */
        uint64_t size;       /**<Size of list */
} NcList;

#define NC_LIST_FOREACH(list, elem) for ((elem) = (list); (elem) != NULL; (elem) = (elem)->next)

/**
 * Append data to an existing list, or create a new list if NULL
 *
 * @note May return false if memory allocation errors exist
 * @param list Pointer to NcList pointer
 * @param data Data pointer to store in the list
 * @return a boolean value, indicating success of the operation
 */
_nica_public_ bool nc_list_append(NcList **list, void *data);

/*
 * Prepend data to an existing list, or create a new list if NULL
 * Much faster than append
 *
 * @note May return false if memory allocation errors exist
 * @param list Pointer to NcList pointer
 * @param data Data pointer to store in the list
 * @return a boolean value, indicating success of the operation
 */
_nica_public_ bool nc_list_prepend(NcList **list, void *data);

/**
 * Remove the given element from the list
 *
 * @param list Pointer to a NcList pointer
 * @param data Remove item with the matching data pointer
 * @param do_free Whether to free the item
 * @return a boolean value, indicating success of the operation
 */
_nica_public_ bool nc_list_remove(NcList **list, void *data, bool do_free);

/**
 * Free all items in the list
 */
static inline void nc_list_free(void *p)
{
        NcList **listp = (NcList **)p;
        NcList *list = *(NcList **)p;
        if (!list) {
                return;
        }
        NcList *elem, *node = NULL;
        elem = list;
        while (elem != NULL) {
                node = elem->next;
                free(elem);
                elem = node;
        }
        *listp = NULL;
}

/**
 * Free all items in the list and their items
 */
static inline void nc_list_free_all(void *p)
{
        NcList **listp = (NcList **)p;
        NcList *list = *(NcList **)p;
        if (!list) {
                return;
        }
        NcList *elem, *node = NULL;
        elem = list;
        while (elem != NULL) {
                free(elem->data);
                node = elem->next;
                free(elem);
                elem = node;
        }
        *listp = NULL;
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
