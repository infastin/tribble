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
 * Returns: A new hash table. Can return `NULL` if an error occurs
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
 * @padding: The padding between the key and the value.
 * @ret: The pointer to retrieve removed entries (can be `NULL`).
 *       The retrieved data will be an unaligned array of key:value pairs.
 * @len: The pointer to retrieve the number of entries (can be `NULL`).
 *
 * Removes all entries from the hash table.
 *
 * This example shows how to use this function:
 * ```c
 * HashTable ht;
 * ht_init(&ht, 30, 8, 0xdeadbeef, jhash, (CmpFunc) strcmp);
 *
 * ht_insert(&ht, get_arr(char, 30, "Mike Urasawa"), get_ptr(u64, 42));
 * ht_insert(&ht, get_arr(char, 32, "Mario Franco"), get_ptr(u64, 12));
 *
 * struct entry {
 *     char key[30];
 *
 *     // In this case the padding between the key and the value is 2.
 *     // You can manually pass 2 in the function or
 *     // use the distance_of(type, m1, m2) macro.
 *
 *     u64 age;
 * };
 *
 * usize len;
 * struct entry buf[ht.slots]; // It is better not to allocate a buffer in the stack.
 *                             // Because this way you can get a stack overflow error,
 *                             // if the buffer is too big.
 *
 * ht_remove_all(&ht, distance_of(struct entry, key, age), buf, &len);
 *
 * for (usize i = 0; i < len; ++i) {
 *     printf("--------------------\n");
 *     printf(" Name: %s\n", buf[i].key);
 *     printf(" Age: %lu\n", buf[i].age);
 *     printf("--------------------\n");
 * }
 *
 * ht_destroy(&ht, NULL, NULL);
 * ```
 *
 * You should get an output similar to the following:
 * ```
 * --------------------
 *  Name: Mario Franco
 *  Age: 12
 * --------------------
 * --------------------
 *  Name: Mike Urasawa
 *  Age: 42
 * --------------------
 * ```
 *
 * Returns: `TRUE` on success.
 **/
bool ht_remove_all(HashTable *ht, usize padding, void *ret, usize *len);

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
