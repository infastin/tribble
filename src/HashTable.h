#ifndef HASHTABLE_H_3HOTI89N
#define HASHTABLE_H_3HOTI89N

#include "Types.h"

typedef struct _HashTable HashTable;

struct _HashTable {
	/* Public */
	u32 slots;
	u32 used;

	/* Private */
	u32 keysize;
	u32 valuesize;
	u32 bucketsize;
	u32 seed;
	HashFunc hash_func;
	CmpFunc cmp_func;
	void *buckets;
};

/**
 * @brief Creates a new HashTable
 *
 * @param ht The pointer to the hash table to be initialized (can be NULL)
 * @param keysize
 * @param valuesize
 * @param hash_func The function to create a hash
 * @param cmp_func The function to compare keys
 *
 * @return A new HashTable
 */
HashTable *ht_init(HashTable *ht, u32 keysize, u32 valuesize, HashFunc hash_func, CmpFunc cmp_func);

/**
 * @brief Adds a new entry to the hash table
 *
 * @param ht
 * @param key
 * @param value
 *
 * @return TRUE on success
 */
bool ht_add(HashTable *ht, const void *key, void *value);

/**
 * @brief
 *
 * @param ht
 * @param key
 * @param value
 *
 * @return TRUE on success
 */
bool ht_insert(HashTable *ht, const void *key, void *value);

/**
 * @brief
 *
 * @param ht
 * @param key
 * @param ret
 *
 * @return TRUE on success
 */
bool ht_remove(HashTable *ht, const void *key, void *ret);

/**
 * @brief 
 *
 * @param ht
 * @param ret
 * @param len
 * @param to_copy
 */
void ht_remove_all(HashTable *ht, void *ret, u32 *len, bool to_copy);

/**
 * @brief
 *
 * @param ht
 * @param key
 *
 * @return TRUE on success
 */
bool ht_contains(HashTable *ht, const void *key);

/**
 * @brief
 *
 * @param ht
 * @param key
 * @param ret
 *
 * @return TRUE on success
 */
bool ht_lookup(HashTable *ht, const void *key, void *ret);

/**
 * @brief
 *
 * @param ht
 */
void ht_purge(HashTable *ht);

/**
 * @brief
 *
 * @param ht
 */
void ht_free(HashTable *ht);

#endif /* end of include guard: HASHTABLE_H_3HOTI89N */