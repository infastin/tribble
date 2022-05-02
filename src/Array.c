#include "Array.h"

#include "Messages.h"
#include "Types.h"
#include "Utils.h"

#include <memory.h>
#include <stdlib.h>

#define BINARY_SEARCH_LEN_THRESHOLD 32
#define arr_cell(a, i) ((void *) &((char *) ((a)->mass))[(i) * (a)->elemsize])

Array *array_init(Array *arr, bool clear, bool zero_terminated, u32 elemsize)
{
	return_val_if_fail(elemsize != 0, NULL);

	bool was_allocated = FALSE;

	if (arr == NULL) {
		arr = talloc(Array, 1);

		if (arr == NULL) {
			msg_error("couldn't allocate memory for the array!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	if (clear)
		arr->mass = calloc(1, elemsize);
	else
		arr->mass = malloc(elemsize);

	if (arr->mass == NULL) {
		if (was_allocated)
			free(arr);

		msg_error("couldn't allocate memory for the array buffer!");
		return NULL;
	}

	arr->len = 0;
	arr->capacity = 1;
	arr->elemsize = elemsize;
	arr->clear = (clear) ? 1 : 0;
	arr->zero_terminated = (zero_terminated) ? 1 : 0;
	arr->sorted = 0;

	return arr;
}

static bool __array_growcap(Array *arr, u32 add)
{
	if (add == 0)
		return TRUE;

	if (arr->len + add <= arr->capacity)
		return TRUE;

	if (arr->capacity == 0 || arr->mass == NULL) {
		msg_error("array buffer is NULL!");
		return FALSE;
	}

	if (arr->capacity > UINT_MAX - add) {
		msg_error("array capacity overflow!");
		return FALSE;
	}

	u32 mincap = arr->capacity + add;
	u32 new_allocated = (mincap >> 3) + (mincap < 9 ? 3 : 6);

	if (mincap > UINT_MAX - new_allocated) {
		msg_error("array capacity overflow!");
		return FALSE;
	}

	mincap += new_allocated;

	void *mass = (void *) realloc(arr->mass, mincap * arr->elemsize);

	if (mass == NULL) {
		msg_error("couldn't reallocate memory for the array buffer!");
		return FALSE;
	}

	arr->mass = mass;

	if (arr->clear)
		memset(
			arr_cell(arr, arr->capacity), 0,
			(mincap - arr->capacity) * arr->elemsize
		);

	arr->capacity = mincap;

	return TRUE;
}

static bool __array_insert(Array *arr, u32 index, const void *data)
{
	u32 zt = arr->zero_terminated;

	if (index > UINT_MAX - zt || arr->len > UINT_MAX - zt - 1) {
		msg_error("array capacity overflow!");
		return FALSE;
	}

	if (index + zt >= arr->capacity) {
		u32 err = __array_growcap(arr, (index + zt) - (arr->capacity - 1));
		return_val_if_fail(err == 0, err);
	} else if (arr->len + zt + 1 > arr->capacity) {
		u32 err = __array_growcap(arr, 1);
		return_val_if_fail(err == 0, err);
	}

	if (index >= arr->len) {
		arr->len = index + 1;

		if (zt)
			memset(arr_cell(arr, index + 1), 0, arr->elemsize);
	} else {
		memmove(
			arr_cell(arr, index + 1), arr_cell(arr, index),
			((arr->len + zt) - index) * arr->elemsize
		);
		arr->len++;
	}

	if (data == NULL)
		memset(arr_cell(arr, index), 0, arr->elemsize);
	else
		memcpy(arr_cell(arr, index), data, arr->elemsize);

	arr->sorted = 0;

	return TRUE;
}

static bool __array_insert_many(Array *arr, u32 index, const void *data, u32 len)
{
	if (len == 0)
		return TRUE;

	u32 zt = arr->zero_terminated;

	if (
		index > UINT_MAX - len ||
		index + len > UINT_MAX - zt ||
		arr->len > UINT_MAX - len ||
		arr->len + len > UINT_MAX - zt
	) {
		msg_error("array capacity overflow!");
		return FALSE;
	}

	if (index + zt + len > arr->capacity) {
		u32 err = __array_growcap(arr, (index + zt + len) - arr->capacity);
		return_val_if_fail(err == 0, err);
	} else if (arr->len + zt + len > arr->capacity) {
		u32 err = __array_growcap(arr, (arr->len + zt + len) - arr->capacity);
		return_val_if_fail(err == 0, err);
	}

	if (index >= arr->len) {
		arr->len = index + len;

		if (zt)
			memset(arr_cell(arr, index + len), 0, arr->elemsize);
	} else {
		memmove(
			arr_cell(arr, index + len), arr_cell(arr, index),
			((arr->len + zt) - index) * arr->elemsize
		);
		arr->len += len;
	}

	if (data == NULL)
		memset(arr_cell(arr, index), 0, len * arr->elemsize);
	else
		memcpy(arr_cell(arr, index), data, len * arr->elemsize);

	arr->sorted = 0;

	return TRUE;
}

bool array_push_back(Array *arr, const void *data)
{
	return_val_if_fail(arr != NULL, FALSE);
	return __array_insert(arr, arr->len, data);
}

bool array_push_back_many(Array *arr, const void *data, u32 len)
{
	return_val_if_fail(arr != NULL, FALSE);
	return __array_insert_many(arr, arr->len, data, len);
}

bool array_push_front(Array *arr, const void *data)
{
	return_val_if_fail(arr != NULL, FALSE);
	return __array_insert(arr, 0, data);
}

bool array_push_front_many(Array *arr, const void *data, u32 len)
{
	return_val_if_fail(arr != NULL, FALSE);
	return __array_insert_many(arr, 0, data, len);
}

bool array_insert(Array *arr, u32 index, const void *data)
{
	return_val_if_fail(arr != NULL, FALSE);
	return __array_insert(arr, index, data);
}

bool array_insert_many(Array *arr, u32 index, const void *data, u32 len)
{
	return_val_if_fail(arr != NULL, FALSE);
	return __array_insert_many(arr, index, data, len);
}

bool array_set(Array *arr, u32 index, const void *data)
{
	return_val_if_fail(arr != NULL, FALSE);

	u32 zt = arr->zero_terminated;

	if (index > UINT_MAX - zt) {
		msg_error("array capacity overflow!");
		return FALSE;
	}

	if (index + zt >= arr->capacity) {
		u32 err = __array_growcap(arr, (index + zt + 1) - arr->capacity);
		return_val_if_fail(err != 0, err);
	}

	if (data == NULL)
		memset(arr_cell(arr, index), 0, arr->elemsize);
	else
		memcpy(arr_cell(arr, index), data, arr->elemsize);

	if (index >= arr->len) {
		arr->len = index + 1;

		if (zt)
			memset(arr_cell(arr, index + 1), 0, arr->elemsize);
	}

	arr->sorted = 0;

	return TRUE;
}

bool array_get(const Array *arr, u32 index, void *ret)
{
	return_val_if_fail(arr != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (index >= arr->len) {
		msg_warn("element at [%lu] is out of bounds!", index);
		return FALSE;
	}

	memcpy(ret, arr_cell(arr, index), arr->elemsize);

	return TRUE;
}

static bool __array_remove_range(Array *arr, u32 index, u32 len, void *ret)
{
	if (index + len - 1 >= arr->len) {
		msg_warn("range [%lu:%lu] is out of bounds!", index, index + len - 1);
		return FALSE;
	}

	if (ret != NULL)
		memcpy(ret, arr_cell(arr, index), len * arr->elemsize);

	u32 zt = arr->zero_terminated;

	if (index + len < arr->len)
		memmove(
			arr_cell(arr, index), arr_cell(arr, index + len),
			((arr->len + zt) - len - index) * arr->elemsize
		);
	else if (zt)
		memset(arr_cell(arr, index), 0, arr->elemsize);

	arr->len -= len;

	return TRUE;
}

bool array_remove_index(Array *arr, u32 index, void *ret)
{
	return_val_if_fail(arr != NULL, FALSE);
	return __array_remove_range(arr, index, 1, ret);
}

bool array_remove_all(Array *arr, void *ret)
{
	return_val_if_fail(arr != NULL, FALSE);
	return __array_remove_range(arr, 0, arr->len, ret);
}

bool array_pop_back(Array *arr, void *ret)
{
	return_val_if_fail(arr != NULL, FALSE);

	if (arr->len == 0) {
		msg_warn("array length is zero!");
		return FALSE;
	}

	return __array_remove_range(arr, arr->len - 1, 1, ret);
}

bool array_pop_front(Array *arr, void *ret)
{
	return_val_if_fail(arr != NULL, FALSE);

	if (arr->len == 0) {
		msg_warn("array length is zero!");
		return FALSE;
	}

	return __array_remove_range(arr, 0, 1, ret);
}

bool array_remove_range(Array *arr, u32 index, u32 len, void *ret)
{
	return_val_if_fail(arr != NULL, FALSE);

	if (len == 0)
		return TRUE;

	return __array_remove_range(arr, index, len, ret);
}

void array_reverse(Array *arr)
{
	return_if_fail(arr != NULL);

	if (arr->len < 2)
		return;

	for (u32 lo = 0, hi = arr->len - 1; lo < hi; ++lo, --hi) {
		mass_swap(arr_cell(arr, lo), arr_cell(arr, hi), arr->elemsize);
	}
}

void array_sort(Array *arr, CmpFunc cmp_func)
{
	return_if_fail(arr != NULL);
	return_if_fail(cmp_func != NULL);

	if (arr->len <= 1 || arr->sorted)
		return;

	quicksort(arr->mass, arr->len, arr->elemsize, cmp_func);
	arr->sorted = 1;
}

bool array_steal(Array *arr, void *ret, u32 *len, bool to_copy)
{
	return_val_if_fail(arr != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (to_copy) {
		memcpy(ret, arr->mass, arr->len * arr->elemsize);
		free(arr->mass);
	} else
		*((void **) ret) = arr->mass;

	if (len != NULL)
		*len = arr->len;

	arr->len = 0;
	arr->capacity = 1;

	if (arr->clear)
		arr->mass = calloc(1, arr->elemsize);
	else
		arr->mass = malloc(arr->elemsize);

	if (arr->mass == NULL) {
		arr->capacity = 0;
		msg_error("couldn't allocate memory for a new buffer of the array!");
		return FALSE;
	}

	return TRUE;
}

void array_purge(Array *arr)
{
	return_if_fail(arr != NULL);

	if (arr->mass != NULL) {
		free(arr->mass);

		arr->mass = NULL;
		arr->capacity = 0;
		arr->len = 0;
	}
}

bool array_steal0(Array *arr, void *ret, u32 *len, bool to_copy)
{
	return_val_if_fail(arr != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (to_copy) {
		memcpy(ret, arr->mass, arr->len * arr->elemsize);
		free(arr->mass);
	} else
		*((void **) ret) = arr->mass;

	if (len != NULL)
		*len = arr->len;

	arr->len = 0;
	arr->capacity = 0;
	arr->mass = NULL;

	return TRUE;
}

void array_free(Array *arr)
{
	return_if_fail(arr != NULL);

	array_purge(arr);
	free(arr);
}

bool array_search(Array *arr, const void *target, CmpFunc cmp_func, u32 *index)
{
	return_val_if_fail(arr != NULL, FALSE);
	return_val_if_fail(cmp_func != NULL, FALSE);

	if (arr->len == 0)
		return FALSE;

	if (arr->len < BINARY_SEARCH_LEN_THRESHOLD)
		return linear_search(
			arr->mass, target, arr->len, arr->elemsize, cmp_func, index
		);

	if (arr->sorted == 0) {
		quicksort(arr->mass, arr->len, arr->elemsize, cmp_func);
		arr->sorted = 1;
	}

	return binary_search(
		arr->mass, target, arr->len, arr->elemsize, cmp_func, index
	);
}

Array *array_copy(Array *dst, const Array *src)
{
	return_val_if_fail(src != NULL, NULL);

	bool was_allocated = FALSE;

	if (dst == NULL) {
		dst = talloc(Array, 1);

		if (dst == NULL) {
			msg_error("couldn't allocate memory for the copy of the array!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	if (src->clear)
		dst->mass = calloc(src->capacity, src->elemsize);
	else
		dst->mass = malloc(src->capacity * src->elemsize);

	if (dst->mass == NULL) {
		if (was_allocated)
			free(dst);

		msg_error(
			"couldn't allocate memory for a buffer of the copy of the array!"
		);
		return NULL;
	}

	dst->elemsize = src->elemsize;
	dst->len = src->len;
	dst->capacity = src->capacity;
	dst->sorted = src->sorted;
	dst->zero_terminated = src->zero_terminated;
	dst->clear = src->clear;

	u32 zt = dst->zero_terminated;
	memcpy(dst->mass, src->mass, (dst->len + zt) * dst->elemsize);

	return dst;
}
