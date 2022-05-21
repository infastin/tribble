#ifndef HASHTABLEITER_H_MCORN7PA
#define HASHTABLEITER_H_MCORN7PA

#include "HashTable.h"

typedef struct _TrbHashTableIter TrbHashTableIter;

/**
 * TrbHashTableIter:
 * @ht: A pointer to the #TrbHashTable.
 * @slot: The current position in the hash table.
 *
 * An iterator over a hash table.
 *
 * This example show how to iterate over a hash table:
 * ```c
 * TrbHashTable ht;
 * trb_hash_table_init(&ht, 32, 8, 0xdeadbeef, jhash, (TrbCmpFunc) strcmp);
 *
 * trb_hash_table_insert(&ht, trb_get_arr(char, 32, "Mike Urasawa"), trb_get_ptr(u64, 42));
 * trb_hash_table_insert(&ht, trb_get_arr(char, 32, "Mario Franco"), trb_get_ptr(u64, 12));
 *
 * TrbHashTableIter iter;
 * trb_hash_table_iter_init(&iter, &ht);
 *
 * const char *name;
 * u64 *age;
 *
 * while (trb_hash_table_iter_next(&iter, (const void **) &name, (void **) &age)) {
 *     printf("--------------------\n");
 *     printf(" Name: %s\n", name);
 *     printf(" Age: %lu\n", *age);
 *     printf("--------------------\n");
 * }
 *
 * trb_hash_table_destroy(&ht, NULL, NULL);
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
struct _TrbHashTableIter {
	TrbHashTable *ht;
	usize slot;
	/* <private> */
	u8 status;
};

/**
 * trb_hash_table_iter_init:
 * @self: The pointer to the hash table iterator to be initialized.
 * @ht: The hash table to be iterated.
 *
 * Creates a new #HashTableIter.
 *
 * Returns: A new #HashTableIter.
 **/
TrbHashTableIter *trb_hash_table_iter_init(TrbHashTableIter *self, TrbHashTable *ht);

/**
 * trb_hash_table_iter_next:
 * @self: The hash table iterator.
 * @key: (optional) (out): The pointer to retrieve the key of the entry.
 * @value: (optional) (out): The pointer to retrieve the value of the entry.
 *
 * Advances the @self and retrieves the key and/or the value (or nothing)
 * that are now pointed to as a result of this advancement.
 *
 * Returns: %TRUE on success.
 **/
bool trb_hash_table_iter_next(TrbHashTableIter *self, const void **key, void **value);

/**
 * trb_hash_table_iter_replace:
 * @self: The hast table iterator.
 * @value: The value to replace with.
 *
 * Replaces the value currently pointed to by the iterator.
 *
 * Returns: %TRUE on success.
 **/
bool trb_hash_table_iter_replace(TrbHashTableIter *self, const void *value);

/**
 * trb_hash_table_iter_get:
 * @self: The hash table iterator.
 * @key: (optional) (out): The pointer to retrieve the key of the entry.
 * @value: (optional) (out): The pointer to retrieve the value of the entry.
 *
 * Retrieves the key and the value.
 *
 * Returns: %TRUE on success.
 **/
bool trb_hash_table_iter_get(TrbHashTableIter *self, void *key, void *value);

/**
 * trb_hash_table_iter_remove:
 * @self: The hash table iterator.
 * @key: (optional) (out): The pointer to retrieve the key of removed entry.
 * @value: (optional) (out): The pointer to retrieve the value of removed entry.
 *
 * Removes the entry currently pointer to by the iterator.
 *
 * Returns: %TRUE on success.
 **/
bool trb_hash_table_iter_remove(TrbHashTableIter *self, void *key, void *value);

#endif /* end of include guard: HASHTABLEITER_H_MCORN7PA */
