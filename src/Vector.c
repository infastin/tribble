#include "Vector.h"

#include "Messages.h"
#include "Types.h"
#include "Utils.h"

#include <memory.h>
#include <stdlib.h>

#define VECTOR_INIT_CAP 8
#define BINARY_SEARCH_LEN_THRESHOLD 64

#define vector_cell(a, i) ((void *) &((char *) ((a)->data))[(i) * (a)->elemsize])

TrbVector *trb_vector_init(TrbVector *self, bool clear, bool zero_terminated, usize elemsize)
{
	trb_return_val_if_fail(elemsize != 0, NULL);

	if (elemsize > USIZE_MAX / VECTOR_INIT_CAP) {
		trb_msg_error("array capacity overflow!");
		return FALSE;
	}

	bool was_allocated = FALSE;

	if (self == NULL) {
		self = trb_talloc(TrbVector, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the array!");
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

		trb_msg_error("couldn't allocate memory for the array buffer!");
		return NULL;
	}

	if (!clear && zero_terminated)
		memset(self->data, 0, elemsize);

	self->len = 0;
	self->capacity = VECTOR_INIT_CAP;
	self->elemsize = elemsize;
	self->clear = clear;
	self->zero_terminated = zero_terminated;
	self->sorted = FALSE;

	return self;
}

static bool __trb_vector_newcap(TrbVector *self, usize newcap)
{
	usize new_allocated = (newcap >> 3) + (newcap < 9 ? 3 : 6);

	if (newcap > USIZE_MAX - new_allocated) {
		trb_msg_error("array capacity overflow!");
		return FALSE;
	}

	newcap += new_allocated;

	if (newcap > USIZE_MAX / self->elemsize) {
		trb_msg_error("array capacity overflow!");
		return FALSE;
	}

	void *data = realloc(self->data, newcap * self->elemsize);

	if (data == NULL) {
		trb_msg_error("couldn't reallocate memory for the array buffer!");
		return FALSE;
	}

	self->data = data;

	if (self->clear) {
		memset(
			vector_cell(self, self->capacity), 0,
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

	usize zt = self->zero_terminated;

	if (
		(index > USIZE_MAX - len) ||
		(index + len > USIZE_MAX - zt) ||
		(self->len > USIZE_MAX - len) ||
		(self->len + len > USIZE_MAX - zt)
	) {
		trb_msg_error("array capacity overflow!");
		return FALSE;
	}

	if (index >= self->len && index + len + zt > self->capacity) {
		if (__trb_vector_newcap(self, index + len + zt) == FALSE)
			return FALSE;
	} else if (index < self->len && self->len + len + zt > self->capacity) {
		if (__trb_vector_newcap(self, self->len + len + zt) == FALSE)
			return FALSE;
	}

	if (index >= self->len) {
		self->len = index + len;
	} else {
		memmove(
			vector_cell(self, index + len), vector_cell(self, index),
			(self->len - index) * self->elemsize
		);

		self->len += len;
	}

	if (zt)
		memset(vector_cell(self, self->len), 0, self->elemsize);

	if (data == NULL)
		memset(vector_cell(self, index), 0, len * self->elemsize);
	else
		memcpy(vector_cell(self, index), data, len * self->elemsize);

	self->sorted = FALSE;

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

static bool __trb_vector_set_range(TrbVector *self, usize index, const void *data, usize len)
{
	if (len == 0)
		return TRUE;

	usize zt = self->zero_terminated;

	if ((index > USIZE_MAX - len) || (index + len > USIZE_MAX - zt)) {
		trb_msg_error("array capacity overflow!");
		return FALSE;
	}

	if (index + len + zt > self->capacity) {
		if (__trb_vector_newcap(self, index + len + zt) == FALSE)
			return FALSE;
	}

	if (index + len > self->len) {
		self->len = index + len;

		if (zt)
			memset(vector_cell(self, self->len), 0, self->elemsize);
	}

	if (data == NULL)
		memset(vector_cell(self, index), 0, len * self->elemsize);
	else
		memcpy(vector_cell(self, index), data, len * self->elemsize);

	self->sorted = FALSE;

	return TRUE;
}

bool trb_vector_set(TrbVector *self, usize index, const void *data)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_vector_set_range(self, index, data, 1);
}

bool trb_vector_set_range(TrbVector *self, usize index, const void *data, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_vector_set_range(self, index, data, len);
}

bool trb_vector_get(const TrbVector *self, usize index, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(ret != NULL, FALSE);

	if (index >= self->len) {
		trb_msg_warn("element at [%zu] is out of bounds!", index);
		return FALSE;
	}

	memcpy(ret, vector_cell(self, index), self->elemsize);

	return TRUE;
}

bool trb_vector_get_range(const TrbVector *self, usize index, usize len, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(ret != NULL, FALSE);

	if (len == 0)
		return TRUE;

	if (index + len > self->len) {
		trb_msg_warn("range [%zu:%zu] is out of bounds!", index, index + len - 1);
		return FALSE;
	}

	memcpy(ret, vector_cell(self, index), len * self->elemsize);

	return TRUE;
}

static bool __trb_vector_remove_range(TrbVector *self, usize index, usize len, void *ret)
{
	if (len == 0)
		return TRUE;

	if (index > USIZE_MAX - len) {
		trb_msg_error("array index overflow!");
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

	usize zt = self->zero_terminated;

	if (ret != NULL)
		memcpy(ret, vector_cell(self, index), len * self->elemsize);

	if (index + len != self->len)
		memmove(
			vector_cell(self, index), vector_cell(self, index + len),
			((self->len + zt) - len - index) * self->elemsize
		);
	else if (zt)
		memset(vector_cell(self, index), 0, self->elemsize);

	self->len -= len;

	return TRUE;
}

bool trb_vector_remove_index(TrbVector *self, usize index, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_vector_remove_range(self, index, 1, ret);
}

bool trb_vector_remove_all(TrbVector *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_vector_remove_range(self, 0, self->len, ret);
}

bool trb_vector_pop_back(TrbVector *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len == 0) {
		trb_msg_warn("array is empty!");
		return FALSE;
	}

	return __trb_vector_remove_range(self, self->len - 1, 1, ret);
}

bool trb_vector_pop_back_many(TrbVector *self, usize len, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len < len) {
		trb_msg_warn("array doesn't hold %zu elements!", len);
		return FALSE;
	}

	return __trb_vector_remove_range(self, self->len - len, len, ret);
}

bool trb_vector_pop_front(TrbVector *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_vector_remove_range(self, 0, 1, ret);
}

bool trb_vector_pop_front_many(TrbVector *self, usize len, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len < len) {
		trb_msg_warn("array doesn't hold %zu elements!", len);
		return FALSE;
	}

	return __trb_vector_remove_range(self, 0, len, ret);
}

bool trb_vector_remove_range(TrbVector *self, usize index, usize len, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_vector_remove_range(self, index, len, ret);
}

void trb_vector_reverse(TrbVector *self)
{
	trb_return_if_fail(self != NULL);

	if (self->len < 2)
		return;

	for (usize lo = 0, hi = self->len - 1; lo < hi; ++lo, --hi) {
		trb_array_swap(vector_cell(self, lo), vector_cell(self, hi), self->elemsize);
	}
}

void trb_vector_sort(TrbVector *self, TrbCmpFunc cmp_func)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(cmp_func != NULL);

	if (self->len <= 1 || self->sorted)
		return;

	trb_quicksort(self->data, self->len, self->elemsize, cmp_func);
	self->sorted = TRUE;
}

void trb_vector_sort_data(TrbVector *self, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(cmpd_func != NULL);

	if (self->len <= 1 || self->sorted)
		return;

	trb_quicksort_data(self->data, self->len, self->elemsize, cmpd_func, data);
	self->sorted = TRUE;
}

void *trb_vector_steal(TrbVector *self, usize *len)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->data == NULL) {
		trb_msg_warn("array buffer is NULL!");
		return NULL;
	}

	void *ret = self->data;

	if (len != NULL)
		*len = self->len;

	self->len = 0;
	self->capacity = VECTOR_INIT_CAP;

	if (self->clear)
		self->data = calloc(VECTOR_INIT_CAP, self->elemsize);
	else
		self->data = malloc(VECTOR_INIT_CAP * self->elemsize);

	if (self->data == NULL) {
		self->capacity = 0;
		trb_msg_error("couldn't allocate memory for a new buffer of the array!");
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
			free_func(vector_cell(self, i));
		}
	}

	free(self->data);

	self->data = NULL;
	self->capacity = 0;
	self->len = 0;
}

void *trb_vector_steal0(TrbVector *self, usize *len)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->data == NULL) {
		trb_msg_warn("array buffer is NULL!");
		return NULL;
	}

	void *ret = self->data;

	if (len != NULL)
		*len = self->len;

	self->len = 0;
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
		if (cmp_func(vector_cell(self, i), target) == 0) {
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
		if (cmpd_func(vector_cell(self, i), target, data) == 0) {
			if (index != NULL)
				*index = i;
			return TRUE;
		}
	}

	return FALSE;
}

TrbVector *trb_vector_copy(TrbVector *dst, const TrbVector *src)
{
	trb_return_val_if_fail(src != NULL, NULL);

	if (src->data == NULL) {
		trb_msg_warn("source array buffer is NULL!");
		return NULL;
	}

	bool was_allocated = FALSE;

	if (dst == NULL) {
		dst = trb_talloc(TrbVector, 1);

		if (dst == NULL) {
			trb_msg_error("couldn't allocate memory for a copy of the array!");
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

		trb_msg_error("couldn't allocate memory for a buffer of a copy of the array!");
		return NULL;
	}

	dst->elemsize = src->elemsize;
	dst->len = src->len;
	dst->capacity = src->capacity;
	dst->sorted = src->sorted;
	dst->zero_terminated = src->zero_terminated;
	dst->clear = src->clear;

	usize zt = dst->zero_terminated;
	memcpy(dst->data, src->data, (dst->len + zt) * dst->elemsize);

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

	usize zt = self->zero_terminated;
	usize mincap = self->len + zt;

	if (mincap == 0) {
		mincap = VECTOR_INIT_CAP;
	}

	void *data = realloc(self->data, mincap * self->elemsize);

	if (data == NULL) {
		trb_msg_error("couldn't shrink memory of the array buffer!");
		return FALSE;
	}

	self->data = data;
	self->capacity = mincap;

	return TRUE;
}
