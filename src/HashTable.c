#include "HashTable.h"

#include "Messages.h"

#include <memory.h>

#define HT_INIT_SLOTS 16

#define htb_bucket(ht, buckets, i) ((void *) (((char *) buckets) + (i) * (ht)->bucketsize))
#define htb_key(ht, buckets, i) (htb_bucket(ht, buckets, i))
#define htb_value(ht, buckets, i) ((void *) (((char *) htb_bucket(ht, buckets, i)) + (ht)->keysize))
#define htb_occupied(ht, buckets, i) ((bool *) (((char *) htb_bucket(ht, buckets, i)) + (ht)->keysize + (ht)->valuesize))

#define ht_bucket(ht, i) (htb_bucket(ht, (ht)->buckets, i))
#define ht_key(ht, i) (htb_key(ht, (ht)->buckets, i))
#define ht_value(ht, i) (htb_value(ht, (ht)->buckets, i))
#define ht_occupied(ht, i) (htb_occupied(ht, (ht)->buckets, i))

HashTable *ht_init(HashTable *ht, usize keysize, usize valuesize, usize seed, HashFunc hash_func, CmpFunc cmp_func)
{
	return_val_if_fail(hash_func != NULL, NULL);
	return_val_if_fail(cmp_func != NULL, NULL);
	return_val_if_fail(keysize != 0, NULL);
	return_val_if_fail(valuesize != 0, NULL);

	if (keysize > USIZE_MAX - valuesize || keysize > USIZE_MAX - valuesize - 1) {
		msg_error("bucket size overflow!");
		return FALSE;
	}

	usize bucketsize = keysize + valuesize + 1;

	if (bucketsize > USIZE_MAX / HT_INIT_SLOTS) {
		msg_error("hash table capacity overflow!");
		return FALSE;
	}

	bool was_allocated = FALSE;

	if (ht == NULL) {
		ht = talloc(HashTable, 1);

		if (ht == NULL) {
			msg_error("couldn't allocate memory for the hash table!");
			return NULL;
		}

		was_allocated = TRUE;
	}

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
	ht->seed = seed;
	ht->hash_func = hash_func;
	ht->cmp_func = cmp_func;

	return ht;
}

static bool __ht_add(HashTable *ht, usize slots, void *buckets, const void *key, const void *value)
{
	usize hash = ht->hash_func(key, ht->keysize, ht->seed);
	usize pos = hash & (slots - 1);

	if (*htb_occupied(ht, buckets, pos)) {
		if (ht->cmp_func(key, htb_key(ht, buckets, pos)) == 0)
			return FALSE;

		usize i = pos;
		usize slot = ((i + i * i) >> 1) & (ht->slots - 1);

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

static bool ht_resize(HashTable *ht, usize new_slots)
{
	if (ht->bucketsize > USIZE_MAX / new_slots) {
		msg_error("hash table capacity overflow!");
		return FALSE;
	}

	void *buckets = calloc(new_slots, ht->bucketsize);
	if (buckets == NULL) {
		msg_error("couldn't reallocate memory for the hash table buckets!");
		return FALSE;
	}

	for (usize i = 0; i < ht->slots; ++i) {
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

bool ht_insert(HashTable *ht, const void *key, const void *value)
{
	return_val_if_fail(ht != NULL, FALSE);
	return_val_if_fail(key != NULL, FALSE);

	if (ht->slots == 0) {
		if (!ht_resize(ht, HT_INIT_SLOTS))
			return FALSE;
	} else {
		f64 load_factor = (f64) ht->used / (f64) ht->slots;
		if (load_factor >= 0.6) {
			usize new_slots = ht->slots << 1;
			if (ht->slots > new_slots) {
				msg_error("hash table capacity overflow!");
				return FALSE;
			}

			if (!ht_resize(ht, new_slots))
				return FALSE;
		}
	}

	bool only_change_value = FALSE;
	usize hash = ht->hash_func(key, ht->keysize, ht->seed);
	usize pos = hash & (ht->slots - 1);

	if (*ht_occupied(ht, pos) && ht->cmp_func(key, ht_key(ht, pos)) != 0) {
		usize i = pos;
		usize slot = ((i + i * i) >> 1) & (ht->slots - 1);

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

	if (!only_change_value) {
		memcpy(ht_key(ht, pos), key, ht->keysize);
		ht->used++;
	}

	if (value != NULL)
		memcpy(ht_value(ht, pos), value, ht->valuesize);
	else
		memset(ht_value(ht, pos), 0, ht->valuesize);

	memset(ht_occupied(ht, pos), 1, 1);

	return TRUE;
}

bool ht_add(HashTable *ht, const void *key, const void *value)
{
	return_val_if_fail(ht != NULL, FALSE);
	return_val_if_fail(key != NULL, FALSE);

	if (ht->slots == 0) {
		if (!ht_resize(ht, HT_INIT_SLOTS))
			return FALSE;
	} else {
		f64 load_factor = (f64) ht->used / (f64) ht->slots;
		if (load_factor >= 0.6) {
			usize new_slots = ht->slots << 1;
			if (ht->slots > new_slots) {
				msg_error("hash table capacity overflow!");
				return FALSE;
			}

			if (!ht_resize(ht, new_slots))
				return FALSE;
		}
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

	if (ht->slots == 0) {
		msg_warn("hash table capacity is zero!");
		return FALSE;
	}

	if (ht->slots > HT_INIT_SLOTS) {
		f64 load_factor = (f64) ht->used / (f64) ht->slots;
		if (load_factor <= 0.4) {
			if (!ht_resize(ht, ht->slots >> 1))
				return FALSE;
		}
	}

	usize hash = ht->hash_func(key, ht->keysize, ht->seed);
	usize pos = hash & (ht->slots - 1);

	if (*ht_occupied(ht, pos) == FALSE)
		return FALSE;

	if (ht->cmp_func(key, ht_key(ht, pos))) {
		usize i = pos;
		usize slot = ((i + i * i) >> 1) & (ht->slots - 1);

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

bool ht_lookup(const HashTable *ht, const void *key, void *ret)
{
	return_val_if_fail(ht != NULL, FALSE);
	return_val_if_fail(key != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (ht->slots == 0) {
		msg_warn("hash table capacity is zero!");
		return FALSE;
	}

	usize hash = ht->hash_func(key, ht->keysize, ht->seed);
	usize pos = hash & (ht->slots - 1);

	if (*ht_occupied(ht, pos)) {
		if (ht->cmp_func(key, ht_key(ht, pos)) == 0) {
			if (ret != NULL)
				memcpy(ret, ht_value(ht, pos), ht->valuesize);
			return TRUE;
		}

		usize i = pos;
		usize slot = ((i + i * i) >> 1) & (ht->slots - 1);

		while (1) {
			if (*ht_occupied(ht, slot) == FALSE)
				return FALSE;

			if (ht->cmp_func(key, ht_key(ht, slot)) == 0) {
				if (ret != NULL)
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

bool ht_remove_all(HashTable *ht, usize padding, void *ret, usize *len)
{
	return_val_if_fail(ht != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (ht->keysize + ht->valuesize > USIZE_MAX - padding) {
		msg_error("bucket size overflow: the padding is too big!");
		return FALSE;
	}

	u8 *data = ret;

	for (usize i = 0, j = 0; i < ht->slots; ++i) {
		if (*ht_occupied(ht, i)) {
			u8 *elem = data + (ht->keysize + padding + ht->valuesize) * j++;
			memcpy(elem, ht_key(ht, i), ht->keysize);

			elem += ht->keysize + padding;
			memcpy(elem, ht_value(ht, i), ht->valuesize);

			*ht_occupied(ht, i) = FALSE;
		}
	}

	if (len != NULL)
		*len = ht->used;

	ht->used = 0;

	return TRUE;
}

void ht_destroy(HashTable *ht, FreeFunc key_free_func, FreeFunc value_free_func)
{
	return_if_fail(ht != NULL);

	if (ht->buckets == NULL)
		return;

	if (key_free_func != NULL || value_free_func != NULL) {
		for (usize i = 0; i < ht->slots; ++i) {
			if (*ht_occupied(ht, i)) {
				if (key_free_func != NULL)
					key_free_func(ht_key(ht, i));
				if (value_free_func != NULL)
					value_free_func(ht_value(ht, i));
			}
		}
	}

	free(ht->buckets);

	ht->buckets = NULL;
	ht->slots = 0;
	ht->used = 0;
}

void ht_free(HashTable *ht, FreeFunc key_free_func, FreeFunc value_free_func)
{
	return_if_fail(ht != NULL);
	ht_destroy(ht, key_free_func, value_free_func);
	free(ht);
}
