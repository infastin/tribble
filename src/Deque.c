#include "Deque.h"

#include "Macros.h"
#include "Messages.h"

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

	bool was_allocated = FALSE;

	if (self == NULL) {
		self = trb_talloc(TrbDeque, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the deque!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	self->n_buckets = 0;
	self->n_unused = 0;
	self->elemsize = elemsize;
	self->bucketsize = (elemsize < 256) ? 2048 : elemsize * 8;
	self->bucketcap = self->bucketsize / elemsize;
	self->len = 0;

	if (trb_vector_init(&self->buckets, TRUE, FALSE, sizeof(TrbDequeBucket *)) == NULL) {
		if (was_allocated)
			free(self);

		trb_msg_error("couldn't allocate memory for the deque buckets!");
		return NULL;
	}

	if (trb_vector_init(&self->unused, TRUE, FALSE, sizeof(TrbDequeBucket *)) == NULL) {
		if (was_allocated)
			free(self);

		trb_msg_error("couldn't allocate memory for the deque buckets!");
		return NULL;
	}

	return self;
}

static TrbDequeBucket *__trb_deque_add_buckets_back(TrbDeque *self, usize n)
{
	TrbDequeBucket *buckets[n];
	usize n_buckets = 0;

	if (self->n_unused != 0) {
		usize rm_len = (n <= self->n_unused) ? self->n_unused : n;
		trb_vector_pop_back_many(&self->unused, rm_len, buckets);
		self->n_unused -= rm_len;
		n_buckets += rm_len;
	}

	if (n_buckets != n) {
		for (u32 i = n_buckets; i < n; ++i) {
			buckets[i] = __bucket_new(self->bucketsize);

			if (buckets[i] == NULL) {
				trb_vector_push_back_many(&self->unused, buckets, i);
				return NULL;
			}

			buckets[i]->offset = 0;
		}
	}

	trb_vector_push_back_many(&self->buckets, buckets, n);
	self->n_buckets += n;

	return buckets[n - 1];
}

static TrbDequeBucket *__trb_deque_add_buckets_front(TrbDeque *self, usize n)
{
	TrbDequeBucket *buckets[n];
	usize n_buckets = 0;

	if (self->n_unused != 0) {
		usize rm_len = (n <= self->n_unused) ? self->n_unused : n;
		trb_vector_pop_back_many(&self->unused, rm_len, buckets);
		self->n_unused -= rm_len;
		n_buckets += rm_len;
	}

	if (n_buckets != n) {
		for (u32 i = n_buckets; i < n; ++i) {
			buckets[i] = __bucket_new(self->bucketsize);

			if (buckets[i] == NULL) {
				trb_vector_push_back_many(&self->unused, buckets, i);
				self->n_buckets += i;
				return NULL;
			}

			buckets[i]->offset = 0;
		}
	}

	buckets[0]->offset = self->bucketcap;
	trb_vector_push_front_many(&self->buckets, buckets, n);
	self->n_buckets += n;

	return buckets[0];
}

static TrbDequeBucket *__trb_deque_get_buckets_back(TrbDeque *self, usize n)
{
	if (self->n_buckets == 0)
		return __trb_deque_add_buckets_back(self, n);

	TrbDequeBucket *last = trb_vector_get_unsafe(&self->buckets, TrbDequeBucket *, self->buckets.len - 1);
	if (last->offset + last->len == self->bucketcap)
		return __trb_deque_add_buckets_back(self, n);

	if (n == 1)
		return last;

	return __trb_deque_add_buckets_back(self, n - 1);
}

static TrbDequeBucket *__trb_deque_get_buckets_front(TrbDeque *self, usize n)
{
	if (self->n_buckets == 0)
		return __trb_deque_add_buckets_front(self, 1);

	TrbDequeBucket *first = trb_vector_get_unsafe(&self->buckets, TrbDequeBucket *, 0);

	if (first->offset == 0)
		return __trb_deque_add_buckets_front(self, n);

	if (n == 1)
		return first;

	return __trb_deque_add_buckets_front(self, n - 1);
}

static bool *__trb_deque_insert_many(TrbDeque *self, usize index, const void *data, usize len)
{
	usize offset = 0;

	if (self->n_buckets != 0) {
		TrbDequeBucket *first = trb_vector_get_unsafe(&self->buckets, TrbDequeBucket *, 0);
		offset = first->offset;
	}

	usize bucket_index = (index + offset) % self->bucketcap;
	usize elem_index = (index + offset) / self->bucketcap;

	if (bucket_index >= self->n_buckets) {
	}
}

bool trb_deque_push_back(TrbDeque *self, const void *data)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	TrbDequeBucket *bucket = __trb_deque_get_buckets_back(self, 1);
	if (bucket == NULL)
		return FALSE;

	if (bucket->offset + bucket->len == self->bucketcap) {
		memmove(
			bucket->data,
			bucket_cell(bucket, self->elemsize, bucket->offset),
			bucket->len * self->elemsize
		);

		bucket->offset = 0;
	}

	memcpy(
		bucket_cell(bucket, self->elemsize, bucket->offset + bucket->len),
		data,
		self->elemsize
	);

	bucket->len++;
	self->len++;

	return TRUE;
}

bool trb_deque_push_front(TrbDeque *self, const void *data)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	TrbDequeBucket *bucket = __trb_deque_get_buckets_front(self, 1);
	if (bucket == NULL)
		return FALSE;

	if (bucket->offset == 0) {
		memmove(
			bucket_cell(bucket, self->elemsize, self->bucketcap - bucket->len),
			bucket->data,
			bucket->len * self->elemsize
		);

		bucket->offset = self->bucketcap - bucket->len;
	}

	bucket->offset--;
	bucket->len++;
	self->len++;

	memcpy(bucket_cell(bucket, self->elemsize, bucket->offset), data, self->elemsize);

	return TRUE;
}

bool trb_deque_pop_back(TrbDeque *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->n_buckets == 0) {
		trb_msg_warn("deque is empty!");
		return FALSE;
	}

	TrbDequeBucket *bucket = trb_vector_get_unsafe(&self->buckets, TrbDequeBucket *, self->buckets.len - 1);

	if (ret != NULL) {
		memcpy(
			ret,
			bucket_cell(bucket, self->elemsize, bucket->offset + bucket->len - 1),
			self->elemsize
		);
	}

	bucket->len--;
	self->len--;

	if (bucket->len == 0) {
		trb_vector_push_back(&self->unused, &bucket);
		self->n_unused++;

		trb_vector_pop_back(&self->buckets, NULL);
		self->n_buckets--;
	}

	return TRUE;
}

bool trb_deque_pop_front(TrbDeque *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->n_buckets == 0) {
		trb_msg_warn("deque is empty!");
		return FALSE;
	}

	TrbDequeBucket *bucket = trb_vector_get_unsafe(&self->buckets, TrbDequeBucket *, 0);

	if (ret != NULL) {
		memcpy(
			ret,
			bucket_cell(bucket, self->elemsize, bucket->offset),
			self->elemsize
		);
	}

	bucket->offset++;
	bucket->len--;
	self->len--;

	if (bucket->len == 0) {
		trb_vector_push_back(&self->unused, &bucket);
		self->n_unused++;

		trb_vector_pop_front(&self->buckets, NULL);
		self->n_buckets--;
	}

	return TRUE;
}

void trb_deque_destroy(TrbDeque *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);

	for (usize i = 0; i < self->buckets.len; ++i) {
		TrbDequeBucket *bucket = trb_vector_get_unsafe(&self->buckets, TrbDequeBucket *, i);

		if (free_func != NULL) {
			for (usize j = bucket->offset; j < bucket->len; ++j) {
				free_func(bucket_cell(bucket, self->elemsize, j));
			}
		}

		free(bucket);
	}

	trb_vector_destroy(&self->buckets, NULL);
	trb_vector_destroy(&self->unused, NULL);

	self->n_unused = 0;
	self->n_buckets = 0;
	self->len = 0;
}

bool trb_deque_get(TrbDeque *self, usize index, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(ret != NULL, FALSE);

	if (index >= self->len) {
		trb_msg_warn("element at [%zu] is out of bounds!", index);
		return FALSE;
	}

	TrbDequeBucket *first = trb_vector_get_unsafe(&self->buckets, TrbDequeBucket *, 0);
	usize elem_index = (index + first->offset) % self->bucketcap;
	usize bucket_index = (index + first->offset) / self->bucketcap;
	TrbDequeBucket *bucket = trb_vector_get_unsafe(&self->buckets, TrbDequeBucket *, bucket_index);

	memcpy(ret, bucket_cell(bucket, self->elemsize, elem_index), self->elemsize);

	return TRUE;
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
