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

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

bool nc_list_append(NcList **list, void *data)
{
        NcList *head = *list;

        NcList *parent = NULL;
        NcList *next = NULL;

        if (!head) {
                /* New list generation */
                head = calloc(1, sizeof(NcList));
                if (!head) {
                        return false;
                }
                head->size = 1;
                head->tail = head;
                head->next = NULL;
                head->data = data;
                *list = head;
        } else {
                /* New tail generation */
                next = calloc(1, sizeof(NcList));
                if (!next) {
                        return false;
                }
                head->size += 1;
                parent = head->tail;
                parent->next = next;
                head->tail = next;
                next->next = NULL;
                next->data = data;
        }
        return true;
}

bool nc_list_prepend(NcList **list, void *data)
{
        NcList *head = *list;
        NcList *prev = NULL;

        if (!head) {
                /* New list generation */
                head = calloc(1, sizeof(NcList));
                if (!head) {
                        return false;
                }
                head->size = 1;
                head->tail = head;
                head->next = NULL;
                head->data = data;
                *list = head;
        } else {
                /* New head generation */
                prev = calloc(1, sizeof(NcList));
                if (!prev) {
                        return false;
                }
                prev->size = head->size + 1;
                head->size = 0;
                prev->next = head;
                prev->tail = head->tail;
                head->tail = NULL;
                prev->data = data;
                *list = prev;
        }

        return true;
}

bool nc_list_remove(NcList **list, void *data, bool do_free)
{
        NcList *head = *list;
        NcList *current = head;
        NcList *prev = head;

        /* Determine the node inside the list */
        while ((current != NULL) && (current->data != data)) {
                prev = current;
                current = current->next;
        }
        /* Not found */
        if (!current) {
                return false;
        }

        /* Data on the head (head removal) */
        if (current == head) {
                if (head->next) {
                        head->next->size = head->size - 1;
                        head->next->tail = head->tail;
                }
                head = head->next;
        } else {
                /* Middle or end */
                prev->next = current->next;
                head->size--;
        }

        /* Update tail pointer */
        if (head && head->tail == current) {
                head->tail = prev;
        }

        /* Should free? */
        if (do_free) {
                free(current->data);
        }
        free(current);

        *list = head;
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
