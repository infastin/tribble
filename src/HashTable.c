#include "HashTable.h"

#include "Messages.h"

#include <memory.h>

#define HT_INIT_SLOTS 16
#define HT_INIT_SLOTS_POW 4

#define htb_bucket(ht, buckets, i) ((void *) (((char *) buckets) + (i) * (ht)->bucketsize))
#define htb_key(ht, buckets, i) (htb_bucket(ht, buckets, i))
#define htb_value(ht, buckets, i) ((void *) (((char *) htb_bucket(ht, buckets, i)) + (ht)->keysize))
#define htb_occupied(ht, buckets, i) ((bool *) (((char *) htb_bucket(ht, buckets, i)) + (ht)->keysize + (ht)->valuesize))

#define ht_key(ht, i) (htb_key(ht, (ht)->buckets, i))
#define ht_value(ht, i) (htb_value(ht, (ht)->buckets, i))
#define ht_occupied(ht, i) (htb_occupied(ht, (ht)->buckets, i))

TrbHashTable *trb_hash_table_init(
	TrbHashTable *self,
	usize keysize,
	usize valuesize,
	usize seed,
	TrbHashFunc hash_func,
	TrbCmpFunc cmp_func
)
{
	trb_return_val_if_fail(hash_func != NULL, NULL);
	trb_return_val_if_fail(cmp_func != NULL, NULL);
	trb_return_val_if_fail(keysize != 0, NULL);
	trb_return_val_if_fail(valuesize != 0, NULL);

	if (keysize > USIZE_MAX - valuesize || keysize > USIZE_MAX - valuesize - 1) {
		trb_msg_error("bucket size overflow!");
		return FALSE;
	}

	usize bucketsize = keysize + valuesize + 1;

	if (bucketsize > (USIZE_MAX >> HT_INIT_SLOTS_POW)) {
		trb_msg_error("hash table capacity overflow!");
		return FALSE;
	}

	bool was_allocated = FALSE;

	if (self == NULL) {
		self = trb_talloc(TrbHashTable, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the hash table!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	self->buckets = calloc(HT_INIT_SLOTS, bucketsize);

	if (self->buckets == NULL) {
		if (was_allocated)
			free(self);

		trb_msg_error("couldn't allocate memory for the hash table buckets!");
		return NULL;
	}

	self->slots = HT_INIT_SLOTS;
	self->used = 0;
	self->keysize = keysize;
	self->valuesize = valuesize;
	self->bucketsize = bucketsize;
	self->seed = seed;
	self->hash_func = hash_func;
	self->cmp_func = cmp_func;
	self->with_data = FALSE;
	self->data = NULL;

	return self;
}

TrbHashTable *trb_hash_table_init_data(
	TrbHashTable *self,
	usize keysize,
	usize valuesize,
	usize seed,
	TrbHashFunc hash_func,
	TrbCmpDataFunc cmpd_func,
	void *data
)
{
	trb_return_val_if_fail(hash_func != NULL, NULL);
	trb_return_val_if_fail(cmpd_func != NULL, NULL);
	trb_return_val_if_fail(keysize != 0, NULL);
	trb_return_val_if_fail(valuesize != 0, NULL);

	if (keysize > USIZE_MAX - valuesize || keysize > USIZE_MAX - valuesize - 1) {
		trb_msg_error("bucket size overflow!");
		return FALSE;
	}

	usize bucketsize = keysize + valuesize + 1;

	if (bucketsize > (USIZE_MAX >> HT_INIT_SLOTS_POW)) {
		trb_msg_error("hash table capacity overflow!");
		return FALSE;
	}

	bool was_allocated = FALSE;

	if (self == NULL) {
		self = trb_talloc(TrbHashTable, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the hash table!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	self->buckets = calloc(HT_INIT_SLOTS, bucketsize);

	if (self->buckets == NULL) {
		if (was_allocated)
			free(self);

		trb_msg_error("couldn't allocate memory for the hash table buckets!");
		return NULL;
	}

	self->slots = HT_INIT_SLOTS;
	self->used = 0;
	self->keysize = keysize;
	self->valuesize = valuesize;
	self->bucketsize = bucketsize;
	self->seed = seed;
	self->hash_func = hash_func;
	self->cmpd_func = cmpd_func;
	self->with_data = TRUE;
	self->data = data;

	return self;
}

static bool __trb_hash_table_add(TrbHashTable *self, usize slots, void *buckets, const void *key, const void *value)
{
	usize hash = self->hash_func(key, self->keysize, self->seed);
	usize pos = hash & (slots - 1);

	if (*htb_occupied(self, buckets, pos)) {
		if (self->with_data) {
			if (self->cmpd_func(key, htb_key(self, buckets, pos), self->data) == 0)
				return FALSE;
		} else if (self->cmp_func(key, htb_key(self, buckets, pos)) == 0) {
			return FALSE;
		}

		for (usize i = pos ?: 1;; ++i) {
			if (i > USIZE_MAX / i || i * i > USIZE_MAX - i || ((i + i * i) >> 1) > USIZE_MAX - pos) {
				trb_msg_error("quadratic probing overflow!");
				return FALSE;
			}

			usize slot = (pos + ((i + i * i) >> 1)) & (slots - 1);

			if (*htb_occupied(self, buckets, slot) == FALSE) {
				pos = slot;
				break;
			}

			if (self->with_data) {
				if (self->cmpd_func(key, htb_key(self, buckets, slot), self->data) == 0)
					return FALSE;
			} else if (self->cmp_func(key, htb_key(self, buckets, slot)) == 0) {
				return FALSE;
			}

			if (i >= slots)
				i = 0;
		}
	}

	memcpy(htb_key(self, buckets, pos), key, self->keysize);

	if (value != NULL)
		memcpy(htb_value(self, buckets, pos), value, self->valuesize);
	else
		memset(htb_value(self, buckets, pos), 0, self->valuesize);

	memset(htb_occupied(self, buckets, pos), 1, 1);

	return TRUE;
}

static bool trb_hash_table_resize(TrbHashTable *self, usize new_slots)
{
	if (self->bucketsize > USIZE_MAX / new_slots) {
		trb_msg_error("hash table capacity overflow!");
		return FALSE;
	}

	void *buckets = calloc(new_slots, self->bucketsize);
	if (buckets == NULL) {
		trb_msg_error("couldn't reallocate memory for the hash table buckets!");
		return FALSE;
	}

	for (usize i = 0; i < self->slots; ++i) {
		if (*ht_occupied(self, i)) {
			if (__trb_hash_table_add(self, new_slots, buckets, ht_key(self, i), ht_value(self, i)) == FALSE) {
				free(buckets);
				return FALSE;
			}
		}
	}

	free(self->buckets);

	self->buckets = buckets;
	self->slots = new_slots;

	return TRUE;
}

bool trb_hash_table_insert(TrbHashTable *self, const void *key, const void *value)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(key != NULL, FALSE);

	if (self->slots == 0) {
		if (!trb_hash_table_resize(self, HT_INIT_SLOTS))
			return FALSE;
	} else {
		f64 load_factor = (f64) self->used / (f64) self->slots;
		if (load_factor >= 0.6) {
			usize new_slots = self->slots << 1;
			if (self->slots > new_slots) {
				trb_msg_error("hash table capacity overflow!");
				return FALSE;
			}

			if (!trb_hash_table_resize(self, new_slots))
				return FALSE;
		}
	}

	bool only_change_value = FALSE;
	usize hash = self->hash_func(key, self->keysize, self->seed);
	usize pos = hash & (self->slots - 1);

	i32 cmp;
	if (self->with_data) {
		cmp = self->cmpd_func(key, ht_key(self, pos), self->data);
	} else {
		cmp = self->cmp_func(key, ht_key(self, pos));
	}

	if (*ht_occupied(self, pos) && cmp != 0) {
		for (usize i = pos ?: 1;; ++i) {
			if (i > USIZE_MAX / i || i * i > USIZE_MAX - i || ((i + i * i) >> 1) > USIZE_MAX - pos) {
				trb_msg_error("quadratic probing overflow!");
				return FALSE;
			}

			usize slot = (pos + ((i + i * i) >> 1)) & (self->slots - 1);

			if (*ht_occupied(self, slot) == FALSE) {
				pos = slot;
				break;
			}

			i32 cmp;
			if (self->with_data) {
				cmp = self->cmpd_func(key, ht_key(self, slot), self->data);
			} else {
				cmp = self->cmp_func(key, ht_key(self, slot));
			}

			if (cmp == 0) {
				only_change_value = TRUE;
				pos = slot;
				break;
			}

			if (i >= self->slots)
				i = 0;
		}
	}

	if (!only_change_value) {
		memcpy(ht_key(self, pos), key, self->keysize);
		self->used++;
	}

	if (value != NULL)
		memcpy(ht_value(self, pos), value, self->valuesize);
	else
		memset(ht_value(self, pos), 0, self->valuesize);

	memset(ht_occupied(self, pos), 1, 1);

	return TRUE;
}

bool trb_hash_table_add(TrbHashTable *self, const void *key, const void *value)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(key != NULL, FALSE);

	if (self->slots == 0) {
		if (!trb_hash_table_resize(self, HT_INIT_SLOTS))
			return FALSE;
	} else {
		f64 load_factor = (f64) self->used / (f64) self->slots;

		if (load_factor >= 0.6) {
			usize new_slots = self->slots << 1;
			if (self->slots > new_slots) {
				trb_msg_error("hash table capacity overflow!");
				return FALSE;
			}

			if (!trb_hash_table_resize(self, new_slots))
				return FALSE;
		}
	}

	if (__trb_hash_table_add(self, self->slots, self->buckets, key, value)) {
		self->used++;
		return TRUE;
	}

	return FALSE;
}

bool trb_hash_table_remove(TrbHashTable *self, const void *key, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(key != NULL, FALSE);

	if (self->slots == 0) {
		trb_msg_warn("hash table capacity is zero!");
		return FALSE;
	}

	if (self->slots > HT_INIT_SLOTS) {
		f64 load_factor = (f64) self->used / (f64) self->slots;
		if (load_factor <= 0.4) {
			if (!trb_hash_table_resize(self, self->slots >> 1))
				return FALSE;
		}
	}

	usize hash = self->hash_func(key, self->keysize, self->seed);
	usize pos = hash & (self->slots - 1);

	if (*ht_occupied(self, pos) == FALSE)
		return FALSE;

	i32 cmp;
	if (self->with_data) {
		cmp = self->cmpd_func(key, ht_key(self, pos), self->data);
	} else {
		cmp = self->cmp_func(key, ht_key(self, pos));
	}

	if (cmp != 0) {
		for (usize i = pos ?: 1;; ++i) {
			if (i > USIZE_MAX / i || i * i > USIZE_MAX - i || ((i + i * i) >> 1) > USIZE_MAX - pos) {
				trb_msg_error("quadratic probing overflow!");
				return FALSE;
			}

			usize slot = (pos + ((i + i * i) >> 1)) & (self->slots - 1);

			if (*ht_occupied(self, slot) == FALSE)
				return FALSE;

			i32 cmp;
			if (self->with_data) {
				cmp = self->cmpd_func(key, ht_key(self, slot), self->data);
			} else {
				cmp = self->cmp_func(key, ht_key(self, slot));
			}

			if (cmp == 0) {
				pos = slot;
				break;
			}

			if (i >= self->slots)
				i = 0;
		}
	}

	if (ret != NULL)
		memcpy(ret, ht_value(self, pos), self->valuesize);

	memset(ht_occupied(self, pos), 0, 1);

	self->used--;

	return TRUE;
}

bool trb_hash_table_lookup(const TrbHashTable *self, const void *key, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(key != NULL, FALSE);
	trb_return_val_if_fail(ret != NULL, FALSE);

	if (self->slots == 0) {
		trb_msg_warn("hash table capacity is zero!");
		return FALSE;
	}

	usize hash = self->hash_func(key, self->keysize, self->seed);
	usize pos = hash & (self->slots - 1);

	if (*ht_occupied(self, pos)) {
		i32 cmp;
		if (self->with_data) {
			cmp = self->cmpd_func(key, ht_key(self, pos), self->data);
		} else {
			cmp = self->cmp_func(key, ht_key(self, pos));
		}

		if (cmp == 0) {
			if (ret != NULL)
				memcpy(ret, ht_value(self, pos), self->valuesize);
			return TRUE;
		}

		for (usize i = pos ?: 1;; ++i) {
			if (i > USIZE_MAX / i || i * i > USIZE_MAX - i || ((i + i * i) >> 1) > USIZE_MAX - pos) {
				trb_msg_error("quadratic probing overflow!");
				return FALSE;
			}

			usize slot = (pos + ((i + i * i) >> 1)) & (self->slots - 1);

			if (*ht_occupied(self, slot) == FALSE)
				return FALSE;

			i32 cmp;
			if (self->with_data) {
				cmp = self->cmpd_func(key, ht_key(self, slot), self->data);
			} else {
				cmp = self->cmp_func(key, ht_key(self, slot));
			}

			if (cmp == 0) {
				if (ret != NULL)
					memcpy(ret, ht_value(self, slot), self->valuesize);
				return TRUE;
			}

			if (i >= self->slots)
				i = 0;
		}
	}

	return FALSE;
}

bool trb_hash_table_remove_all(TrbHashTable *self, usize padding, void *ret, usize *len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(ret != NULL, FALSE);

	if (self->keysize + self->valuesize > USIZE_MAX - padding) {
		trb_msg_error("bucket size overflow: the padding is too big!");
		return FALSE;
	}

	u8 *data = ret;

	for (usize i = 0, j = 0; i < self->slots; ++i) {
		if (*ht_occupied(self, i)) {
			u8 *elem = data + (self->keysize + padding + self->valuesize) * j++;
			memcpy(elem, ht_key(self, i), self->keysize);

			elem += self->keysize + padding;
			memcpy(elem, ht_value(self, i), self->valuesize);

			*ht_occupied(self, i) = FALSE;
		}
	}

	if (len != NULL)
		*len = self->used;

	self->used = 0;

	return TRUE;
}

void trb_hash_table_destroy(TrbHashTable *self, TrbFreeFunc key_free_func, TrbFreeFunc value_free_func)
{
	trb_return_if_fail(self != NULL);

	if (self->buckets == NULL)
		return;

	if (key_free_func != NULL || value_free_func != NULL) {
		for (usize i = 0; i < self->slots; ++i) {
			if (*ht_occupied(self, i)) {
				if (key_free_func != NULL)
					key_free_func(ht_key(self, i));
				if (value_free_func != NULL)
					value_free_func(ht_value(self, i));
			}
		}
	}

	free(self->buckets);

	self->buckets = NULL;
	self->slots = 0;
	self->used = 0;
}

void trb_hash_table_free(TrbHashTable *self, TrbFreeFunc key_free_func, TrbFreeFunc value_free_func)
{
	trb_return_if_fail(self != NULL);
	trb_hash_table_destroy(self, key_free_func, value_free_func);
	free(self);
}
