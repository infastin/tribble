#ifndef HASHTABLE_H_3HOTI89N
#define HASHTABLE_H_3HOTI89N

#include "Types.h"

typedef struct _HashTable HashTable;

struct _HashTable {
	usize slots;
	usize used;
	usize keysize;
	usize valuesize;
	usize seed;
	usize bucketsize;
	HashFunc hash_func;
	CmpFunc cmp_func;
	void *buckets;
};

/**
 * ht_init:
 * @ht: The pointer to the hash table to be initialized (can be `NULL`).
 * @keysize: The size of keys in the hash table.
 * @valuesize: The size of values in the hash table.
 * @hash_func: The function for hashing keys.
 * @cmp_func: The function for comparing keys.
 *
 * Creates a new hash table.
 *
 * Returns: A new hash table.
 **/
HashTable *ht_init(HashTable *ht, usize keysize, usize valuesize, usize seed, HashFunc hash_func, CmpFunc cmp_func);

/**
 * ht_add:
 * @ht: The hash table where to add a new entry.
 * @key: The key of the entry.
 * @value: The value of the entry (can be `NULL`).
 *
 * Adds a new entry to the hash table.
 *
 * Returns: `TRUE` on success.
 **/
bool ht_add(HashTable *ht, const void *key, const void *value);

/**
 * ht_insert:
 * @ht: The hash table where to insert an entry.
 * @key: The key of the entry.
 * @value: The value of the entry (can be `NULL`).
 *
 * Inserts an entry to the hash table.
 * If the entry exists in the table, then replaces
 * its value with the given one.
 *
 * Returns: `TRUE` on success.
 **/
bool ht_insert(HashTable *ht, const void *key, const void *value);

/**
 * ht_remove:
 * @ht: The hash table where to remove the entry.
 * @key: The key of the entry.
 * @ret: The pointer to retrieve the value of removed entry (can be `NULL`).
 *
 * Removes the entry from the hash table.
 *
 * Returns: `TRUE` on success.
 **/
bool ht_remove(HashTable *ht, const void *key, void *ret);

/**
 * ht_remove_all:
 * @ht: The hash table where to remove all entries.
 * @ret: The pointer to retrieve removed entries (can be `NULL`).
 *       The retrieved data will be an unaligned array of key:value pairs.
 * @len: The pointer to retrieve the number of entries (can be `NULL`).
 *
 * Removes all entries from the hash table.
 **/
void ht_remove_all(HashTable *ht, void *ret, usize *len);

/**
 * ht_lookup:
 * @ht: The hash table where to search for the entry.
 * @key: The key of the entry.
 * @ret: The pointer to retrieve the value of the entry (can be `NULL`).
 *
 * Searches for the entry in the hash table.
 *
 * Returns: `TRUE` if entry is found.
 **/
bool ht_lookup(const HashTable *ht, const void *key, void *ret);

/**
 * ht_destroy:
 * @ht: The hash table which buckets will be freed.
 * @key_free_func: The function for freeing keys (can be `NULL`).
 * @value_free_func: The function for freeing values (can be `NULL`).
 *
 * Frees the hash table buckets.
 **/
void ht_destroy(HashTable *ht, FreeFunc key_free_func, FreeFunc value_free_func);

/**
 * ht_free:
 * @ht: The hash table to be freed.
 * @key_free_func: The function for freeing keys (can be `NULL`).
 * @value_free_func: The function for freeing values (can be `NULL`).
 *
 * Frees the hash table completely.
 **/
void ht_free(HashTable *ht, FreeFunc key_free_func, FreeFunc value_free_func);

#endif /* end of include guard: HASHTABLE_H_3HOTI89N */
