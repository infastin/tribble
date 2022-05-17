#include "HashTableIter.h"

#include "Messages.h"

#include <memory.h>

#define htb_bucket(ht, buckets, i) ((void *) (((char *) buckets) + (i) * (ht)->bucketsize))
#define htb_key(ht, buckets, i) (htb_bucket(ht, buckets, i))
#define htb_value(ht, buckets, i) ((void *) (((char *) htb_bucket(ht, buckets, i)) + (ht)->keysize))
#define htb_occupied(ht, buckets, i) ((bool *) (((char *) htb_bucket(ht, buckets, i)) + (ht)->keysize + (ht)->valuesize))

#define ht_bucket(ht, i) (htb_bucket(ht, (ht)->buckets, i))
#define ht_key(ht, i) (htb_key(ht, (ht)->buckets, i))
#define ht_value(ht, i) (htb_value(ht, (ht)->buckets, i))
#define ht_occupied(ht, i) (htb_occupied(ht, (ht)->buckets, i))

enum {
	NONE,
	STARTED,
	FINISHED
};

HashTableIter *ht_iter_init(HashTableIter *hti, HashTable *ht)
{
	return_val_if_fail(ht != NULL, NULL);

	if (hti == NULL) {
		hti = talloc(HashTableIter, 1);

		if (hti == NULL) {
			msg_error("couldn't allocate memory for the hash table iterator!");
			return NULL;
		}
	}

	hti->ht = ht;
	hti->slot = -1;
	hti->status = NONE;

	return hti;
}

bool ht_iter_next(HashTableIter *hti, const void **key, void **value)
{
	return_val_if_fail(hti != NULL, FALSE);

	if (hti->status == FINISHED) {
		msg_warn("end of the hash table already has been reached!");
		return FALSE;
	} else {
		hti->status = STARTED;
	}

	for (usize i = hti->slot + 1; i < hti->ht->slots; ++i) {
		if (*ht_occupied(hti->ht, i)) {
			if (key != NULL)
				*key = ht_key(hti->ht, i);

			if (value != NULL)
				*value = ht_value(hti->ht, i);

			hti->slot = i;

			return TRUE;
		}
	}

	hti->status = FINISHED;

	return FALSE;
}

bool ht_iter_replace(HashTableIter *hti, const void *value)
{
	return_val_if_fail(hti != NULL, FALSE);

	switch (hti->status) {
	case NONE:
		msg_warn("iteration hasn't even started yet!");
		return FALSE;
	case FINISHED:
		msg_warn("end of the hash table already has been reached!");
		return FALSE;
	default:
		break;
	}

	if (value == NULL)
		memset(ht_value(hti->ht, hti->slot), 0, hti->ht->valuesize);
	else
		memcpy(ht_value(hti->ht, hti->slot), value, hti->ht->valuesize);

	return TRUE;
}

bool ht_iter_get(HashTableIter *hti, void *key, void *value)
{
	return_val_if_fail(hti != NULL, FALSE);
	return_val_if_fail(key != NULL, FALSE);
	return_val_if_fail(value != NULL, FALSE);

	switch (hti->status) {
	case NONE:
		msg_warn("iteration hasn't even started yet!");
		return FALSE;
	case FINISHED:
		msg_warn("end of the hash table already has been reached!");
		return FALSE;
	default:
		break;
	}

	if (key != NULL)
		memcpy(key, ht_key(hti->ht, hti->slot), hti->ht->keysize);

	if (value != NULL)
		memcpy(value, ht_value(hti->ht, hti->slot), hti->ht->valuesize);

	return TRUE;
}

bool ht_iter_remove(HashTableIter *hti, void *key, void *value)
{
	return_val_if_fail(hti != NULL, FALSE);

	switch (hti->status) {
	case NONE:
		msg_warn("iteration hasn't even started yet!");
		return FALSE;
	case FINISHED:
		msg_warn("end of the hash table already has been reached!");
		return FALSE;
	default:
		break;
	}

	if (key != NULL)
		memcpy(key, ht_key(hti->ht, hti->slot), hti->ht->keysize);

	if (value != NULL)
		memcpy(value, ht_value(hti->ht, hti->slot), hti->ht->valuesize);

	*ht_occupied(hti->ht, hti->slot) = FALSE;

	return TRUE;
}
