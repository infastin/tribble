#include "Vector.h"

#include "Messages.h"
#include "Types.h"
#include "Utils.h"

#include <memory.h>
#include <stdlib.h>

#define VECTOR_INIT_CAP 4
#define BINARY_SEARCH_LEN_THRESHOLD 64
#define vector_cell(a, i) ((void *) &((char *) ((a)->data))[(i) * (a)->elemsize])

Vector *vector_init(Vector *vec, bool clear, bool zero_terminated, usize elemsize)
{
	return_val_if_fail(elemsize != 0, NULL);

	if (elemsize > USIZE_MAX / VECTOR_INIT_CAP) {
		msg_error("array capacity overflow!");
		return FALSE;
	}

	bool was_allocated = FALSE;

	if (vec == NULL) {
		vec = talloc(Vector, 1);

		if (vec == NULL) {
			msg_error("couldn't allocate memory for the array!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	if (clear)
		vec->data = calloc(VECTOR_INIT_CAP, elemsize);
	else
		vec->data = malloc(VECTOR_INIT_CAP * elemsize);

	if (vec->data == NULL) {
		if (was_allocated)
			free(vec);

		msg_error("couldn't allocate memory for the array buffer!");
		return NULL;
	}

	if (!clear && zero_terminated)
		memset(vec->data, 0, elemsize);

	vec->len = 0;
	vec->capacity = VECTOR_INIT_CAP;
	vec->elemsize = elemsize;
	vec->clear = clear;
	vec->zero_terminated = zero_terminated;
	vec->sorted = 0;

	return vec;
}

static bool __vector_newcap(Vector *vec, usize newcap)
{
	usize new_allocated = (newcap >> 3) + (newcap < 9 ? 3 : 6);

	if (newcap > USIZE_MAX - new_allocated) {
		msg_error("array capacity overflow!");
		return FALSE;
	}

	newcap += new_allocated;

	if (newcap > USIZE_MAX / vec->elemsize) {
		msg_error("array capacity overflow!");
		return FALSE;
	}

	void *data = (void *) realloc(vec->data, newcap * vec->elemsize);

	if (data == NULL) {
		msg_error("couldn't reallocate memory for the array buffer!");
		return FALSE;
	}

	vec->data = data;

	if (vec->clear) {
		memset(
			vector_cell(vec, vec->capacity), 0,
			(newcap - vec->capacity) * vec->elemsize
		);
	}

	vec->capacity = newcap;

	return TRUE;
}

static bool __vector_insert_many(Vector *vec, usize index, const void *data, usize len)
{
	if (len == 0)
		return TRUE;

	usize zt = vec->zero_terminated;

	if (
		(index > USIZE_MAX - len) ||
		(index + len > USIZE_MAX - zt) ||
		(vec->len > USIZE_MAX - len) ||
		(vec->len + len > USIZE_MAX - zt)
	) {
		msg_error("array capacity overflow!");
		return FALSE;
	}

	if (index >= vec->len && index + len + zt > vec->capacity) {
		if (__vector_newcap(vec, index + len + zt) == FALSE)
			return FALSE;
	} else if (index < vec->len && vec->len + len + zt > vec->capacity) {
		if (__vector_newcap(vec, vec->len + len + zt) == FALSE)
			return FALSE;
	}

	if (index >= vec->len) {
		vec->len = index + len;
	} else {
		memmove(
			vector_cell(vec, index + len), vector_cell(vec, index),
			(vec->len - index) * vec->elemsize
		);

		vec->len += len;
	}

	if (zt)
		memset(vector_cell(vec, vec->len), 0, vec->elemsize);

	if (data == NULL)
		memset(vector_cell(vec, index), 0, len * vec->elemsize);
	else
		memcpy(vector_cell(vec, index), data, len * vec->elemsize);

	vec->sorted = 0;

	return TRUE;
}

bool vector_push_back(Vector *vec, const void *data)
{
	return_val_if_fail(vec != NULL, FALSE);
	return __vector_insert_many(vec, vec->len, data, 1);
}

bool vector_push_back_many(Vector *vec, const void *data, usize len)
{
	return_val_if_fail(vec != NULL, FALSE);
	return __vector_insert_many(vec, vec->len, data, len);
}

bool vector_push_front(Vector *vec, const void *data)
{
	return_val_if_fail(vec != NULL, FALSE);
	return __vector_insert_many(vec, 0, data, 1);
}

bool vector_push_front_many(Vector *vec, const void *data, usize len)
{
	return_val_if_fail(vec != NULL, FALSE);
	return __vector_insert_many(vec, 0, data, len);
}

bool vector_insert(Vector *vec, usize index, const void *data)
{
	return_val_if_fail(vec != NULL, FALSE);
	return __vector_insert_many(vec, index, data, 1);
}

bool vector_insert_many(Vector *vec, usize index, const void *data, usize len)
{
	return_val_if_fail(vec != NULL, FALSE);
	return __vector_insert_many(vec, index, data, len);
}

static bool __vector_set_range(Vector *vec, usize index, const void *data, usize len)
{
	if (len == 0)
		return TRUE;

	usize zt = vec->zero_terminated;

	if ((index > USIZE_MAX - len) || (index + len > USIZE_MAX - zt)) {
		msg_error("array capacity overflow!");
		return FALSE;
	}

	if (index + len + zt > vec->capacity) {
		if (__vector_newcap(vec, index + len + zt) == FALSE)
			return FALSE;
	}

	if (index + len > vec->len) {
		vec->len = index + len;

		if (zt)
			memset(vector_cell(vec, vec->len), 0, vec->elemsize);
	}

	if (data == NULL)
		memset(vector_cell(vec, index), 0, len * vec->elemsize);
	else
		memcpy(vector_cell(vec, index), data, len * vec->elemsize);

	vec->sorted = 0;

	return TRUE;
}

bool vector_set(Vector *vec, usize index, const void *data)
{
	return_val_if_fail(vec != NULL, FALSE);
	return __vector_set_range(vec, index, data, 1);
}

bool vector_set_range(Vector *vec, usize index, const void *data, usize len)
{
	return_val_if_fail(vec != NULL, FALSE);
	return __vector_set_range(vec, index, data, len);
}

bool vector_get(const Vector *vec, usize index, void *ret)
{
	return_val_if_fail(vec != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (index >= vec->len) {
		msg_warn("element at [%zu] is out of bounds!", index);
		return FALSE;
	}

	memcpy(ret, vector_cell(vec, index), vec->elemsize);

	return TRUE;
}

bool vector_get_range(const Vector *vec, usize index, usize len, void *ret)
{
	return_val_if_fail(vec != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (len == 0)
		return TRUE;

	if (index + len > vec->len) {
		msg_warn("range [%zu:%zu] is out of bounds!", index, index + len - 1);
		return FALSE;
	}

	memcpy(ret, vector_cell(vec, index), len * vec->elemsize);

	return TRUE;
}

static bool __vector_remove_range(Vector *vec, usize index, usize len, void *ret)
{
	if (len == 0)
		return TRUE;

	if (index > USIZE_MAX - len) {
		msg_error("array index overflow!");
		return FALSE;
	}

	if (index + len > vec->len) {
		if (len == 1) {
			msg_warn("element at [%zu] is out of bounds!", index);
		} else {
			msg_warn("range [%zu:%zu] is out of bounds!", index, index + len - 1);
		}

		return FALSE;
	}

	usize zt = vec->zero_terminated;

	if (ret != NULL)
		memcpy(ret, vector_cell(vec, index), len * vec->elemsize);

	if (index + len != vec->len)
		memmove(
			vector_cell(vec, index), vector_cell(vec, index + len),
			((vec->len + zt) - len - index) * vec->elemsize
		);
	else if (zt)
		memset(vector_cell(vec, index), 0, vec->elemsize);

	vec->len -= len;

	return TRUE;
}

bool vector_remove_index(Vector *vec, usize index, void *ret)
{
	return_val_if_fail(vec != NULL, FALSE);
	return __vector_remove_range(vec, index, 1, ret);
}

bool vector_remove_all(Vector *vec, void *ret)
{
	return_val_if_fail(vec != NULL, FALSE);
	return __vector_remove_range(vec, 0, vec->len, ret);
}

bool vector_pop_back(Vector *vec, void *ret)
{
	return_val_if_fail(vec != NULL, FALSE);

	if (vec->len == 0) {
		msg_warn("array is empty!");
		return FALSE;
	}

	return __vector_remove_range(vec, vec->len - 1, 1, ret);
}

bool vector_pop_front(Vector *vec, void *ret)
{
	return_val_if_fail(vec != NULL, FALSE);
	return __vector_remove_range(vec, 0, 1, ret);
}

bool vector_remove_range(Vector *vec, usize index, usize len, void *ret)
{
	return_val_if_fail(vec != NULL, FALSE);
	return __vector_remove_range(vec, index, len, ret);
}

void vector_reverse(Vector *vec)
{
	return_if_fail(vec != NULL);

	if (vec->len < 2)
		return;

	for (usize lo = 0, hi = vec->len - 1; lo < hi; ++lo, --hi) {
		array_swap(vector_cell(vec, lo), vector_cell(vec, hi), vec->elemsize);
	}
}

void vector_sort(Vector *vec, CmpFunc cmp_func)
{
	return_if_fail(vec != NULL);
	return_if_fail(cmp_func != NULL);

	if (vec->len <= 1 || vec->sorted)
		return;

	quicksort(vec->data, vec->len, vec->elemsize, cmp_func);
	vec->sorted = 1;
}

bool vector_steal(Vector *vec, void *ret, usize *len, bool to_copy)
{
	return_val_if_fail(vec != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (vec->data == NULL) {
		msg_warn("array buffer is NULL!");
		return FALSE;
	}

	if (to_copy) {
		memcpy(ret, vec->data, vec->len * vec->elemsize);
		free(vec->data);
	} else
		*((void **) ret) = vec->data;

	if (len != NULL)
		*len = vec->len;

	vec->len = 0;
	vec->capacity = VECTOR_INIT_CAP;

	if (vec->clear)
		vec->data = calloc(VECTOR_INIT_CAP, vec->elemsize);
	else
		vec->data = malloc(VECTOR_INIT_CAP * vec->elemsize);

	if (vec->data == NULL) {
		vec->capacity = 0;
		msg_error("couldn't allocate memory for a new buffer of the array!");
		return FALSE;
	}

	return TRUE;
}

void vector_destroy(Vector *vec, FreeFunc free_func)
{
	return_if_fail(vec != NULL);

	if (vec->data == NULL)
		return;

	if (free_func != NULL) {
		for (usize i = 0; i < vec->len; ++i) {
			free_func(vector_cell(vec, i));
		}
	}

	free(vec->data);

	vec->data = NULL;
	vec->capacity = 0;
	vec->len = 0;
}

bool vector_steal0(Vector *vec, void *ret, usize *len, bool to_copy)
{
	return_val_if_fail(vec != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (vec->data == NULL) {
		msg_warn("array buffer is NULL!");
		return FALSE;
	}

	if (to_copy) {
		memcpy(ret, vec->data, vec->len * vec->elemsize);
		free(vec->data);
	} else
		*((void **) ret) = vec->data;

	if (len != NULL)
		*len = vec->len;

	vec->len = 0;
	vec->capacity = 0;
	vec->data = NULL;

	return TRUE;
}

void vector_free(Vector *vec, FreeFunc free_func)
{
	return_if_fail(vec != NULL);
	vector_destroy(vec, free_func);
	free(vec);
}

bool vector_search(Vector *vec, const void *target, CmpFunc cmp_func, usize *index)
{
	return_val_if_fail(vec != NULL, FALSE);
	return_val_if_fail(cmp_func != NULL, FALSE);

	if (vec->len == 0)
		return FALSE;

	if (vec->len < BINARY_SEARCH_LEN_THRESHOLD) {
		for (usize i = 0; i < vec->len; ++i) {
			if (cmp_func(vector_cell(vec, i), target) == 0) {
				if (index != NULL)
					*index = i;
				return TRUE;
			}
		}

		return FALSE;
	}

	if (vec->sorted == 0) {
		quicksort(vec->data, vec->len, vec->elemsize, cmp_func);
		vec->sorted = 1;
	}

	return binary_search(
		vec->data, target, vec->len, vec->elemsize, cmp_func, index
	);
}

Vector *vector_copy(Vector *dst, const Vector *src)
{
	return_val_if_fail(src != NULL, NULL);

	if (src->data == NULL) {
		msg_warn("source array buffer is NULL!");
		return NULL;
	}

	bool was_allocated = FALSE;

	if (dst == NULL) {
		dst = talloc(Vector, 1);

		if (dst == NULL) {
			msg_error("couldn't allocate memory for a copy of the array!");
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

		msg_error("couldn't allocate memory for a buffer of a copy of the array!");
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

bool vector_require(Vector *vec, usize newcap)
{
	return_val_if_fail(vec != NULL, FALSE);

	if (newcap <= vec->capacity)
		return TRUE;

	return __vector_newcap(vec, newcap);
}

bool vector_shrink(Vector *vec)
{
	return_val_if_fail(vec != NULL, FALSE);

	usize zt = vec->zero_terminated;

	void *data = (void *) realloc(vec->data, (vec->len + zt) * vec->elemsize);

	if (data == NULL) {
		msg_error("couldn't shrink memory of the array buffer!");
		return FALSE;
	}

	vec->data = data;
	vec->capacity = vec->len + zt;

	return TRUE;
}
