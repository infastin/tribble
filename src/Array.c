#include <stdlib.h>
#include <memory.h>

#include "Array.h"
#include "Messages.h"
#include "Types.h"
#include "Utils.h"

#define BINARY_SEARCH_LEN_THRESHOLD 32
#define arr_cell(a, i) ((void*) &((char*) ((a)->mass))[(i) * (a)->elemsize])

typedef struct _ArrayReal ArrayReal;

struct _ArrayReal
{
	void *mass;
	uint len;
	uint capacity;
	uint elemsize;
	uint zero_terminated : 1;
	uint sorted : 1;
	uint clear : 1;
};

Array* array_init(Array *_arr, bool clear, bool zero_terminated, uint elemsize)
{
	return_val_if_fail(elemsize != 0, NULL);

	bool was_allocated = FALSE;

	if (_arr == NULL)
	{
		_arr = talloc(Array, 1);

		if (_arr == NULL)
		{
			msg_error("couldn't allocate memory for the array!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	ArrayReal *arr = (ArrayReal*) _arr;

	if (clear)
		arr->mass = calloc(1, elemsize);
	else
		arr->mass = malloc(elemsize);

	if (arr->mass == NULL)
	{
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

	return (Array*) arr;
}

static int __array_growcap(ArrayReal *arr, uint add)
{
	if (add == 0)
		return 0;

	if (arr->len + add <= arr->capacity)
		return 0;

	if (arr->capacity == 0 || arr->mass == NULL)
	{
		msg_error("array buffer is NULL!");
		return -3;
	}

	if (arr->capacity > UINT_MAX - add)
	{
		msg_error("array capacity overflow!");
		return -1;
	}

	uint mincap = arr->capacity + add;
	uint new_allocated = (mincap >> 3) + (mincap < 9 ? 3 : 6);

	if (mincap > UINT_MAX - new_allocated)
	{
		msg_error("array capacity overflow!");
		return -1;
	}

	mincap += new_allocated;

	void *mass = (void*)realloc(arr->mass, mincap * arr->elemsize);

	if (mass == NULL)
	{
		msg_error("couldn't reallocate memory for the array buffer!");
		return -2;
	}

	arr->mass = mass;

	if (arr->clear)
		memset(arr_cell(arr, arr->capacity), 0, (mincap - arr->capacity) * arr->elemsize);

	arr->capacity = mincap;

	return 0;
}

static int __array_insert(ArrayReal *arr, uint index, const void *data)
{
	uint zt = arr->zero_terminated;

	if (index > UINT_MAX - zt || 
			arr->len > UINT_MAX - zt - 1)
	{
		msg_error("array capacity overflow!");
		return -1;
	}

	if (index + zt >= arr->capacity)
	{
		int err = __array_growcap(arr, (index + zt) - (arr->capacity - 1));
		return_val_if_fail(err == 0, err);
	}
	else if (arr->len + zt + 1 > arr->capacity)
	{
		int err = __array_growcap(arr, 1);
		return_val_if_fail(err == 0, err);
	}

	if (index >= arr->len)
	{
		arr->len = index + 1;

		if (zt)
			memset(arr_cell(arr, index + 1), 0, arr->elemsize);
	}
	else
	{
		memmove(arr_cell(arr, index + 1), arr_cell(arr, index), ((arr->len + zt) - index) * arr->elemsize);
		arr->len++;
	}

	if (data == NULL)
		memset(arr_cell(arr, index), 0, arr->elemsize);
	else
		memcpy(arr_cell(arr, index), data, arr->elemsize);

	arr->sorted = 0;

	return 0;
}

static int __array_insert_many(ArrayReal *arr, uint index, const void *data, uint len)
{
	if (len == 0)
		return 0;

	uint zt = arr->zero_terminated;

	if (index > UINT_MAX - len || 
			index + len > UINT_MAX - zt ||
			arr->len > UINT_MAX - len ||
			arr->len + len > UINT_MAX - zt)
	{
		msg_error("array capacity overflow!");
		return -1;
	}

	if (index + zt + len > arr->capacity)
	{
		int err = __array_growcap(arr, (index + zt + len) - arr->capacity);
		return_val_if_fail(err == 0, err);
	}
	else if (arr->len + zt + len > arr->capacity)
	{
		int err = __array_growcap(arr, (arr->len + zt + len) - arr->capacity);
		return_val_if_fail(err == 0, err);
	}

	if (index >= arr->len)
	{
		arr->len = index + len;

		if (zt)
			memset(arr_cell(arr, index + len), 0, arr->elemsize);
	}
	else
	{
		memmove(arr_cell(arr, index + len), arr_cell(arr, index), ((arr->len + zt) - index) * arr->elemsize);
		arr->len += len;
	}

	if (data == NULL)
		memset(arr_cell(arr, index), 0, len * arr->elemsize);
	else
		memcpy(arr_cell(arr, index), data, len * arr->elemsize);

	arr->sorted = 0;

	return 0;
}

int array_append(Array *_arr, const void *data)
{
	return_val_if_fail(_arr != NULL, -4);
	ArrayReal *arr = (ArrayReal*) _arr;
	return __array_insert(arr, arr->len, data);
}

int array_append_many(Array *_arr, const void *data, uint len)
{
	return_val_if_fail(_arr != NULL, -4);
	ArrayReal *arr = (ArrayReal*) _arr;
	return __array_insert_many(arr, arr->len, data, len);
}

int array_prepend(Array *_arr, const void *data)
{
	return_val_if_fail(_arr != NULL, -4);
	ArrayReal *arr = (ArrayReal*) _arr;
	return __array_insert(arr, 0, data);
}

int array_prepend_many(Array *_arr, const void *data, uint len)
{
	return_val_if_fail(_arr != NULL, -4);
	ArrayReal *arr = (ArrayReal*) _arr;
	return __array_insert_many(arr, 0, data, len);
}

int array_insert(Array *_arr, uint index, const void *data)
{
	return_val_if_fail(_arr != NULL, -4);
	ArrayReal *arr = (ArrayReal*) _arr;
	return __array_insert(arr, index, data);
}

int array_insert_many(Array *_arr, uint index, const void *data, uint len)
{
	return_val_if_fail(_arr != NULL, -4);
	ArrayReal *arr = (ArrayReal*) _arr;
	return __array_insert_many(arr, index, data, len);
}

int array_set(Array *_arr, uint index, const void *data)
{
	return_val_if_fail(_arr != NULL, -4);

	ArrayReal *arr = (ArrayReal*) _arr;
	uint zt = arr->zero_terminated;

	if (index > UINT_MAX - zt)
	{
		msg_error("array capacity overflow!");
		return -1;
	}

	if (index + zt >= arr->capacity)
	{
		int err = __array_growcap(arr, (index + zt + 1) - arr->capacity);
		return_val_if_fail(err != 0, err);
	}

	if (data == NULL)
		memset(arr_cell(arr, index), 0, arr->elemsize);
	else
		memcpy(arr_cell(arr, index), data, arr->elemsize);

	if (index >= arr->len)
	{
		arr->len = index + 1;

		if (zt)
			memset(arr_cell(arr, index + 1), 0, arr->elemsize);
	}

	arr->sorted = 0;

	return 0;
}

int array_get(const Array *_arr, uint index, void *ret)
{
	return_val_if_fail(_arr != NULL, -4);
	return_val_if_fail(ret != NULL, -5);

	ArrayReal *arr = (ArrayReal*) _arr;

	if (index >= arr->len)
	{
		msg_warn("element at [%lu] is out of bounds!", index);
		return -1;
	}

	memcpy(ret, arr_cell(arr, index), arr->elemsize);

	return 0;
}

static int __array_remove_range(Array *_arr, uint index, uint len, void *ret)
{
	ArrayReal *arr = (ArrayReal*) _arr;

	if (index + len - 1 >= arr->len)
	{
		msg_warn("range [%lu:%lu] is out of bounds!", index, index + len - 1);
		return -1;
	}

	if (ret != NULL)
		memcpy(ret, arr_cell(arr, index), len * arr->elemsize);

	uint zt = arr->zero_terminated;

	if (index + len < arr->len)
		memmove(arr_cell(arr, index), arr_cell(arr, index + len), ((arr->len + zt) - len - index) * arr->elemsize);
	else if (zt)
		memset(arr_cell(arr, index), 0, arr->elemsize);

	arr->len -= len;

	return 0;
}

int array_remove_index(Array *arr, uint index, void *ret)
{
	return_val_if_fail(arr != NULL, -4);
	return __array_remove_range(arr, index, 1, ret);
}

int array_pop(Array *arr, void *ret)
{
	return_val_if_fail(arr != NULL, -4);

	if (arr->len == 0)
	{
		msg_warn("array length is zero!");
		return -1;
	}

	return __array_remove_range(arr, arr->len - 1, 1, ret);
}

int array_remove_range(Array *arr, uint index, uint len, void *ret)
{
	return_val_if_fail(arr != NULL, -4);

	if (len == 0)
		return 0;

	return __array_remove_range(arr, index, len, ret);
}

void array_reverse(Array *_arr)
{
	return_if_fail(_arr != NULL);

	ArrayReal *arr = (ArrayReal*) _arr;
	
	if (arr->len < 2)
		return;

	for (uint lo = 0, hi = arr->len - 1; lo < hi; ++lo, --hi)
	{
		mass_swap(arr_cell(arr, lo), arr_cell(arr, hi), arr->elemsize);
	}
}

void array_sort(Array *_arr, CmpFunc cmp_func)
{
	return_if_fail(_arr != NULL);
	return_if_fail(cmp_func != NULL);

	ArrayReal *arr = (ArrayReal*) _arr;
	
	if (arr->len <= 1 || arr->sorted)
		return;

	quicksort(arr->mass, arr->len, arr->elemsize, cmp_func);
	arr->sorted = 1;
}

int array_steal(Array *_arr, void *ret, uint *len, bool to_copy)
{
	return_val_if_fail(_arr != NULL, -4);
	return_val_if_fail(ret != NULL, -5);

	ArrayReal *arr = (ArrayReal*) _arr;

	if (to_copy)
	{
		memcpy(ret, arr->mass, arr->len * arr->elemsize);
		free(arr->mass);
	}
	else
		*((void**) ret) = arr->mass;

	if (len != NULL)
		*len = arr->len;

	arr->len = 0;
	arr->capacity = 1;

	if (arr->clear)
		arr->mass = calloc(1, arr->elemsize);
	else
		arr->mass = malloc(arr->elemsize);

	if (arr->mass == NULL)
	{
		arr->capacity = 0;
		msg_error("couldn't allocate memory for a new buffer of the array!");
		return -1;
	}

	return 0;
}

void array_purge(Array *_arr)
{
	return_if_fail(_arr != NULL);

	ArrayReal *arr = (ArrayReal*) _arr;

	if (arr->mass != NULL)
	{
		free(arr->mass);

		arr->mass = NULL;
		arr->capacity = 0;
		arr->len = 0;
	}
}

int array_steal0(Array *_arr, void *ret, uint *len, bool to_copy)
{
	return_val_if_fail(_arr != NULL, -4);
	return_val_if_fail(ret != NULL, -5);
	
	ArrayReal *arr = (ArrayReal*) _arr;

	if (to_copy)
	{
		memcpy(ret, arr->mass, arr->len * arr->elemsize);
		free(arr->mass);
	}
	else
		*((void**) ret) = arr->mass;

	if (len != NULL)
		*len = arr->len;

	arr->len = 0;
	arr->capacity = 0;
	arr->mass = NULL;

	return 0;
}

void array_free(Array *arr)
{
	return_if_fail(arr != NULL);

	array_purge(arr);
	free(arr);
}

bool array_search(Array *_arr, const void *target, CmpFunc cmp_func, uint *index)
{
	return_val_if_fail(_arr != NULL, FALSE);
	return_val_if_fail(cmp_func != NULL, FALSE);

	ArrayReal *arr = (ArrayReal*) _arr;

	if (arr->len == 0)
		return FALSE;

	if (arr->len < BINARY_SEARCH_LEN_THRESHOLD)
		return linear_search(arr->mass, target, arr->len, arr->elemsize, cmp_func, index);

	if (arr->sorted == 0)
	{
		quicksort(arr->mass, arr->len, arr->elemsize, cmp_func);
		arr->sorted = 1;
	}

	return binary_search(arr->mass, target, arr->len, arr->elemsize, cmp_func, index);
}

Array* array_copy(Array *_dst, const Array *_src)
{
	return_val_if_fail(_src != NULL, NULL);

	bool was_allocated = FALSE;

	if (_dst == NULL)
	{
		_dst = talloc(Array, 1);

		if (_dst == NULL)
		{
			msg_error("couldn't allocate memory for the copy of the array!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	ArrayReal *dst = (ArrayReal*) _dst;
	ArrayReal *src = (ArrayReal*) _src;

	if (src->clear)
		dst->mass = calloc(src->capacity, src->elemsize);
	else
		dst->mass = malloc(src->capacity * src->elemsize);

	if (dst->mass == NULL)
	{
		if (was_allocated)
			free(dst);

		msg_error("couldn't allocate memory for a buffer of the copy of the array!");
		return NULL;
	}

	dst->elemsize = src->elemsize;
	dst->len = src->len;
	dst->capacity = src->capacity;
	dst->sorted = src->sorted;
	dst->zero_terminated = src->zero_terminated;
	dst->clear = src->clear;

	uint zt = dst->zero_terminated;
	memcpy(dst->mass, src->mass, (dst->len + zt) * dst->elemsize);

	return (Array*) dst;
}
