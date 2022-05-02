#include "HashTable.h"

#include "Messages.h"

#include <memory.h>

#define HT_INIT_SLOTS 16

#define htb_bucket(ht, buckets, i) ((void *) &((char *) (buckets))[(i) * (ht)->bucketsize])
#define htb_key(ht, buckets, i) (htb_bucket(ht, buckets, i))
#define htb_value(ht, buckets, i) ((void *) (((char *) htb_bucket(ht, buckets, i)) + (ht)->keysize))
#define htb_occupied(ht, buckets, i) ((bool *) (((char *) htb_bucket(ht, buckets, i)) + (ht)->keysize + (ht)->valuesize))

#define ht_bucket(ht, i) (htb_bucket(ht, (ht)->buckets, i))
#define ht_key(ht, i) (htb_key(ht, (ht)->buckets, i))
#define ht_value(ht, i) (htb_value(ht, (ht)->buckets, i))
#define ht_occupied(ht, i) (htb_occupied(ht, (ht)->buckets, i))

HashTable *ht_init(HashTable *ht, u32 keysize, u32 valuesize, HashFunc hash_func, CmpFunc cmp_func)
{
	return_val_if_fail(hash_func != NULL, NULL);
	return_val_if_fail(cmp_func != NULL, NULL);

	bool was_allocated = FALSE;

	if (ht == NULL) {
		ht = talloc(HashTable, 1);

		if (ht == NULL) {
			msg_error("couldn't allocate memory for the hash table!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	u32 bucketsize = keysize + valuesize + 1;
	ht->buckets = calloc(HT_INIT_SLOTS, bucketsize);

	if (ht->buckets == NULL) {
		if (was_allocated)
			free(ht);

		msg_error("couldn't allocate memory for the hash table buckets!");
		return NULL;
	}

	ht->slots = HT_INIT_SLOTS;
	ht->used = 0;
	ht->keysize = keysize;
	ht->valuesize = valuesize;
	ht->bucketsize = bucketsize;
	ht->seed = 0x31415926;
	ht->hash_func = hash_func;
	ht->cmp_func = cmp_func;

	return ht;
}

static bool __ht_add(HashTable *ht, u32 slots, void *buckets, const void *key, void *value)
{
	u32 hash = ht->hash_func(key, ht->keysize, ht->seed);
	u32 pos = hash & (slots - 1);

	if (*htb_occupied(ht, buckets, pos)) {
		if (ht->cmp_func(key, htb_key(ht, buckets, pos)) == 0)
			return FALSE;

		u32 i = pos;
		u32 slot = ((i + i * i) >> 1) & (ht->slots - 1);

		while (1) {
			if (*htb_occupied(ht, buckets, slot) == FALSE) {
				pos = slot;
				break;
			}

			if (ht->cmp_func(key, htb_key(ht, buckets, slot)) == 0)
				return FALSE;

			if (i++ >= slots)
				i = 0;

			slot = ((i + i * i) >> 1) & (ht->slots - 1);
		}
	}

	memcpy(htb_key(ht, buckets, pos), key, ht->keysize);

	if (value != NULL)
		memcpy(htb_value(ht, buckets, pos), value, ht->valuesize);
	else
		memset(htb_value(ht, buckets, pos), 0, ht->valuesize);

	memset(htb_occupied(ht, buckets, pos), 1, 1);

	return TRUE;
}

static bool ht_resize(HashTable *ht, u32 new_slots)
{
	void *buckets = calloc(new_slots, ht->bucketsize);
	if (buckets == NULL) {
		msg_error("couldn't reallocate memory for the hash table buckets!");
		return FALSE;
	}

	for (u32 i = 0; i < ht->slots; ++i) {
		if (*ht_occupied(ht, i)) {
			if (__ht_add(ht, new_slots, buckets, ht_key(ht, i), ht_value(ht, i)) == FALSE) {
				free(buckets);
				return FALSE;
			}
		}
	}

	free(ht->buckets);
	ht->buckets = buckets;
	ht->slots = new_slots;

	return TRUE;
}

bool ht_insert(HashTable *ht, const void *key, void *value)
{
	return_val_if_fail(ht != NULL, FALSE);
	return_val_if_fail(key != NULL, FALSE);

	float load_factor = (float) ht->used / (float) ht->slots;
	if (load_factor >= 0.6) {
		u32 new_slots = ht->slots << 1;
		if (ht->slots > new_slots)
			return FALSE;

		if (!ht_resize(ht, new_slots))
			return FALSE;
	}

	bool only_change_value = FALSE;
	u32 hash = ht->hash_func(key, ht->keysize, ht->seed);
	u32 pos = hash & (ht->slots - 1);

	if (*ht_occupied(ht, pos) && ht->cmp_func(key, ht_key(ht, pos)) != 0) {
		u32 i = pos;
		u32 slot = ((i + i * i) >> 1) & (ht->slots - 1);

		while (1) {
			if (*ht_occupied(ht, slot) == FALSE) {
				pos = slot;
				break;
			}

			if (ht->cmp_func(key, ht_key(ht, slot)) == 0) {
				only_change_value = TRUE;
				pos = slot;
				break;
			}

			if (i >= ht->slots)
				i = 0;

			slot = ((i + i * i) >> 1) & (ht->slots - 1);
		}
	}

	if (!only_change_value)
		memcpy(ht_key(ht, pos), key, ht->keysize);
	else
		ht->used++;

	if (value != NULL)
		memcpy(ht_value(ht, pos), value, ht->valuesize);
	else
		memset(ht_value(ht, pos), 0, ht->valuesize);

	memset(ht_occupied(ht, pos), 1, 1);

	return TRUE;
}

bool ht_add(HashTable *ht, const void *key, void *value)
{
	return_val_if_fail(ht != NULL, FALSE);
	return_val_if_fail(key != NULL, FALSE);

	float load_factor = (float) ht->used / (float) ht->slots;
	if (load_factor >= 0.6) {
		u32 new_slots = ht->slots << 1;
		if (ht->slots > new_slots)
			return FALSE;

		if (!ht_resize(ht, new_slots))
			return FALSE;
	}

	if (__ht_add(ht, ht->slots, ht->buckets, key, value)) {
		ht->used++;
		return TRUE;
	}

	return FALSE;
}

bool ht_remove(HashTable *ht, const void *key, void *ret)
{
	return_val_if_fail(ht != NULL, FALSE);
	return_val_if_fail(key != NULL, FALSE);

	float load_factor = (float) ht->used / (float) ht->slots;
	if (load_factor <= 0.4) {
		if (!ht_resize(ht, ht->slots >> 1))
			return FALSE;
	}

	u32 hash = ht->hash_func(key, ht->keysize, ht->seed);
	u32 pos = hash & (ht->slots - 1);

	if (*ht_occupied(ht, pos) == FALSE)
		return FALSE;

	if (ht->cmp_func(key, ht_key(ht, pos))) {
		u32 i = pos;
		u32 slot = ((i + i * i) >> 1) & (ht->slots - 1);

		while (1) {
			if (*ht_occupied(ht, slot) == FALSE)
				return FALSE;

			if (ht->cmp_func(key, ht_key(ht, slot)) == 0) {
				pos = slot;
				break;
			}

			if (i++ >= ht->slots)
				i = 0;

			slot = ((i + i * i) >> 1) & (ht->slots - 1);
		}
	}

	if (ret != NULL)
		memcpy(ret, ht_value(ht, pos), ht->valuesize);

	memset(ht_occupied(ht, pos), 0, 1);

	ht->used--;

	return TRUE;
}

bool ht_contains(HashTable *ht, const void *key)
{
	return_val_if_fail(ht != NULL, FALSE);
	return_val_if_fail(key != NULL, FALSE);

	u32 hash = ht->hash_func(key, ht->keysize, ht->seed);
	u32 pos = hash & (ht->slots - 1);

	if (*ht_occupied(ht, pos)) {
		if (ht->cmp_func(key, ht_key(ht, pos)) == 0)
			return TRUE;

		u32 i = pos;
		u32 slot = ((i + i * i) >> 1) & (ht->slots - 1);

		while (1) {
			if (*ht_occupied(ht, slot) == FALSE)
				return FALSE;

			if (ht->cmp_func(key, ht_key(ht, slot)) == 0)
				return TRUE;

			if (i++ >= ht->slots)
				i = 0;

			slot = ((i + i * i) >> 1) & (ht->slots - 1);
		}
	}

	return FALSE;
}

bool ht_lookup(HashTable *ht, const void *key, void *ret)
{
	return_val_if_fail(ht != NULL, FALSE);
	return_val_if_fail(key != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	u32 hash = ht->hash_func(key, ht->keysize, ht->seed);
	u32 pos = hash & (ht->slots - 1);

	if (*ht_occupied(ht, pos)) {
		if (ht->cmp_func(key, ht_key(ht, pos)) == 0) {
			memcpy(ret, ht_value(ht, pos), ht->valuesize);
			return TRUE;
		}

		u32 i = pos;
		u32 slot = ((i + i * i) >> 1) & (ht->slots - 1);

		while (1) {
			if (*ht_occupied(ht, slot) == FALSE)
				return FALSE;

			if (ht->cmp_func(key, ht_key(ht, slot)) == 0) {
				memcpy(ret, ht_value(ht, slot), ht->valuesize);
				return TRUE;
			}

			if (i++ >= ht->slots)
				i = 0;

			slot = ((i + i * i) >> 1) & (ht->slots - 1);
		}
	}

	return FALSE;
}

void ht_remove_all(HashTable *ht, void *ret, u32 *len, bool to_copy)
{
}

void ht_purge(HashTable *ht)
{
	return_if_fail(ht != NULL);

	if (ht->buckets != NULL) {
		free(ht->buckets);

		ht->buckets = NULL;
		ht->slots = 0;
		ht->used = 0;
	}
}

void ht_free(HashTable *ht)
{
	return_if_fail(ht != NULL);

	ht_purge(ht);
	free(ht);
}
