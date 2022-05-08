#ifndef HASHTABLE_H_3HOTI89N
#define HASHTABLE_H_3HOTI89N

#include "Types.h"

typedef struct _HashTable HashTable;

struct _HashTable {
	usize slots;
	usize used;
	usize keysize;
	usize valuesize;
	usize bucketsize;
	usize seed;
	HashFunc hash_func;
	CmpFunc cmp_func;
	void *buckets;
};

/**
 * @brief Creates a new HashTable.
 *
 * @param ht The pointer to the hash table to be initialized (can be NULL).
 * @param keysize The size of keys in the hash table.
 * @param valuesize The size of values in the hash table.
 * @param hash_func The function for hashing keys.
 * @param cmp_func The function for comparing keys.
 *
 * @return A new HashTable.
 */
HashTable *ht_init(HashTable *ht, usize keysize, usize valuesize, usize seed, HashFunc hash_func, CmpFunc cmp_func);

/**
 * @brief Adds a new entry to the hash table.
 *
 * @param ht The hash table where to add a new entry.
 * @param key The key of the entry.
 * @param value The value of the entry.
 *
 * @return TRUE on success.
 */
bool ht_add(HashTable *ht, const void *key, const void *value);

/**
 * @brief Inserts an entry to the hash table.
 * 		  If the entry exists in the table, then replaces
 * 		  its value with the given one.
 *
 * @param ht The hash table where to insert an entry.
 * @param key The key of the entry.
 * @param value The value of the entry.
 *
 * @return TRUE on success.
 */
bool ht_insert(HashTable *ht, const void *key, const void *value);

/**
 * @brief Removes the entry from the hash table.
 *
 * @param ht The hash table where to remove the entry.
 * @param key The key of the entry.
 * @param ret The pointer to retrieve the value of removed entry (can be NULL).
 *
 * @return TRUE on success.
 */
bool ht_remove(HashTable *ht, const void *key, void *ret);

/**
 * @brief Removes all entries from the hash table.
 *
 * @param ht The hash table where to remove all entries.
 * @param ret The pointer to retrieve removed entries (can be NULL).
 * 			  The retrieved data will be an unaligned array of key:value pairs.
 * @param len The pointer to retrieve the number of entries (can be NULL).
 */
void ht_remove_all(HashTable *ht, void *ret, usize *len);

/**
 * @brief Checks whether the entry exists in the hash table or not.
 *
 * @param ht The hash table where to search for the entry.
 * @param key The key of the entry.
 *
 * @return TRUE if exists.
 */
bool ht_contains(const HashTable *ht, const void *key);

/**
 * @brief Finds the entry in the hash table.
 *
 * @param ht The hash table where to search for the entry.
 * @param key The key of the entry.
 * @param ret The pointer to retrieve the value of the entry.
 *
 * @return TRUE if entry is found.
 */
bool ht_lookup(const HashTable *ht, const void *key, void *ret);

/**
 * @brief Frees the hash table buckets.
 *
 * @param ht The hash table which buckets will be freed.
 * @param key_free_func The function for freeing keys.
 * @param value_free_func The function for freeing values.
 */
void ht_purge(HashTable *ht, FreeFunc key_free_func, FreeFunc value_free_func);

/**
 * @brief Frees the hash table completely.
 *
 * @param ht The hash table to be freed.
 * @param key_free_func The function for freeing keys.
 * @param value_free_func The function for freeing values.
 */
void ht_free(HashTable *ht, FreeFunc key_free_func, FreeFunc value_free_func);

#endif /* end of include guard: HASHTABLE_H_3HOTI89N */
