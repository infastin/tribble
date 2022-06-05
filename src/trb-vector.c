#include "trb-vector.h"

#include "trb-checked.h"
#include "trb-messages.h"
#include "trb-types.h"
#include "trb-utils.h"

#include <stdlib.h>
#include <string.h>

#define VECTOR_INIT_CAP 8
#define BINARY_SEARCH_LEN_THRESHOLD 64

#define trb_vector_cell(a, i) ((void *) &((char *) ((a)->data))[(i) * (a)->elemsize])

TrbVector *trb_vector_init(TrbVector *self, bool clear, usize elemsize)
{
	trb_return_val_if_fail(elemsize != 0, NULL);

	if (trb_chk_mul(elemsize, VECTOR_INIT_CAP, NULL)) {
		trb_msg_error("vector capacity overflow!");
		return FALSE;
	}

	bool was_allocated = FALSE;

	if (self == NULL) {
		self = trb_talloc(TrbVector, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the vector!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	if (clear)
		self->data = calloc(VECTOR_INIT_CAP, elemsize);
	else
		self->data = malloc(VECTOR_INIT_CAP * elemsize);

	if (self->data == NULL) {
		if (was_allocated)
			free(self);

		trb_msg_error("couldn't allocate memory for the vector buffer!");
		return NULL;
	}

	self->len = 0;
	self->offset = 0;
	self->capacity = VECTOR_INIT_CAP;
	self->elemsize = elemsize;
	self->clear = clear;

	return self;
}

static bool __trb_vector_newcap(TrbVector *self, usize newcap)
{
	usize new_allocated = (newcap >> 3) + (newcap < 9 ? 3 : 6);

	if (trb_chk_add(newcap, new_allocated, &newcap)) {
		trb_msg_error("vector capacity overflow!");
		return FALSE;
	}

	if (trb_chk_mul(newcap, self->elemsize, NULL)) {
		trb_msg_error("vector capacity overflow!");
		return FALSE;
	}

	void *data = realloc(self->data, newcap * self->elemsize);

	if (data == NULL) {
		trb_msg_error("couldn't reallocate memory for the vector buffer!");
		return FALSE;
	}

	self->data = data;

	if (self->clear) {
		memset(
			trb_vector_cell(self, self->capacity), 0,
			(newcap - self->capacity) * self->elemsize
		);
	}

	self->capacity = newcap;

	return TRUE;
}

static bool __trb_vector_insert_many(TrbVector *self, usize index, const void *data, usize len)
{
	if (len == 0)
		return TRUE;

	if (
		trb_chk_add(index, len, NULL) ||
		trb_chk_add(index + len, self->offset, NULL) ||
		trb_chk_add(self->len, len, NULL) ||
		trb_chk_add(self->len + len, self->offset, NULL)
	) {
		trb_msg_error("vector index/length overflow!");
		return FALSE;
	}

	if (index >= self->len && self->offset + index + len > self->capacity) {
		if (__trb_vector_newcap(self, self->offset + index + len) == FALSE)
			return FALSE;
	} else if (index < self->len && self->offset + self->len + len > self->capacity) {
		if (__trb_vector_newcap(self, self->offset + self->len + len) == FALSE)
			return FALSE;
	}

	if (self->offset >= self->len / 2) {
		memmove(
			trb_vector_cell(self, 0),
			trb_vector_cell(self, self->offset),
			self->len * self->elemsize
		);

		self->offset = 0;
	}

	if (index >= self->len) {
		self->len = index;
	} else if (index == 0) {
		if (self->offset >= len) {
			self->offset -= len;
		} else {
			memmove(
				trb_vector_cell(self, len - self->offset),
				trb_vector_cell(self, self->offset),
				self->len * self->elemsize
			);

			self->offset = 0;
		}
	} else {
		memmove(
			trb_vector_cell(self, self->offset + index + len),
			trb_vector_cell(self, self->offset + index),
			(self->len - index) * self->elemsize
		);
	}

	self->len += len;

	if (data == NULL)
		memset(trb_vector_cell(self, self->offset + index), 0, len * self->elemsize);
	else
		memcpy(trb_vector_cell(self, self->offset + index), data, len * self->elemsize);

	return TRUE;
}

bool trb_vector_push_back(TrbVector *self, const void *data)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_vector_insert_many(self, self->len, data, 1);
}

bool trb_vector_push_back_many(TrbVector *self, const void *data, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_vector_insert_many(self, self->len, data, len);
}

bool trb_vector_push_front(TrbVector *self, const void *data)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_vector_insert_many(self, 0, data, 1);
}

bool trb_vector_push_front_many(TrbVector *self, const void *data, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_vector_insert_many(self, 0, data, len);
}

bool trb_vector_insert(TrbVector *self, usize index, const void *data)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_vector_insert_many(self, index, data, 1);
}

bool trb_vector_insert_many(TrbVector *self, usize index, const void *data, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_vector_insert_many(self, index, data, len);
}

static bool __trb_vector_remove_range(TrbVector *self, usize index, usize len, void *ret)
{
	if (len == 0)
		return TRUE;

	if (trb_chk_add(index, len, NULL) || trb_chk_add(index + len, self->offset, NULL)) {
		trb_msg_error("vector index overflow!");
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

	if (ret != NULL)
		memcpy(ret, trb_vector_cell(self, self->offset + index), len * self->elemsize);

	if (index == 0) {
		self->offset += len;
	} else if (index + len != self->len) {
		memmove(
			trb_vector_cell(self, self->offset + index),
			trb_vector_cell(self, self->offset + index + len),
			(self->len - len - index) * self->elemsize
		);
	}

	self->len -= len;

	return TRUE;
}

bool trb_vector_remove(TrbVector *self, usize index, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len == 0) {
		trb_msg_warn("vector is empty!");
		return FALSE;
	}

	return __trb_vector_remove_range(self, index, 1, ret);
}

bool trb_vector_remove_all(TrbVector *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len == 0) {
		trb_msg_warn("vector is empty!");
		return FALSE;
	}

	return __trb_vector_remove_range(self, 0, self->len, ret);
}

bool trb_vector_pop_back(TrbVector *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len == 0) {
		trb_msg_warn("vector is empty!");
		return FALSE;
	}

	return __trb_vector_remove_range(self, self->len - 1, 1, ret);
}

bool trb_vector_pop_back_many(TrbVector *self, usize len, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len < len) {
		trb_msg_warn("vector doesn't hold %zu elements!", len);
		return FALSE;
	}

	return __trb_vector_remove_range(self, self->len - len, len, ret);
}

bool trb_vector_pop_front(TrbVector *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len == 0) {
		trb_msg_warn("vector is empty!");
		return FALSE;
	}

	return __trb_vector_remove_range(self, 0, 1, ret);
}

bool trb_vector_pop_front_many(TrbVector *self, usize len, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len < len) {
		trb_msg_warn("vector doesn't hold %zu elements!", len);
		return FALSE;
	}

	return __trb_vector_remove_range(self, 0, len, ret);
}

bool trb_vector_remove_range(TrbVector *self, usize index, usize len, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len < len) {
		trb_msg_warn("vector doesn't hold %zu elements!", len);
		return FALSE;
	}

	return __trb_vector_remove_range(self, index, len, ret);
}

void *trb_vector_steal(TrbVector *self, usize *len)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->data == NULL) {
		trb_msg_warn("vector buffer is NULL!");
		return NULL;
	}

	void *ret = self->data;

	if (len != NULL)
		*len = self->len;

	self->len = 0;
	self->offset = 0;
	self->capacity = VECTOR_INIT_CAP;

	if (self->clear)
		self->data = calloc(VECTOR_INIT_CAP, self->elemsize);
	else
		self->data = malloc(VECTOR_INIT_CAP * self->elemsize);

	if (self->data == NULL) {
		self->capacity = 0;
		trb_msg_error("couldn't allocate memory for a new buffer of the vector!");
	}

	return ret;
}

void trb_vector_destroy(TrbVector *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);

	if (self->data == NULL)
		return;

	if (free_func != NULL) {
		for (usize i = 0; i < self->len; ++i) {
			free_func(trb_vector_cell(self, i));
		}
	}

	free(self->data);

	self->data = NULL;
	self->capacity = 0;
	self->len = 0;
	self->offset = 0;
}

void *trb_vector_steal0(TrbVector *self, usize *len)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->data == NULL) {
		trb_msg_warn("vector buffer is NULL!");
		return NULL;
	}

	void *ret = self->data;

	if (len != NULL)
		*len = self->len;

	self->len = 0;
	self->offset = 0;
	self->capacity = 0;
	self->data = NULL;

	return ret;
}

void trb_vector_free(TrbVector *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);
	trb_vector_destroy(self, free_func);
	free(self);
}

bool trb_vector_search(const TrbVector *self, const void *target, TrbCmpFunc cmp_func, usize *index)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(cmp_func != NULL, FALSE);

	if (self->len == 0)
		return FALSE;

	for (usize i = 0; i < self->len; ++i) {
		if (cmp_func(trb_vector_cell(self, self->offset + i), target) == 0) {
			if (index != NULL)
				*index = i;
			return TRUE;
		}
	}

	return FALSE;
}

bool trb_vector_search_data(const TrbVector *self, const void *target, TrbCmpDataFunc cmpd_func, void *data, usize *index)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(cmpd_func != NULL, FALSE);

	if (self->len == 0)
		return FALSE;

	for (usize i = 0; i < self->len; ++i) {
		if (cmpd_func(trb_vector_cell(self, self->offset + i), target, data) == 0) {
			if (index != NULL)
				*index = i;
			return TRUE;
		}
	}

	return FALSE;
}

static void *__trb_vector_slice_at(const TrbSlice *self, usize index)
{
	TrbVector *vector = self->data;

	usize len = trb_slice_len(self);
	if (index >= len)
		return trb_vector_cell(vector, vector->offset + self->end);

	return trb_vector_cell(vector, vector->offset + self->start + index);
}

TrbSlice *trb_vector_slice(TrbVector *self, TrbSlice *slice, usize start, usize end)
{
	trb_return_val_if_fail(self != NULL, NULL);
	trb_return_val_if_fail(start <= end, NULL);

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

	slice->at = __trb_vector_slice_at;
	slice->data = self;
	slice->start = start;
	slice->end = end;
	slice->elemsize = self->elemsize;

	return slice;
}

TrbVector *trb_vector_copy(const TrbVector *src, TrbVector *dst)
{
	trb_return_val_if_fail(src != NULL, NULL);

	if (src->data == NULL) {
		trb_msg_warn("source vector buffer is NULL!");
		return NULL;
	}

	bool was_allocated = FALSE;

	if (dst == NULL) {
		dst = trb_talloc(TrbVector, 1);

		if (dst == NULL) {
			trb_msg_error("couldn't allocate memory for a copy of the vector!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	if (src->clear)
		dst->data = calloc(src->capacity, src->elemsize);
	else
		dst->data = malloc(src->capacity * src->elemsize);

	if (dst->data == NULL) {
		if (was_allocated)
			free(dst);

		trb_msg_error("couldn't allocate memory for a buffer of a copy of the vector!");
		return NULL;
	}

	dst->elemsize = src->elemsize;
	dst->len = src->len;
	dst->offset = src->offset;
	dst->capacity = src->capacity;
	dst->clear = src->clear;

	memcpy(dst->data, src->data, dst->len * dst->elemsize);

	return dst;
}

bool trb_vector_require(TrbVector *self, usize newcap)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (newcap <= self->capacity)
		return TRUE;

	return __trb_vector_newcap(self, newcap);
}

bool trb_vector_shrink(TrbVector *self)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	usize mincap = self->len;

	if (mincap == 0) {
		mincap = VECTOR_INIT_CAP;
	}

	if (self->offset != 0) {
		memmove(
			trb_vector_cell(self, 0),
			trb_vector_cell(self, self->offset),
			self->len * self->elemsize
		);

		self->offset = 0;
	}

	void *data = realloc(self->data, mincap * self->elemsize);

	if (data == NULL) {
		trb_msg_error("couldn't shrink memory of the vector buffer!");
		return FALSE;
	}

	self->data = data;
	self->capacity = mincap;

	return TRUE;
}
