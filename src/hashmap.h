/*
 * This file is part of libthingamabob.
 *
 * Copyright (C) 2016 Intel Corporation
 *
 * libthingamabob is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This is a chained Hashmap implementation. It focuses on being clean
 * and efficient, and is comparable (at -O2) to open addressing hashmaps
 * up until around 105,000 elements, where open addressing will begin
 * to come out in front. At 1 million elements, open addressing is a clear
 * winner, however currently we only need a maximum of 70k elements in
 * our implementation.
 *
 * Given the memory required to even begin to deal with 1 million elements,
 * it becomes very questionable whether a hashmap should have even been
 * used in the first place (mmap'd db?)
 */

#pragma once

#define _GNU_SOURCE

#include <stdbool.h>
#include <stdint.h>

/* Convert between uint and void* */
#define HASH_KEY(x) ((void*)((uintptr_t)(x)))
#define HASH_VALUE(x) HASH_KEY(x)
#define UNHASH_KEY(x) ((unsigned int)((uintptr_t)(x)))
#define UNHASH_VALUE(x) UNHASH_KEY(x)

typedef struct TbHashmap TbHashmap;

/**
 * Iteration object
 */
typedef struct TbHashmapIter {
        int n0;
        void *n1;
        void *n2;
} TbHashmapIter;


/**
 * Hash comparison function definition
 *
 * @param l First value to compare
 * @param r Second value to compare
 *
 * @return true if l and r both match, otherwise false
 */
typedef bool (*hash_compare_func)(const void *l, const void *r);

/**
 * Hash creation function definition
 *
 * @param key Key to generate a hash for
 *
 * @return an unsigned integer hash result
 */
typedef unsigned (*hash_create_func)(const void *key);

/**
 * Callback definition to free keys and values
 *
 * @param p Single-depth pointer to either a key or value that should be freed
 */
typedef void (*hash_free_func)(void *p);

/**
 * Default hash/comparison functions
 *
 * @note These are only used for comparison of *keys*, not values. Unless
 * explicitly using string keys, you should most likely stick with the default
 * simple_hash and simple_compare functions
 */

/* Default string hash */
static inline unsigned string_hash(const void *key)
{
        unsigned hash = 5381;
        const signed char *c;

        /* DJB's hash function */
        for (c = key; *c != '\0'; c++) {
                hash = (hash << 5) + hash + (unsigned)*c;
        }
        return hash;
}

/**
 * Trivial pointer->uint hash
 */
static inline unsigned simple_hash(const void *source)
{
        return UNHASH_KEY(source);
}

/**
 * Comparison of string keys
 */
static inline bool string_compare(const void *l, const void *r)
{
        if (!l || !r) {
                return false;
        }
        return (strcmp(l,r) == 0);
}

/**
 * Trivial pointer comparison
 */
static inline bool simple_compare(const void *l, const void *r)
{
        return (l == r);
}


/**
 * Create a new TbHashmap
 *
 * @param hash Hash creation function
 * @param compare Key comparison function
 *
 * @return A newly allocated TbHashmap
 */
TbHashmap *tb_hashmap_new(hash_create_func hash, hash_compare_func compare);

/**
 * Create a new TbHashmap with cleanup functions
 *
 * @param hash Hash creation function
 * @param compare Key comparison function
 * @param key_free Function to free keys when removed/destroyed
 * @param value_free Function to free values when removed/destroyed
 *
 * @return A newly allocated TbHashmap
 */
TbHashmap *tb_hashmap_new_full(hash_create_func hash, hash_compare_func compare, hash_free_func key_free, hash_free_func value_free);

/**
 * Store a key/value pair in the hashmap
 *
 * @note This will displace duplicate keys, and may free both the key
 * and value if key_free and value_free are non null
 *
 * @param key Key to store in the hashmap
 * @param value Value to be associated with the key
 *
 * @return true if the operation succeeded.
 */
bool tb_hashmap_put(TbHashmap *map, const void *key, void *value);

/**
 * Get the value associated with the unique key
 *
 * @param key Unique key to obtain a value for
 * @return The associated value if it exists, otherwise NULL
 */
void *tb_hashmap_get(TbHashmap *map, const void *key);

/**
 * Determine if the key has an associated value in the TbHashmap
 *
 * @param key Unique key to check value for
 * @return True if the key exists in the hashmap
 */
bool tb_hashmap_contains(TbHashmap *self, const void *key);

/**
 * Free the given TbHashmap, and all keys/values if appropriate
 */
void tb_hashmap_free(TbHashmap *map);

/**
 * Remove the value and key identified by key.
 *
 * @note If key_free or value_free are non-NULL, they will be invoked
 * for both the key and value being removed
 *
 * @param key The unique key to remove
 * @return true if the key/value pair were removed
 */
bool tb_hashmap_remove(TbHashmap *map, const void *key);

/**
 * Remove the value and key identified by key without freeing them
 *
 * @return true if the key/value pair were stolen
 */
bool tb_hashmap_steal(TbHashmap *map, const void *key);

/**
 * Return the current size of the hashmap
 *
 * @return size of the current hashmap (element count)
 */
int tb_hashmap_size(TbHashmap *map);

/**
 * Initialise a TbHashmapIter for iteration purposes
 *
 * @note The iter *must* be re-inited to re-use, or if it becomes
 * exhausted from a previous iteration run
 *
 * @param iter pointer to a TbHashmapIter
 */
void tb_hashmap_iter_init(TbHashmap *map, TbHashmapIter *iter);

/**
 * Iterate every key/value pair in the hashmap
 *
 * @param iter A correctly initialised TbHashmapIter
 * @param key Pointers to store the key in, may be NULL to skip
 * @param value Pointer to store the value in, may be NULL to skip
 *
 * @return true if it's possible to iterate
 */
bool tb_hashmap_iter_next(TbHashmapIter *iter, void **key, void **value);

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
