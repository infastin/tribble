#include "Deque.h"

#include "Macros.h"
#include "Math.h"
#include "Messages.h"
#include "Utils.h"

#include <memory.h>

#define bucket_cell(b, e, i) (&((b)->data[(i) * (e)]))

static TrbDequeBucket *__bucket_new(usize bucketsize)
{
	TrbDequeBucket *bucket = malloc(sizeof(TrbDequeBucket) + bucketsize);

	if (bucket == NULL) {
		trb_msg_error("couldn't allocate memory for the deque bucket!");
		return NULL;
	}

	bucket->len = 0;

	return bucket;
}

TrbDeque *trb_deque_init(TrbDeque *self, usize elemsize)
{
	trb_return_val_if_fail(elemsize != 0, NULL);

	if (elemsize > USIZE_MAX / 8) {
		trb_msg_error("bucket size overflow!");
		return NULL;
	}

	bool was_allocated = FALSE;

	if (self == NULL) {
		self = trb_talloc(TrbDeque, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the deque!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	self->elemsize = elemsize;
	/* self->bucketsize = (elemsize < 256) ? 2048 : elemsize * 8; */
	self->bucketsize = elemsize * 8;
	self->bucketcap = self->bucketsize / elemsize;
	self->len = 0;

	if (trb_vector_init(&self->buckets, TRUE, FALSE, sizeof(TrbDequeBucket *)) == NULL) {
		if (was_allocated)
			free(self);

		trb_msg_error("couldn't allocate memory for deque buckets!");
		return NULL;
	}

	if (trb_vector_init(&self->unused, TRUE, FALSE, sizeof(TrbDequeBucket *)) == NULL) {
		trb_vector_destroy(&self->buckets, NULL);

		if (was_allocated)
			free(self);

		trb_msg_error("couldn't allocate memory for unused deque buckets!");
		return NULL;
	}

	return self;
}

static usize __trb_deque_add_buckets_back(TrbDeque *self, usize n)
{
	TrbDequeBucket **buckets = malloc(n * sizeof(TrbDequeBucket *));
	if (buckets == NULL) {
		trb_msg_error("couldn't allocate temporary memory for deque buckets!");
		return -1;
	}

	usize n_buckets = 0;

	if (self->unused.len != 0) {
		usize rm_len = (n <= self->unused.len) ? self->unused.len : n;
		trb_vector_pop_back_many(&self->unused, rm_len, buckets);
		n_buckets += rm_len;
	}

	for (; n_buckets < n; ++n_buckets) {
		buckets[n_buckets] = __bucket_new(self->bucketsize);

		if (buckets[n_buckets] == NULL) {
			if (!trb_vector_push_back_many(&self->unused, buckets, n_buckets))
				goto on_error;

			free(buckets);
			return -1;
		}

		buckets[n_buckets]->offset = 0;
	}

	usize nb_index = self->buckets.len;

	if (!trb_vector_push_back_many(&self->buckets, buckets, n))
		goto on_error;

	free(buckets);

	return nb_index;

on_error:
	for (usize i = 0; i < n_buckets; ++i)
		free(buckets[i]);

	free(buckets);

	return -1;
}

static usize __trb_deque_add_buckets_front(TrbDeque *self, usize n)
{
	TrbDequeBucket **buckets = malloc(n * sizeof(TrbDequeBucket *));
	if (buckets == NULL) {
		trb_msg_error("couldn't allocate temporary memory for deque buckets!");
		return -1;
	}

	usize n_buckets = 0;

	if (self->unused.len != 0) {
		usize rm_len = (n <= self->unused.len) ? self->unused.len : n;
		trb_vector_pop_back_many(&self->unused, rm_len, buckets);
		n_buckets += rm_len;
	}

	for (; n_buckets < n; ++n_buckets) {
		buckets[n_buckets] = __bucket_new(self->bucketsize);

		if (buckets[n_buckets] == NULL) {
			if (!trb_vector_push_back_many(&self->unused, buckets, n_buckets))
				goto on_error;

			free(buckets);
			return -1;
		}

		buckets[n_buckets]->offset = 0;
	}

	buckets[0]->offset = self->bucketcap;

	if (!trb_vector_push_front_many(&self->buckets, buckets, n))
		goto on_error;

	free(buckets);

	return n;

on_error:
	for (usize i = 0; i < n_buckets; ++i)
		free(buckets[i]);

	free(buckets);

	return -1;
}

static usize __trb_deque_get_buckets_back(TrbDeque *self, usize len)
{
	if (self->buckets.len == 0) {
		usize n = (len + (self->bucketcap - 1)) / self->bucketcap;
		return __trb_deque_add_buckets_back(self, n);
	}

	TrbDequeBucket *last = trb_vector_get(&self->buckets, TrbDequeBucket *, self->buckets.len - 1);

	if (last->offset + last->len + len > self->bucketcap) {
		usize free_space = self->bucketcap - (last->offset + last->len);
		usize n = ((len - free_space) + (self->bucketcap - 1)) / self->bucketcap;
		return __trb_deque_add_buckets_back(self, n);
	}

	return self->buckets.len - 1;
}

static usize __trb_deque_get_buckets_front(TrbDeque *self, usize len)
{
	if (self->buckets.len == 0) {
		usize n = (len + (self->bucketcap - 1)) / self->bucketcap;
		return __trb_deque_add_buckets_front(self, n);
	}

	TrbDequeBucket *first = trb_vector_get(&self->buckets, TrbDequeBucket *, 0);

	if (len > first->offset) {
		usize n = ((len - first->offset) + (self->bucketcap - 1)) / self->bucketcap;
		return __trb_deque_add_buckets_front(self, n);
	}

	return 0;
}

static void __trb_deque_memcpy(TrbDeque *self, usize index, const void *data, usize len)
{
	const char *cdata = data;
	usize remaining_len = len;

	usize bucket_index = index / self->bucketcap;
	usize elem_index = index % self->bucketcap;

	for (; remaining_len != 0; ++bucket_index) {
		TrbDequeBucket *bucket = trb_vector_get(&self->buckets, TrbDequeBucket *, bucket_index);
		usize copy_len = trb_min(remaining_len, self->bucketcap - elem_index);

		memcpy(
			bucket_cell(bucket, self->elemsize, elem_index),
			cdata, copy_len * self->elemsize
		);

		if (elem_index == 0)
			bucket->offset = 0;

		remaining_len -= copy_len;
		bucket->len += copy_len;
		cdata += copy_len * self->elemsize;
		elem_index = 0;
	}
}

static void __trb_deque_memmove_higher(TrbDeque *self, usize dst_index, usize src_index, usize len)
{
	usize remaining_len = len;

	usize b1_index = (src_index + len) / self->bucketcap;
	usize e1_index = (src_index + len) % self->bucketcap;

	usize b2_index = (dst_index + len) / self->bucketcap;
	usize e2_index = (dst_index + len) % self->bucketcap;

	while (remaining_len != 0) {
		usize min_index = trb_min(e1_index, e2_index);
		usize move_len = trb_min(remaining_len, min_index + 1);

		if (e1_index < move_len)
			b1_index--;

		if (e2_index < move_len)
			b2_index--;

		e1_index = (e1_index + (self->bucketcap - move_len)) % self->bucketcap;
		e2_index = (e2_index + (self->bucketcap - move_len)) % self->bucketcap;

		TrbDequeBucket *b1 = trb_vector_get(&self->buckets, TrbDequeBucket *, b1_index);
		TrbDequeBucket *b2 = trb_vector_get(&self->buckets, TrbDequeBucket *, b2_index);

		memmove(
			bucket_cell(b2, self->elemsize, e2_index),
			bucket_cell(b1, self->elemsize, e1_index),
			move_len * self->elemsize
		);

		remaining_len -= move_len;
	}
}

static void __trb_deque_memmove_lower(TrbDeque *self, usize dst_index, usize src_index, usize len)
{
	usize remaining_len = len;

	usize b1_index = src_index / self->bucketcap;
	usize e1_index = src_index % self->bucketcap;

	usize b2_index = dst_index / self->bucketcap;
	usize e2_index = dst_index % self->bucketcap;

	while (remaining_len != 0) {
		TrbDequeBucket *b1 = trb_vector_get(&self->buckets, TrbDequeBucket *, b1_index);
		TrbDequeBucket *b2 = trb_vector_get(&self->buckets, TrbDequeBucket *, b2_index);

		usize max_index = trb_max(e1_index, e2_index);
		usize move_len = trb_min(remaining_len, self->bucketcap - max_index);

		memmove(
			bucket_cell(b2, self->elemsize, e2_index),
			bucket_cell(b1, self->elemsize, e1_index),
			move_len * self->elemsize
		);

		remaining_len -= move_len;
		e1_index = (e1_index + move_len) % self->bucketcap;
		e2_index = (e2_index + move_len) % self->bucketcap;

		if (e1_index == 0)
			b1_index++;

		if (e2_index == 0)
			b2_index++;
	}
}

static bool __trb_deque_insert_many(TrbDeque *self, usize index, const void *data, usize len)
{
	if (len == 0)
		return TRUE;

	if (index >= self->len) {
		usize add_len = len + (index - self->len);

		usize nb_index = __trb_deque_get_buckets_back(self, add_len);
		if (nb_index == -1)
			return FALSE;

		TrbDequeBucket *new_bucket = trb_vector_get(&self->buckets, TrbDequeBucket *, nb_index);
		TrbDequeBucket *first = trb_vector_get(&self->buckets, TrbDequeBucket *, 0);

		__trb_deque_memcpy(self, first->offset + index, data, len);

		self->len += add_len;
		new_bucket->len += (first->offset + index) % self->bucketcap;

		return TRUE;
	}

	if (index > self->len / 2) {
		usize kek = __trb_deque_get_buckets_back(self, len);

		if (kek == -1)
			return FALSE;

		TrbDequeBucket *first = trb_vector_get(&self->buckets, TrbDequeBucket *, 0);

		usize dst_index = first->offset + index + len;
		usize src_index = first->offset + index;

		__trb_deque_memmove_higher(self, dst_index, src_index, self->len - index);
		__trb_deque_memcpy(self, src_index, data, len);

		self->len += len;

		return TRUE;
	}

	if (index == 0) {
		usize ob_index = __trb_deque_get_buckets_front(self, len);
		if (ob_index == -1)
			return FALSE;

		TrbDequeBucket *old_bucket = trb_vector_get(&self->buckets, TrbDequeBucket *, ob_index);
		usize offset = ((self->bucketcap + old_bucket->offset) - (len % self->bucketcap)) % self->bucketcap;

		__trb_deque_memcpy(self, offset, data, len);

		TrbDequeBucket *first = trb_vector_get(&self->buckets, TrbDequeBucket *, 0);
		first->offset = offset;
		self->len += len;

		return TRUE;
	}

	usize ob_index = __trb_deque_get_buckets_front(self, len);
	if (ob_index == -1)
		return FALSE;

	TrbDequeBucket *old_bucket = trb_vector_get(&self->buckets, TrbDequeBucket *, ob_index);

	usize dst_index = (old_bucket->offset + self->bucketcap - (len % self->bucketcap)) % self->bucketcap;
	usize src_index = dst_index + len;

	__trb_deque_memmove_lower(self, dst_index, src_index, index);
	__trb_deque_memcpy(self, dst_index + index, data, len);

	TrbDequeBucket *first = trb_vector_get(&self->buckets, TrbDequeBucket *, 0);
	first->offset = dst_index;

	self->len += len;

	return TRUE;
}

bool trb_deque_push_back_many(TrbDeque *self, const void *data, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_insert_many(self, self->len, data, len);
}

bool trb_deque_push_back(TrbDeque *self, const void *data)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_insert_many(self, self->len, data, 1);
}

bool trb_deque_push_front_many(TrbDeque *self, const void *data, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_insert_many(self, 0, data, len);
}

bool trb_deque_push_front(TrbDeque *self, const void *data)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_insert_many(self, 0, data, 1);
}

bool trb_deque_insert(TrbDeque *self, usize index, const void *data)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_insert_many(self, index, data, 1);
}

bool trb_deque_insert_many(TrbDeque *self, usize index, const void *data, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_insert_many(self, index, data, len);
}

bool trb_deque_pop_back(TrbDeque *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->buckets.len == 0) {
		trb_msg_warn("deque is empty!");
		return FALSE;
	}

	TrbDequeBucket *last = trb_vector_get(&self->buckets, TrbDequeBucket *, self->buckets.len - 1);

	if (ret != NULL) {
		memcpy(
			ret,
			bucket_cell(last, self->elemsize, last->offset + last->len - 1),
			self->elemsize
		);
	}

	last->len--;
	self->len--;

	if (last->len == 0) {
		if (!trb_vector_push_back(&self->unused, &last))
			free(last);
		else
			trb_vector_pop_back(&self->buckets, NULL);
	}

	return TRUE;
}

bool trb_deque_pop_front(TrbDeque *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->buckets.len == 0) {
		trb_msg_warn("deque is empty!");
		return FALSE;
	}

	TrbDequeBucket *first = trb_vector_get(&self->buckets, TrbDequeBucket *, 0);

	if (ret != NULL) {
		memcpy(
			ret,
			bucket_cell(first, self->elemsize, first->offset),
			self->elemsize
		);
	}

	first->offset++;
	first->len--;
	self->len--;

	if (first->len == 0) {
		if (!trb_vector_push_back(&self->unused, &first))
			free(first);
		else
			trb_vector_pop_back(&self->buckets, NULL);
	}

	return TRUE;
}

void trb_deque_destroy(TrbDeque *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);

	for (usize i = 0; i < self->buckets.len; ++i) {
		TrbDequeBucket *bucket = trb_vector_get(&self->buckets, TrbDequeBucket *, i);

		if (free_func != NULL) {
			for (usize j = bucket->offset; j < bucket->len; ++j) {
				free_func(bucket_cell(bucket, self->elemsize, j));
			}
		}

		free(bucket);
	}

	trb_vector_destroy(&self->buckets, NULL);
	trb_vector_destroy(&self->unused, NULL);

	self->len = 0;
}

void trb_deque_shrink(TrbDeque *self)
{
	trb_return_if_fail(self != NULL);
	trb_vector_destroy(&self->unused, free);
	trb_vector_shrink(&self->buckets);
}

void trb_deque_free(TrbDeque *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);
	trb_deque_destroy(self, free_func);
	free(self);
}
