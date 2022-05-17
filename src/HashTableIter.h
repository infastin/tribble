#ifndef HASHTABLEITER_H_MCORN7PA
#define HASHTABLEITER_H_MCORN7PA

#include "HashTable.h"

/**
 * SECTION: HashTableIter
 * @title: HashTableIter
 *
 * This example show how to iterate over a hash table:
 * ```c
 * HashTable ht;
 * ht_init(&ht, 32, 8, 0xdeadbeef, jhash, (CmpFunc) strcmp);
 *
 * ht_insert(&ht, get_arr(char, 32, "Mike Urasawa"), get_ptr(u64, 42));
 * ht_insert(&ht, get_arr(char, 32, "Mario Franco"), get_ptr(u64, 12));
 *
 * HashTableIter iter;
 * ht_iter_init(&iter, &ht);
 *
 * const char *name;
 * u64 *age;
 *
 * while (ht_iter_next(&iter, (const void **) &name, (void **) &age)) {
 *     printf("--------------------\n");
 *     printf(" Name: %s\n", name);
 *     printf(" Age: %lu\n", *age);
 *     printf("--------------------\n");
 * }
 *
 * ht_destroy(&ht, NULL, NULL);
 * ```
 *
 * You should get an output similar to the following:
 * ```
 * --------------------
 *  Name: Mike Urasawa
 *  Age: 42
 * --------------------
 * --------------------
 *  Name: Mario Franco
 *  Age: 12
 * --------------------
 * ```
 **/

typedef struct _HashTableIter HashTableIter;

struct _HashTableIter {
	HashTable *ht;
	usize slot;
	u8 status;
};

/**
 * ht_iter_init:
 * @hti: The pointer to the hash table iterator to be initialized (can be `NULL`).
 * @ht: The hash table to be iterated.
 *
 * Creates a new hash table iterator.
 *
 * Returns: A new hash table iterator.
 **/
HashTableIter *ht_iter_init(HashTableIter *hti, HashTable *ht);

/**
 * ht_iter_next:
 * @hti: The hash table iterator.
 * @key: The pointer to retrieve the key of the entry (can be `NULL`).
 * @value: The pointer to retrieve the value of the entry (can be `NULL`).
 *
 * Advances the @hti and retrieves the key and/or the value (or nothing)
 * that are now pointed to as a result of this advancement.
 *
 * Returns: `TRUE` on success.
 **/
bool ht_iter_next(HashTableIter *hti, const void **key, void **value);

/**
 * ht_iter_replace:
 * @hti: The hast table iterator.
 * @value: The value to replace with.
 *
 * Replaces the value currently pointed to by the iterator.
 *
 * Returns: `TRUE` on success.
 **/
bool ht_iter_replace(HashTableIter *hti, const void *value);

/**
 * ht_iter_get:
 * @hti: The hash table iterator.
 * @key: The pointer to retrieve the key of the entry (can be `NULL`).
 * @value: The pointer to retrieve the value of the entry (can be `NULL`).
 *
 * Retrieves the key and the value.
 *
 * Returns: `TRUE` on success.
 **/
bool ht_iter_get(HashTableIter *hti, void *key, void *value);

/**
 * ht_iter_remove:
 * @hti: The hash table iterator.
 * @key: The pointer to retrieve the key of removed entry (can be `NULL`).
 * @value: The pointer to retrieve the value of removed entry (can be `NULL`).
 *
 * Removes the entry currently pointer to by the iterator.
 *
 * Returns: `TRUE` on success.
 **/
bool ht_iter_remove(HashTableIter *hti, void *key, void *value);

#endif /* end of include guard: HASHTABLEITER_H_MCORN7PA */
