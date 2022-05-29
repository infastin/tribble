#ifndef HASHTABLE_H_3HOTI89N
#define HASHTABLE_H_3HOTI89N

#include "Types.h"

typedef struct _TrbHashTable TrbHashTable;

/**
 * TrbHashTable:
 * @slots: The number of buckets.
 * @used: The number of used buckets.
 * @keysize: The key size.
 * @valuesize: The value size.
 * @seed: The seed for the @hash_func.
 * @hash_func: The function for hashing keys.
 * @cmp_func: The function for comparing keys.
 * @cmpd_func: The function for comparing keys using user data.
 * @data: User data.
 * @with_data: Indicates whether #TrbHashTable has been initialized with data or not.
 *
 * A hash table with quadratic probing and size 2^n.
 **/
struct _TrbHashTable {
	usize slots;
	usize used;
	usize keysize;
	usize valuesize;
	usize seed;
	TrbHashFunc hash_func;

	union {
		TrbCmpFunc cmp_func;
		TrbCmpDataFunc cmpd_func;
	};

	void *data;
	bool with_data;

	/* <private> */
	usize bucketsize;
	void *buckets;
};

/**
 * trb_hash_table_init:
 * @self: (nullable): The pointer to the hash table to be initialized.
 * @keysize: The size of keys in the hash table.
 * @valuesize: The size of values in the hash table.
 * @hash_func: (scope call): The function for hashing keys.
 * @cmp_func: (scope call): The function for comparing keys.
 *
 * Creates a new #TrbHashTable.
 *
 * Returns: (nullable): A new #TrbHashTable.
 * Can return %NULL if an error occurs.
 **/
TrbHashTable *trb_hash_table_init(
	TrbHashTable *self,
	usize keysize,
	usize valuesize,
	usize seed,
	TrbHashFunc hash_func,
	TrbCmpFunc cmp_func
);

/**
 * trb_hash_table_init_data:
 * @self: (nullable): The pointer to the hash table to be initialized.
 * @keysize: The size of keys in the hash table.
 * @valuesize: The size of values in the hash table.
 * @hash_func: The function for hashing keys.
 * @cmpd_func: The function for comparing keys using user data.
 * @data: User data.
 *
 * Creates a new #TrbHashTable with the comparison function that accepts user data.
 *
 * Returns: (nullable): A new #TrbHashTable.
 * Can return %NULL if an error occurs
 **/
TrbHashTable *trb_hash_table_init_data(
	TrbHashTable *self,
	usize keysize,
	usize valuesize,
	usize seed,
	TrbHashFunc hash_func,
	TrbCmpDataFunc cmpd_func,
	void *data
);

/**
 * trb_hash_table_add:
 * @self: The hash table where to add a new entry.
 * @key: The key of the entry.
 * @value: The value of the entry.
 *
 * Adds a new entry to the hash table.
 *
 * Returns: %TRUE on success.
 **/
bool trb_hash_table_add(TrbHashTable *self, const void *key, const void *value);

/**
 * trb_hash_table_insert:
 * @self: The hash table where to insert an entry.
 * @key: The key of the entry.
 * @value: The value of the entry.
 *
 * Inserts an entry to the hash table.
 * If the entry exists in the table, then replaces
 * its value with the given one.
 *
 * Returns: %TRUE on success.
 **/
bool trb_hash_table_insert(TrbHashTable *self, const void *key, const void *value);

/**
 * trb_hash_table_remove:
 * @self: The hash table where to remove the entry.
 * @key: The key of the entry.
 * @ret: (optional) (out): The pointer to retrieve the value of removed entry.
 *
 * Removes the entry from the hash table.
 *
 * Returns: %TRUE on success.
 **/
bool trb_hash_table_remove(TrbHashTable *self, const void *key, void *ret);

/**
 * trb_hash_table_remove_all:
 * @self: The hash table where to remove all entries.
 * @padding: The padding between the key and the value.
 * @ret: (optional) (out): The pointer to retrieve removed entries.
 *       The retrieved data will be an unaligned array of key:value pairs.
 * @len: (optional) (out): The pointer to retrieve the number of entries.
 *
 * Removes all entries from the hash table.
 *
 * This example shows how to use this function:
 * ```c
 * TrbHashTable ht;
 * trb_hash_table_init(&ht, 30, 8, 0xdeadbeef, jhash, (TrbCmpFunc) strcmp);
 *
 * trb_hash_table_insert(&ht, trb_get_arr(char, 30, "Mike Urasawa"), trb_get_ptr(u64, 42));
 * trb_hash_table_insert(&ht, trb_get_arr(char, 32, "Mario Franco"), trb_get_ptr(u64, 12));
 *
 * struct entry {
 *     char key[30];
 *
 *     // In this case the padding between the key and the value is 2.
 *     // You can manually pass 2 in the function or
 *     // use the trb_distance_of(type, m1, m2) macro.
 *
 *     u64 age;
 * };
 *
 * usize len;
 * struct entry buf[ht.slots]; // It is better not to allocate a buffer in the stack.
 *                             // Because this way you can get a stack overflow error,
 *                             // if the buffer is too big.
 *
 * trb_hash_table_remove_all(&ht, trb_distance_of(struct entry, key, age), buf, &len);
 *
 * for (usize i = 0; i < len; ++i) {
 *     printf("--------------------\n");
 *     printf(" Name: %s\n", buf[i].key);
 *     printf(" Age: %lu\n", buf[i].age);
 *     printf("--------------------\n");
 * }
 *
 * trb_hash_table_destroy(&ht, NULL, NULL);
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
 * Returns: %TRUE on success.
 **/
bool trb_hash_table_remove_all(TrbHashTable *self, usize padding, void *ret, usize *len);

/**
 * trb_hash_table_lookup:
 * @self: The hash table where to search for the entry.
 * @key: The key of the entry.
 * @ret: (optional) (out): The pointer to retrieve the value of the entry.
 *
 * Searches for the entry in the hash table.
 *
 * Returns: %TRUE if entry is found.
 **/
bool trb_hash_table_lookup(const TrbHashTable *self, const void *key, void *ret);

/**
 * trb_hash_table_destroy:
 * @self: The hash table which buckets will be freed.
 * @key_free_func: (scope call) (nullable): The function for freeing keys.
 * @value_free_func: (scope call) (nullable): The function for freeing values.
 *
 * Frees the hash table buckets.
 **/
void trb_hash_table_destroy(TrbHashTable *self, TrbFreeFunc key_free_func, TrbFreeFunc value_free_func);

/**
 * trb_hash_table_free:
 * @self: The hash table to be freed.
 * @key_free_func: (scope call) (nullable): The function for freeing keys.
 * @value_free_func: (scope call) (nullable): The function for freeing values.
 *
 * Frees the hash table completely.
 **/
void trb_hash_table_free(TrbHashTable *self, TrbFreeFunc key_free_func, TrbFreeFunc value_free_func);

#endif /* end of include guard: HASHTABLE_H_3HOTI89N */
