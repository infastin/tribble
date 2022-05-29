#include "Deque.h"

#include "Checked.h"
#include "Macros.h"
#include "Math.h"
#include "Messages.h"
#include "Utils.h"

#include <memory.h>

TrbDeque *trb_deque_init(TrbDeque *self, bool clear, usize elemsize)
{
	trb_return_val_if_fail(elemsize != 0, NULL);

	if (trb_chk_mul(elemsize, 8, NULL)) {
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
	self->bucketsize = (elemsize < 256) ? 2048 : elemsize * 8;
	self->bucketcap = self->bucketsize / elemsize;

	self->len = 0;
	self->offset = 0;
	self->clear = clear;
	self->sorted = FALSE;

	if (trb_vector_init(&self->buckets, TRUE, sizeof(void *)) == NULL) {
		if (was_allocated)
			free(self);

		trb_msg_error("couldn't allocate memory for deque buckets!");
		return NULL;
	}

	if (trb_vector_init(&self->unused, TRUE, sizeof(void *)) == NULL) {
		trb_vector_destroy(&self->buckets, NULL);

		if (was_allocated)
			free(self);

		trb_msg_error("couldn't allocate memory for unused deque buckets!");
		return NULL;
	}

	return self;
}

static bool __trb_deque_add_buckets_back(TrbDeque *self, usize n)
{
	void **buckets = malloc(n * sizeof(void *));
	if (buckets == NULL) {
		trb_msg_error("couldn't allocate temporary memory for deque buckets!");
		return FALSE;
	}

	usize n_buckets = 0;

	if (self->unused.len != 0) {
		usize rm_len = (n >= self->unused.len) ? self->unused.len : n;
		trb_vector_pop_back_many(&self->unused, rm_len, buckets);
		n_buckets += rm_len;
	}

	for (; n_buckets < n; ++n_buckets) {
		if (self->clear)
			buckets[n_buckets] = calloc(1, self->bucketsize);
		else
			buckets[n_buckets] = malloc(self->bucketsize);

		if (buckets[n_buckets] == NULL) {
			if (!trb_vector_push_back_many(&self->unused, buckets, n_buckets))
				goto on_error;

			free(buckets);
			return FALSE;
		}
	}

	if (!trb_vector_push_back_many(&self->buckets, buckets, n))
		goto on_error;

	free(buckets);

	return TRUE;

on_error:
	for (usize i = 0; i < n_buckets; ++i)
		free(buckets[i]);

	free(buckets);

	return FALSE;
}

static bool __trb_deque_add_buckets_front(TrbDeque *self, usize n)
{
	void **buckets = malloc(n * sizeof(void *));
	if (buckets == NULL) {
		trb_msg_error("couldn't allocate temporary memory for deque buckets!");
		return FALSE;
	}

	usize n_buckets = 0;

	if (self->unused.len != 0) {
		usize rm_len = (n >= self->unused.len) ? self->unused.len : n;
		trb_vector_pop_back_many(&self->unused, rm_len, buckets);
		n_buckets += rm_len;
	}

	for (; n_buckets < n; ++n_buckets) {
		if (self->clear)
			buckets[n_buckets] = calloc(1, self->bucketsize);
		else
			buckets[n_buckets] = malloc(self->bucketsize);

		if (buckets[n_buckets] == NULL) {
			if (!trb_vector_push_back_many(&self->unused, buckets, n_buckets))
				goto on_error;

			free(buckets);
			return FALSE;
		}
	}

	if (!trb_vector_push_front_many(&self->buckets, buckets, n))
		goto on_error;

	free(buckets);

	return TRUE;

on_error:
	for (usize i = 0; i < n_buckets; ++i)
		free(buckets[i]);

	free(buckets);

	return FALSE;
}

static bool __trb_deque_get_buckets_back(TrbDeque *self, usize len)
{
	if (self->buckets.len == 0) {
		usize n;

		if (trb_chk_add(len, self->bucketcap - 1, &n)) {
			trb_msg_error("deque capacity overflow!");
			return FALSE;
		}

		n /= self->bucketcap;

		return __trb_deque_add_buckets_back(self, n);
	}

	usize first_len = trb_min(self->len, self->bucketcap - self->offset);
	usize other_len = self->len - first_len;

	usize last_offset = (self->buckets.len == 1) ? self->offset : 0;
	usize last_len = (other_len == 0)
						 ? first_len
						 : (other_len - 1) % self->bucketcap + 1;

	if (trb_chk_add(last_offset + last_len, len, NULL)) {
		trb_msg_error("deque capacity overflow!");
		return FALSE;
	}

	if (last_offset + last_len + len > self->bucketcap) {
		usize free_space = self->bucketcap - (last_offset + last_len);
		usize n = ((len - free_space) + (self->bucketcap - 1)) / self->bucketcap;

		return __trb_deque_add_buckets_back(self, n);
	}

	return TRUE;
}

static bool __trb_deque_get_buckets_front(TrbDeque *self, usize len)
{
	if (self->buckets.len == 0) {
		usize n;

		if (trb_chk_add(len, self->bucketcap - 1, &n)) {
			trb_msg_error("deque capacity overflow!");
			return FALSE;
		}

		n /= self->bucketcap;

		return __trb_deque_add_buckets_front(self, n);
	}

	if (len > self->offset) {
		usize n = ((len - self->offset) + (self->bucketcap - 1)) / self->bucketcap;
		return __trb_deque_add_buckets_front(self, n);
	}

	return TRUE;
}

static void __trb_deque_memcpy(TrbDeque *self, usize index, const void *data, usize len)
{
	const char *cdata = data;
	usize remaining_len = len;

	usize bucket_index = index / self->bucketcap;
	usize elem_index = index % self->bucketcap;

	for (; remaining_len != 0; ++bucket_index) {
		void *bucket = trb_vector_get(&self->buckets, void *, bucket_index);
		usize copy_len = trb_min(remaining_len, self->bucketcap - elem_index);

		memcpy(
			trb_array_cell(bucket, self->elemsize, elem_index),
			cdata, copy_len * self->elemsize
		);

		remaining_len -= copy_len;
		cdata += copy_len * self->elemsize;
		elem_index = 0;
	}
}

static bool __trb_deque_memmove_higher(TrbDeque *self, usize dst_index, usize src_index, usize len)
{
	usize remaining_len = len;

	if (trb_chk_add(src_index, len, &src_index) || trb_chk_add(dst_index, len, &dst_index)) {
		trb_msg_error("deque index overflow!");
		return FALSE;
	}

	usize b1_index = src_index / self->bucketcap;
	usize e1_index = src_index % self->bucketcap;

	usize b2_index = dst_index / self->bucketcap;
	usize e2_index = dst_index % self->bucketcap;

	while (remaining_len != 0) {
		usize min_index = trb_min(e1_index, e2_index);
		usize move_len = trb_min(remaining_len, min_index + 1);

		if (e1_index < move_len)
			b1_index--;

		if (e2_index < move_len)
			b2_index--;

		e1_index = (e1_index + (self->bucketcap - move_len)) % self->bucketcap;
		e2_index = (e2_index + (self->bucketcap - move_len)) % self->bucketcap;

		void *b1 = trb_vector_get(&self->buckets, void *, b1_index);
		void *b2 = trb_vector_get(&self->buckets, void *, b2_index);

		memmove(
			trb_array_cell(b2, self->elemsize, e2_index),
			trb_array_cell(b1, self->elemsize, e1_index),
			move_len * self->elemsize
		);

		remaining_len -= move_len;
	}

	return TRUE;
}

static void __trb_deque_memmove_lower(TrbDeque *self, usize dst_index, usize src_index, usize len)
{
	usize remaining_len = len;

	usize b1_index = src_index / self->bucketcap;
	usize e1_index = src_index % self->bucketcap;

	usize b2_index = dst_index / self->bucketcap;
	usize e2_index = dst_index % self->bucketcap;

	while (remaining_len != 0) {
		void *b1 = trb_vector_get(&self->buckets, void *, b1_index);
		void *b2 = trb_vector_get(&self->buckets, void *, b2_index);

		usize max_index = trb_max(e1_index, e2_index);
		usize move_len = trb_min(remaining_len, self->bucketcap - max_index);

		memmove(
			trb_array_cell(b2, self->elemsize, e2_index),
			trb_array_cell(b1, self->elemsize, e1_index),
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

	if (trb_chk_add(self->len, len, NULL)) {
		trb_msg_error("deque length overflow!");
		return FALSE;
	}

	if (index >= self->len) {
		usize add_len;

		if (trb_chk_add(len, index - self->len, &add_len) || trb_chk_add(self->len, add_len, NULL)) {
			trb_msg_error("deque index/length overflow!");
			return FALSE;
		}

		if (__trb_deque_get_buckets_back(self, add_len) == FALSE)
			return FALSE;

		__trb_deque_memcpy(self, self->offset + index, data, len);
		self->len += add_len;

		return TRUE;
	}

	if (index > self->len / 2) {
		if (__trb_deque_get_buckets_back(self, len) == FALSE)
			return FALSE;

		usize dst_index, src_index;

		if (
			trb_chk_add(self->offset, index, &dst_index) ||
			trb_chk_add(dst_index, len, &dst_index) ||
			trb_chk_add(self->offset, index, &src_index)
		) {
			trb_msg_error("deque index overflow!");
			return FALSE;
		}

		if (__trb_deque_memmove_higher(self, dst_index, src_index, self->len - index) == FALSE)
			return FALSE;

		__trb_deque_memcpy(self, src_index, data, len);

		self->len += len;

		return TRUE;
	}

	if (index == 0) {
		if (__trb_deque_get_buckets_front(self, len) == FALSE)
			return FALSE;

		if (trb_chk_add(self->len, len, NULL)) {
			trb_msg_error("deque length overflow!");
			return FALSE;
		}

		usize offset = ((self->bucketcap + self->offset) - (len % self->bucketcap)) % self->bucketcap;

		__trb_deque_memcpy(self, offset, data, len);

		self->offset = offset;
		self->len += len;

		return TRUE;
	}

	if (__trb_deque_get_buckets_front(self, len) == FALSE)
		return FALSE;

	usize dst_index = (self->offset + self->bucketcap - (len % self->bucketcap)) % self->bucketcap;
	usize src_index;

	if (trb_chk_add(dst_index, len, &src_index)) {
		trb_msg_error("deque index overflow!");
		return FALSE;
	}

	__trb_deque_memmove_lower(self, dst_index, src_index, index);
	__trb_deque_memcpy(self, dst_index + index, data, len);

	self->offset = dst_index;
	self->len += len;

	return TRUE;
}

static bool __trb_deque_push_back_many(TrbDeque *self, const void *data, usize len)
{
	if (len == 0)
		return TRUE;

	if (trb_chk_add(self->len, len, NULL) || trb_chk_add(self->offset, self->len, NULL)) {
		trb_msg_error("deque length overflow!");
		return FALSE;
	}

	if (__trb_deque_get_buckets_back(self, len) == FALSE)
		return FALSE;

	__trb_deque_memcpy(self, self->offset + self->len, data, len);

	self->len += len;

	return TRUE;
}

static bool __trb_deque_push_front_many(TrbDeque *self, const void *data, usize len)
{
	if (len == 0)
		return TRUE;

	if (trb_chk_add(self->len, len, NULL)) {
		trb_msg_error("deque length overflow!");
		return FALSE;
	}

	if (__trb_deque_get_buckets_front(self, len) == FALSE)
		return FALSE;

	usize offset = ((self->bucketcap + self->offset) - (len % self->bucketcap)) % self->bucketcap;

	__trb_deque_memcpy(self, offset, data, len);

	self->offset = offset;
	self->len += len;

	return TRUE;
}

bool trb_deque_push_back_many(TrbDeque *self, const void *data, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_push_back_many(self, data, len);
}

bool trb_deque_push_back(TrbDeque *self, const void *data)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_push_back_many(self, data, 1);
}

bool trb_deque_push_front_many(TrbDeque *self, const void *data, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_push_front_many(self, data, len);
}

bool trb_deque_push_front(TrbDeque *self, const void *data)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_push_front_many(self, data, 1);
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

static void __trb_deque_memcpy_r(TrbDeque *self, usize index, usize len, void *ret)
{
	char *cret = ret;
	usize remaining_len = len;

	usize bucket_index = index / self->bucketcap;
	usize elem_index = index % self->bucketcap;

	for (; remaining_len != 0; ++bucket_index) {
		void *bucket = trb_vector_get(&self->buckets, void *, bucket_index);
		usize copy_len = trb_min(remaining_len, self->bucketcap - elem_index);

		memcpy(
			cret,
			trb_array_cell(bucket, self->elemsize, elem_index),
			copy_len * self->elemsize
		);

		remaining_len -= copy_len;
		cret += copy_len * self->elemsize;
		elem_index = 0;
	}
}

static void __trb_deque_remove_buckets_back(TrbDeque *self, usize len)
{
	usize n;
	if (self->len == len) {
		n = self->buckets.len;
	} else {
		usize first_len = trb_min(self->len, self->bucketcap - self->offset);
		usize other_len = self->len - first_len;

		usize last_len = (other_len == 0)
							 ? first_len
							 : (other_len - 1) % self->bucketcap + 1;

		if (len >= last_len)
			n = (len - last_len) / self->bucketcap + 1;
		else
			n = 0;
	}

	if (n == 0)
		return;

	if (trb_vector_push_back_many(&self->unused, NULL, n)) {
		void **src_ptr = trb_vector_ptr(&self->buckets, void *, self->buckets.len - n);
		void **dst_ptr = trb_vector_ptr(&self->unused, void *, self->unused.len - n);
		memcpy(dst_ptr, src_ptr, n * sizeof(void *));
	}

	trb_vector_pop_back_many(&self->buckets, n, NULL);
}

static bool __trb_deque_pop_back_many(TrbDeque *self, usize len, void *ret)
{
	if (len == 0)
		return TRUE;

	if (self->len < len) {
		trb_msg_warn("deque doesn't hold %zu elements!", len);
		return FALSE;
	}

	if (ret != NULL) {
		usize src_index = self->offset + (self->len - len);
		__trb_deque_memcpy_r(self, src_index, len, ret);
	}

	__trb_deque_remove_buckets_back(self, len);

	self->len -= len;

	if (self->len == 0)
		self->offset = 0;

	return TRUE;
}

static void __trb_deque_remove_buckets_front(TrbDeque *self, usize len)
{
	usize n;
	if (self->len == len) {
		n = self->buckets.len;
	} else {
		usize first_len = trb_min(self->len, self->bucketcap - self->offset);

		if (len >= first_len)
			n = (len - first_len) / self->bucketcap + 1;
		else
			n = 0;
	}

	if (n == 0)
		return;

	if (trb_vector_push_back_many(&self->unused, NULL, n)) {
		void **src_ptr = trb_vector_ptr(&self->buckets, void *, 0);
		void **dst_ptr = trb_vector_ptr(&self->unused, void *, self->unused.len - n);
		memcpy(dst_ptr, src_ptr, n * sizeof(void *));
	}

	trb_vector_pop_front_many(&self->buckets, n, NULL);
}

static bool __trb_deque_pop_front_many(TrbDeque *self, usize len, void *ret)
{
	if (len == 0)
		return TRUE;

	if (self->len < len) {
		trb_msg_warn("deque doesn't hold %zu elements!", len);
		return FALSE;
	}

	if (ret != NULL) {
		usize src_index = self->offset;
		__trb_deque_memcpy_r(self, src_index, len, ret);
	}

	__trb_deque_remove_buckets_front(self, len);

	self->len -= len;
	self->offset = (self->offset + len) % self->bucketcap;

	if (self->len == 0)
		self->offset = 0;

	return TRUE;
}

static bool __trb_deque_remove_range(TrbDeque *self, usize index, usize len, void *ret)
{
	if (len == 0)
		return TRUE;

	if (trb_chk_add(index, len, NULL)) {
		trb_msg_error("deque index overflow!");
		return FALSE;
	}

	if (index + len > self->len) {
		if (len == 1) {
			trb_msg_warn("element at [%zu] is out of bounds!", index);
		} else {
			trb_msg_warn("range [%zu:%zu] is out of bounds!", index, index + len - 1);
		}

		return FALSE;
	}

	if (index + len == self->len) {
		return __trb_deque_pop_back_many(self, len, ret);
	}

	if (index + len > self->len / 2) {
		usize dst_index = self->offset + index;
		usize src_index = self->offset + index + len;
		usize move = self->len - index - len;

		if (ret != NULL)
			__trb_deque_memcpy_r(self, dst_index, len, ret);

		__trb_deque_memmove_lower(self, dst_index, src_index, move);
		__trb_deque_remove_buckets_back(self, len);

		self->len -= len;

		return TRUE;
	}

	if (index == 0) {
		return __trb_deque_pop_front_many(self, len, ret);
	}

	usize min_move = trb_min(len, index);

	usize dst_index = self->offset + index + len - min_move;
	usize src_index = (self->offset + index + self->bucketcap - (min_move % self->bucketcap)) % self->bucketcap;

	if (ret != NULL)
		__trb_deque_memcpy_r(self, self->offset + index, len, ret);

	if (__trb_deque_memmove_higher(self, dst_index, src_index, min_move) == FALSE)
		return FALSE;

	__trb_deque_remove_buckets_front(self, len);

	self->len -= len;
	self->offset = dst_index;

	return TRUE;
}

bool trb_deque_remove(TrbDeque *self, usize index, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len == 0) {
		trb_msg_warn("deque is empty!");
		return FALSE;
	}

	return __trb_deque_remove_range(self, index, 1, ret);
}

bool trb_deque_pop_back(TrbDeque *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_pop_back_many(self, 1, ret);
}

bool trb_deque_pop_front(TrbDeque *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_pop_front_many(self, 1, ret);
}

bool trb_deque_pop_back_many(TrbDeque *self, usize len, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_pop_back_many(self, len, ret);
}

bool trb_deque_pop_front_many(TrbDeque *self, usize len, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_deque_pop_front_many(self, len, ret);
}

bool trb_deque_remove_range(TrbDeque *self, usize index, usize len, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len < len) {
		trb_msg_warn("deque doesn't hold %zu elements!", len);
		return FALSE;
	}

	return __trb_deque_remove_range(self, index, len, ret);
}

bool trb_deque_remove_all(TrbDeque *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len == 0) {
		trb_msg_warn("array is empty!");
		return FALSE;
	}

	return __trb_deque_remove_range(self, 0, self->len, ret);
}

bool trb_deque_search(const TrbDeque *self, const void *target, TrbCmpFunc cmp_func, usize *index)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(cmp_func != NULL, FALSE);

	if (self->len == 0)
		return FALSE;

	usize first_len = trb_min(self->len, self->bucketcap - self->offset);
	usize other_len = self->len - first_len;

	for (usize bi = 0, i = 0; bi < self->buckets.len; ++bi) {
		void *bucket = trb_vector_get(&self->buckets, void *, bi);

		usize offset = (bi == 0) ? self->offset : 0;
		usize len = (bi == 0)
						? first_len
					: (bi == self->buckets.len - 1)
						? (other_len - 1) % self->bucketcap + 1
						: self->bucketcap;

		for (usize j = 0; j < len; ++j, ++i) {
			if (cmp_func(trb_array_cell(bucket, self->elemsize, j + offset), target) == 0) {
				if (index != NULL)
					*index = i;
				return TRUE;
			}
		}
	}

	return FALSE;
}

bool trb_deque_search_data(const TrbDeque *self, const void *target, TrbCmpDataFunc cmpd_func, void *data, usize *index)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(cmpd_func != NULL, FALSE);

	if (self->len == 0)
		return FALSE;

	usize first_len = trb_min(self->len, self->bucketcap - self->offset);
	usize other_len = self->len - first_len;

	for (usize bi = 0, i = 0; bi < self->buckets.len; ++bi) {
		void *bucket = trb_vector_get(&self->buckets, void *, bi);

		usize offset = (bi == 0) ? self->offset : 0;
		usize len = (bi == 0)
						? first_len
					: (bi == self->buckets.len - 1)
						? (other_len - 1) % self->bucketcap + 1
						: self->bucketcap;

		for (usize j = 0; j < len; ++j, ++i) {
			if (cmpd_func(trb_array_cell(bucket, self->elemsize, j + offset), target, data) == 0) {
				if (index != NULL)
					*index = i;
				return TRUE;
			}
		}
	}

	return FALSE;
}

static void *__trb_deque_slice_at(const TrbSlice *self, usize index)
{
	TrbDeque *deque = self->data;

	usize bi, ei;
	usize len = trb_slice_len(self);

	if (index >= len) {
		bi = (deque->offset + self->end) / deque->bucketcap;
		ei = (deque->offset + self->end) % deque->bucketcap;
	} else {
		bi = (deque->offset + self->start + index) / deque->bucketcap;
		ei = (deque->offset + self->start + index) % deque->bucketcap;
	}

	void *bucket = trb_vector_get(&deque->buckets, void *, bi);

	return trb_array_cell(bucket, self->elemsize, ei);
}

TrbSlice *trb_deque_slice(TrbDeque *self, TrbSlice *slice, usize start, usize end)
{
	trb_return_val_if_fail(self != NULL, NULL);
	trb_return_val_if_fail(start < end, NULL);

	if (end > self->len) {
		trb_msg_warn("interval [%zu:%zu) is out of bounds!", start, end);
		return NULL;
	}

	if (slice == NULL) {
		slice = trb_talloc(TrbSlice, 1);

		if (slice == NULL) {
			trb_msg_error("couldn't allocate memory for the slice!");
			return NULL;
		}
	}

	slice->at = __trb_deque_slice_at;
	slice->data = self;
	slice->start = start;
	slice->end = end;
	slice->elemsize = self->elemsize;

	return slice;
}

static inline void __trb_deque_free_bucket(void **bucket)
{
	if (bucket == NULL)
		return;

	free(*bucket);
}

void trb_deque_destroy(TrbDeque *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);

	usize first_len = trb_min(self->len, self->bucketcap - self->offset);
	usize other_len = self->len - first_len;

	for (usize bi = 0; bi < self->buckets.len; ++bi) {
		void *bucket = trb_vector_get(&self->buckets, void *, bi);

		if (free_func != NULL) {
			usize offset = (bi == 0) ? self->offset : 0;
			usize len = (bi == 0)
							? first_len
						: (bi == self->buckets.len - 1)
							? (other_len - 1) % self->bucketcap + 1
							: self->bucketcap;

			for (usize i = 0; i < len; ++i) {
				free_func(trb_array_cell(bucket, self->elemsize, i + offset));
			}
		}

		free(bucket);
	}

	trb_vector_destroy(&self->buckets, NULL);
	trb_vector_destroy(&self->unused, (TrbFreeFunc) __trb_deque_free_bucket);

	self->len = 0;
	self->offset = 0;
}

void trb_deque_shrink(TrbDeque *self)
{
	trb_return_if_fail(self != NULL);

	for (usize i = 0; i < self->unused.len; ++i) {
		void *bucket = trb_vector_get(&self->buckets, void *, i);
		free(bucket);
	}

	self->unused.len = 0;

	trb_vector_shrink(&self->unused);
	trb_vector_shrink(&self->buckets);
}

void trb_deque_free(TrbDeque *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);
	trb_deque_destroy(self, free_func);
	free(self);
}
