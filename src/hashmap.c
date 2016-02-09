/*
 * This file is part of libthingamabob.
 *
 * Copyright (C) 2016 Intel Corporation
 *
 * libthingamabob is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "hashmap.h"

#define INITIAL_SIZE 61

/* When we're "full", 70% */
#define FULL_FACTOR 0.7

/* Multiple to increase bucket size by */
#define INCREASE_FACTOR 4

/**
 * An bucket/chain within the hashmap
 */
typedef struct TbHashmapEntry {
        void *hash;                             /**<The key for this item */
        void *value;                            /**<Value for this item */
        struct TbHashmapEntry *next;         /**<Next item in the chain */
        bool occ;                               /**<Whether this bucket is occupied */
} TbHashmapEntry;

/**
 * A TbHashmap
 */
struct TbHashmap {
        int size;                       /**<Current size of the hashmap */
        int next_size;                  /**<Next size at which we need to resize */
        int n_buckets;                  /**<Current number of buckets */
        TbHashmapEntry *buckets;     /**<Stores our bucket chains */

        tb_hash_create_func hash;         /**<Hash generation function */
        tb_hash_compare_func compare;     /**<Key comparison function */
        tb_hash_free_func key_free;        /**<Cleanup function for keys */
        tb_hash_free_func value_free;      /**<Cleanup function for values */
};

/**
 * Iteration object
 */
typedef struct _TbHashmapIter {
        int bucket;              /**<Current bucket position */
        TbHashmap *map;        /**<Associated TbHashmap */
        void *item;              /**<Current item in this iteration */
} _TbHashmapIter;

static void tb_hashmap_update_next_size(TbHashmap *self);
static bool tb_hashmap_resize(TbHashmap *self);

static inline bool tb_hashmap_maybe_resize(TbHashmap *self)
{
        if (!self) {
                return false;
        }
        if (self->size >= self->next_size) {
                return true;
        }
        return false;
}

static TbHashmap *tb_hashmap_new_internal(tb_hash_create_func create , tb_hash_compare_func compare, tb_hash_free_func key_free, tb_hash_free_func value_free)
{
        TbHashmap *map = NULL;
        TbHashmapEntry *buckets = NULL;

        map = calloc(1, sizeof(TbHashmap));
        if (!map) {
                return NULL;
        }

        buckets = calloc(INITIAL_SIZE, sizeof(TbHashmapEntry));
        if (!buckets ) {
                free(map);
                return NULL;
        }
        map->buckets = buckets;
        map->n_buckets = INITIAL_SIZE;
        map->hash = create ? create : simple_hash;
        map->compare = compare ? compare : simple_compare;
        map->key_free = key_free;
        map->value_free = value_free;
        map->size = 0;

        tb_hashmap_update_next_size(map);

        return map;
}

TbHashmap *tb_hashmap_new(tb_hash_create_func create, tb_hash_compare_func compare)
{
        return tb_hashmap_new_internal(create, compare, NULL, NULL);
}

TbHashmap *tb_hashmap_new_full(tb_hash_create_func create , tb_hash_compare_func compare, tb_hash_free_func key_free, tb_hash_free_func value_free)
{
        return tb_hashmap_new_internal(create, compare, key_free, value_free);
}

static inline unsigned tb_hashmap_get_hash(TbHashmap *self, const void *key)
{
        unsigned hash = self->hash(key);
        return hash;
}

static bool tb_hashmap_insert_bucket(TbHashmap *self, TbHashmapEntry *buckets, int n_buckets, unsigned hash, const void *key, void *value)
{
        if (!self || !buckets) {
                return false;
        }

        TbHashmapEntry *row = &(buckets[hash % n_buckets]);
        TbHashmapEntry *head = NULL;
        TbHashmapEntry *parent = head = row;
        bool can_replace = false;
        TbHashmapEntry *tomb = NULL;
        int ret = 1;

        while (row) {
                if (!row->occ) {
                        tomb = row;
                }
                parent = row;
                if (row->occ && row->hash == key) {
                        if (self->compare(row->hash, key)) {
                                can_replace = true;
                                break;
                        }
                }
                row = row->next;
        }

        if (can_replace) {
                /* Replace existing allocations. */
                if (self->value_free) {
                        self->value_free(row->value);
                }
                if (self->key_free) {
                        self->key_free(row->hash);
                }
                ret = 0;
        } else if (tomb) {
                row = tomb;
        }

        if (!row) {
                row = calloc(1, sizeof(TbHashmapEntry));
                if (!row) {
                        return -1;
                }
        }

        row->hash = (void*)key;
        row->value = value;
        row->occ = true;
        if (parent != row) {
                parent->next = row;
        }

        return ret;
}

bool tb_hashmap_put(TbHashmap *self, const void *key, void *value)
{
        if (!self) {
                return false;
        }
        int inc;

        if (tb_hashmap_maybe_resize(self)) {
                if (!tb_hashmap_resize(self)) {
                        return false;
                }
        }
        unsigned hash = tb_hashmap_get_hash(self, key);
        inc = tb_hashmap_insert_bucket(self, self->buckets, self->n_buckets, hash, key, value);
        if (inc > 0) {
                self->size += inc;
                return true;
        } else {
                return false;
        }
}

static TbHashmapEntry *tb_hashmap_get_entry(TbHashmap *self, const void *key)
{
        if (!self) {
                return NULL;
        }

        unsigned hash = tb_hashmap_get_hash(self, key);
        TbHashmapEntry *row = &(self->buckets[hash % self->n_buckets]);

        while (row) {
                if (self->compare(row->hash, key)) {
                        return row;
                }
                row = row->next;
        }
        return NULL;
}

void *tb_hashmap_get(TbHashmap *self, const void *key)
{
        if (!self) {
                return NULL;
        }

        TbHashmapEntry *row = tb_hashmap_get_entry(self, key);
        if (row) {
                return row->value;
        }
        return NULL;
}

static bool tb_hashmap_remove_internal(TbHashmap *self, const void *key, bool remove)
{
        if (!self) {
                return false;
        }
        TbHashmapEntry *row = tb_hashmap_get_entry(self, key);

        if (!row) {
                return false;
        }

        if (remove) {
                if (self->key_free) {
                        self->key_free(row->hash);
                }
                if (self->value_free) {
                        self->value_free(row->value);
                }
        }
        self->size -= 1;
        row->hash = NULL;
        row->value = NULL;
        row->occ = false;

        return true;
}

bool tb_hashmap_steal(TbHashmap *self, const void *key)
{
        return tb_hashmap_remove_internal(self, key, false);
}

bool tb_hashmap_remove(TbHashmap *self, const void *key)
{
        return tb_hashmap_remove_internal(self, key, true);
}

bool tb_hashmap_contains(TbHashmap *self, const void *key)
{
        return (tb_hashmap_get(self, key)) != NULL;
}

static inline void tb_hashmap_free_bucket(TbHashmap *self, TbHashmapEntry *bucket, bool nuke)
{
        if (!self) {
                return;
        }
        TbHashmapEntry *tmp = bucket;
        TbHashmapEntry *bk = bucket;
        TbHashmapEntry *root = bucket;

        while (tmp) {
                bk = NULL;
                if (tmp->next) {
                        bk = tmp->next;
                }

                if (tmp->occ && nuke) {
                        if (self->key_free) {
                                self->key_free(tmp->hash);
                        }
                        if (self->value_free) {
                                self->value_free(tmp->value);
                        }
                }
                if (tmp != root) {
                        free(tmp);
                }
                tmp = bk;
        }
}

void tb_hashmap_free(TbHashmap *self)
{
        if (!self) {
                return;
        }
        for (int i = 0; i < self->n_buckets; i++) {
                TbHashmapEntry *row = &(self->buckets[i]);
                tb_hashmap_free_bucket(self, row, true);
        }
        if (self->buckets) {
                free(self->buckets);
        }

        free(self);

}

static void tb_hashmap_update_next_size(TbHashmap *self)
{
        if (!self) {
                return;
        }
        self->next_size = (int) (self->n_buckets * FULL_FACTOR);
}

int tb_hashmap_size(TbHashmap *self)
{
        if (!self) {
                return -1;
        }
        return self->size;
}

static bool tb_hashmap_resize(TbHashmap *self)
{
        if (!self || !self->buckets) {
                return false;
        }

        TbHashmapEntry *old_buckets = self->buckets;
        TbHashmapEntry *new_buckets = NULL;
        TbHashmapEntry *entry = NULL;
        int incr;

        int old_size, new_size;
        int items = 0;

        new_size = old_size = self->n_buckets;
        new_size *= INCREASE_FACTOR;

        new_buckets = calloc(new_size, sizeof(TbHashmapEntry));
        if (!new_buckets) {
                return false;
        }

        for (int i = 0; i < old_size; i++) {
                entry = &(old_buckets[i]);
                while (entry) {
                        if (entry->occ) {
                                unsigned hash = tb_hashmap_get_hash(self, entry->hash);
                                if ((incr = tb_hashmap_insert_bucket(self, new_buckets, new_size, hash, entry->hash, entry->value)) > 0) {
                                        items += incr;
                                } else {
                                        /* Likely a memory issue */
                                        goto failure;
                                }
                        }
                        entry = entry->next;
                }
        }
        /* Successfully resized - do this separately because we need to gaurantee old data is preserved */
        for (int i = 0; i < old_size; i++) {
                tb_hashmap_free_bucket(self, &(old_buckets[i]), false);
        }

        free(old_buckets);
        self->n_buckets = new_size;
        self->size = items;
        self->buckets = new_buckets;

        tb_hashmap_update_next_size(self);
        return true;

failure:
        for (int i = 0; i < new_size; i++) {
                tb_hashmap_free_bucket(self, &(new_buckets[i]), true);
        }
        free(new_buckets);
        return false;
}


void tb_hashmap_iter_init(TbHashmap *map, TbHashmapIter *citer)
{
        _TbHashmapIter *iter = NULL;
        if (!map || !citer) {
                return;
        }
        iter = (_TbHashmapIter*)citer;
        _TbHashmapIter it = {
               .bucket = -1,
               .map = map,
               .item = NULL,
        };
        *iter = it;
}

bool tb_hashmap_iter_next(TbHashmapIter *citer, void **key, void **value)
{
        _TbHashmapIter *iter = NULL;
        TbHashmapEntry *item = NULL;

        if (!citer) {
                return false;
        }

        iter = (_TbHashmapIter*)citer;
        if (!iter->map) {
                return false;
        }

        item = iter->item;

        for (;;) {
                if (iter->bucket >= iter->map->n_buckets) {
                        if (item && !item->next) {
                                return false;
                        }
                }
                if (!item) {
                        iter->bucket++;
                        if (iter->bucket > iter->map->n_buckets-1) {
                                return false;
                        }
                        item = &(iter->map->buckets[iter->bucket]);
                }
                if (item && item->occ) {
                        goto success;
                }
                item = item->next;
        }
        return false;

success:
        iter->item = item->next;
        if (key) {
                *key = item->hash;
        }
        if (value) {
                *value = item->value;
        }

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
