#include "HashTableIter.h"

#include "Messages.h"

#include <memory.h>

#define htb_bucket(ht, buckets, i) ((void *) (((char *) buckets) + (i) * (ht)->bucketsize))
#define htb_key(ht, buckets, i) (htb_bucket(ht, buckets, i))
#define htb_value(ht, buckets, i) ((void *) (((char *) htb_bucket(ht, buckets, i)) + (ht)->keysize))
#define htb_occupied(ht, buckets, i) ((bool *) (((char *) htb_bucket(ht, buckets, i)) + (ht)->keysize + (ht)->valuesize))

#define ht_key(ht, i) (htb_key(ht, (ht)->buckets, i))
#define ht_value(ht, i) (htb_value(ht, (ht)->buckets, i))
#define ht_occupied(ht, i) (htb_occupied(ht, (ht)->buckets, i))

enum {
	NONE,
	STARTED,
	FINISHED
};

TrbHashTableIter *trb_hash_table_iter_init(TrbHashTableIter *self, TrbHashTable *ht)
{
	trb_return_val_if_fail(ht != NULL, NULL);

	if (self == NULL) {
		self = trb_talloc(TrbHashTableIter, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the hash table iterator!");
			return NULL;
		}
	}

	self->ht = ht;
	self->slot = -1;
	self->status = NONE;
}

bool trb_hash_table_iter_next(TrbHashTableIter *self, const void **key, void **value)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->status == FINISHED) {
		trb_msg_warn("end of the hash table already has been reached!");
		return FALSE;
	} else {
		self->status = STARTED;
	}

	for (usize i = self->slot + 1; i < self->ht->slots; ++i) {
		if (*ht_occupied(self->ht, i)) {
			if (key != NULL)
				*key = ht_key(self->ht, i);

			if (value != NULL)
				*value = ht_value(self->ht, i);

			self->slot = i;

			return TRUE;
		}
	}

	self->status = FINISHED;

	return FALSE;
}

bool trb_hash_table_iter_replace(TrbHashTableIter *self, const void *value)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	switch (self->status) {
	case NONE:
		trb_msg_warn("iteration hasn't even started yet!");
		return FALSE;
	case FINISHED:
		trb_msg_warn("end of the hash table already has been reached!");
		return FALSE;
	default:
		break;
	}

	if (value == NULL)
		memset(ht_value(self->ht, self->slot), 0, self->ht->valuesize);
	else
		memcpy(ht_value(self->ht, self->slot), value, self->ht->valuesize);

	return TRUE;
}

bool trb_hash_table_iter_get(TrbHashTableIter *self, void *key, void *value)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(key != NULL, FALSE);
	trb_return_val_if_fail(value != NULL, FALSE);

	switch (self->status) {
	case NONE:
		trb_msg_warn("iteration hasn't even started yet!");
		return FALSE;
	case FINISHED:
		trb_msg_warn("end of the hash table already has been reached!");
		return FALSE;
	default:
		break;
	}

	if (key != NULL)
		memcpy(key, ht_key(self->ht, self->slot), self->ht->keysize);

	if (value != NULL)
		memcpy(value, ht_value(self->ht, self->slot), self->ht->valuesize);

	return TRUE;
}

bool trb_hash_table_iter_remove(TrbHashTableIter *self, void *key, void *value)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	switch (self->status) {
	case NONE:
		trb_msg_warn("iteration hasn't even started yet!");
		return FALSE;
	case FINISHED:
		trb_msg_warn("end of the hash table already has been reached!");
		return FALSE;
	default:
		break;
	}

	if (key != NULL)
		memcpy(key, ht_key(self->ht, self->slot), self->ht->keysize);

	if (value != NULL)
		memcpy(value, ht_value(self->ht, self->slot), self->ht->valuesize);

	*ht_occupied(self->ht, self->slot) = FALSE;

	return TRUE;
}
