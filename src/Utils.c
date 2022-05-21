#include "Utils.h"

#include "Macros.h"
#include "Messages.h"
#include "Types.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define SORT_LEN_THRESHOLD 16

usize trb_strfmt(char **buf, const char *fmt, ...)
{
	trb_return_val_if_fail(buf != NULL, -1);
	trb_return_val_if_fail(fmt != NULL, -1);

	va_list args, args_copy;
	va_start(args, fmt);
	va_copy(args_copy, args);

	i32 len = vsnprintf(NULL, 0, fmt, args_copy);
	if (len < 0)
		return -1;

	usize ulen = len;
	*buf = (char *) calloc(sizeof(char), len + 1);

	if (*buf != NULL) {
		if (vsnprintf(*buf, len + 1, fmt, args) < 0) {
			free(*buf);
			ulen = -1;
		}
	}

	va_end(args_copy);
	va_end(args);

	return ulen;
}

usize trb_vstrfmt(char **buf, const char *fmt, va_list args)
{
	trb_return_val_if_fail(buf != NULL, -1);
	trb_return_val_if_fail(fmt != NULL, -1);

	va_list args_copy;
	va_copy(args_copy, args);

	i32 len = vsnprintf(NULL, 0, fmt, args_copy);
	if (len < 0)
		return -1;

	usize ulen = len;
	*buf = (char *) calloc(sizeof(char), len + 1);

	if (*buf != NULL) {
		if (vsnprintf(*buf, len + 1, fmt, args) < 0) {
			free(*buf);
			ulen = -1;
		}
	}

	va_end(args_copy);
	va_end(args);

	return ulen;
}

char *trb_strdup_fmt(const char *fmt, ...)
{
	trb_return_val_if_fail(fmt != NULL, NULL);

	va_list args, args_copy;
	va_start(args, fmt);
	va_copy(args_copy, args);

	i32 len = vsnprintf(NULL, 0, fmt, args_copy);
	if (len < 0)
		return NULL;

	char *result = (char *) calloc(sizeof(char), len + 1);

	if (result != NULL) {
		if (vsnprintf(result, len + 1, fmt, args) < 0) {
			free(result);
			result = NULL;
		}
	}

	va_end(args_copy);
	va_end(args);

	return result;
}

char *trb_strdup_vfmt(const char *fmt, va_list args)
{
	trb_return_val_if_fail(fmt != NULL, NULL);

	va_list args_copy;
	va_copy(args_copy, args);

	i32 len = vsnprintf(NULL, 0, fmt, args_copy);
	if (len < 0)
		return NULL;

	char *result = (char *) calloc(sizeof(char), len + 1);

	if (result != NULL) {
		if (vsnprintf(result, len + 1, fmt, args) < 0) {
			free(result);
			result = NULL;
		}
	}

	va_end(args_copy);

	return result;
}

u32 trb_pow2_32(u32 value)
{
	return (1 << (32 - __builtin_clz(value)));
}

u64 trb_pow2_64(u64 value)
{
	return (1 << (64 - __builtin_clzl(value)));
}

usize trb_pow2(usize value)
{
#if USIZE_WIDTH == 64
	return trb_pow2_64(value);
#else
	return trb_pow2_32(value);
#endif
}

/* Insertion sort */
static void __inssort(void *array, usize len, usize elemsize, TrbCmpFunc cmp_func)
{
	for (i32 i = 1; i < len; ++i) {
		usize cur = i;

		for (usize j = i - 1; j >= 0; --j) {
			if (cmp_func(trb_array_cell(array, elemsize, j), trb_array_cell(array, elemsize, cur)) <= 0)
				break;

			trb_array_swap(trb_array_cell(array, elemsize, j), trb_array_cell(array, elemsize, cur), elemsize);

			cur = j;

			if (j == 0)
				break;
		}
	}
}

void trb_inssort(void *array, usize len, usize elemsize, TrbCmpFunc cmp_func)
{
	trb_return_if_fail(array != NULL);
	trb_return_if_fail(elemsize != 0);
	trb_return_if_fail(cmp_func != NULL);

	__inssort(array, len, elemsize, cmp_func);
}

/* Insertion sort with data */
static void __inssort_data(void *array, usize len, usize elemsize, TrbCmpDataFunc cmpd_func, void *data)
{
	for (i32 i = 1; i < len; ++i) {
		usize cur = i;

		for (usize j = i - 1; j >= 0; --j) {
			if (cmpd_func(trb_array_cell(array, elemsize, j), trb_array_cell(array, elemsize, cur), data) <= 0)
				break;

			trb_array_swap(trb_array_cell(array, elemsize, j), trb_array_cell(array, elemsize, cur), elemsize);

			cur = j;

			if (j == 0)
				break;
		}
	}
}

void trb_inssort_data(void *array, usize len, usize elemsize, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_if_fail(array != NULL);
	trb_return_if_fail(elemsize != 0);
	trb_return_if_fail(cmpd_func != NULL);

	__inssort_data(array, len, elemsize, cmpd_func, data);
}

/* Heapsort */
static void __heap(void *array, usize start, usize end, usize elemsize, TrbCmpFunc cmp_func)
{
	usize root = start;

	while ((root << 1) < end) {
		usize child = (root << 1) + 1;

		if ((child < end) && cmp_func(trb_array_cell(array, elemsize, child), trb_array_cell(array, elemsize, child + 1)) < 0)
			child++;

		if (cmp_func(trb_array_cell(array, elemsize, root), trb_array_cell(array, elemsize, child)) < 0) {
			trb_array_swap(trb_array_cell(array, elemsize, root), trb_array_cell(array, elemsize, child), elemsize);
			root = child;
		} else
			return;
	}
}

static void __heapify(void *array, usize len, usize elemsize, TrbCmpFunc cmp_func)
{
	usize start = (len - 1) >> 1;

	while (start >= 0) {
		__heap(array, start, len - 1, elemsize, cmp_func);

		if (start == 0)
			break;

		start--;
	}
}

void trb_heapify(void *array, usize len, usize elemsize, TrbCmpFunc cmp_func)
{
	trb_return_if_fail(array != NULL);
	trb_return_if_fail(elemsize != 0);
	trb_return_if_fail(cmp_func != NULL);

	if (len <= 1)
		return;

	__heapify(array, len, elemsize, cmp_func);
}

static void __heapsort(void *array, usize len, usize elemsize, TrbCmpFunc cmp_func)
{
	usize end = len - 1;

	if (len <= 1)
		return;

	__heapify(array, len, elemsize, cmp_func);

	while (end > 0) {
		trb_array_swap(trb_array_cell(array, elemsize, 0), trb_array_cell(array, elemsize, end), elemsize);
		end--;
		__heap(array, 0, end, elemsize, cmp_func);
	}
}

void trb_heapsort(void *array, usize len, usize elemsize, TrbCmpFunc cmp_func)
{
	trb_return_if_fail(array != NULL);
	trb_return_if_fail(elemsize != 0);
	trb_return_if_fail(cmp_func != NULL);

	__heapsort(array, len, elemsize, cmp_func);
}

/* Heapsort with data */
static void __heap_data(void *array, usize start, usize end, usize elemsize, TrbCmpDataFunc cmpd_func, void *data)
{
	usize root = start;

	while ((root << 1) < end) {
		usize child = (root << 1) + 1;

		if ((child < end) && cmpd_func(trb_array_cell(array, elemsize, child), trb_array_cell(array, elemsize, child + 1), data) < 0)
			child++;

		if (cmpd_func(trb_array_cell(array, elemsize, root), trb_array_cell(array, elemsize, child), data) < 0) {
			trb_array_swap(trb_array_cell(array, elemsize, root), trb_array_cell(array, elemsize, child), elemsize);
			root = child;
		} else
			return;
	}
}

static void __heapify_data(void *array, usize len, usize elemsize, TrbCmpDataFunc cmpd_func, void *data)
{
	usize start = (len - 1) >> 1;

	while (start >= 0) {
		__heap_data(array, start, len - 1, elemsize, cmpd_func, data);

		if (start == 0)
			break;

		start--;
	}
}

void trb_heapify_data(void *array, usize len, usize elemsize, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_if_fail(array != NULL);
	trb_return_if_fail(elemsize != 0);
	trb_return_if_fail(cmpd_func != NULL);

	if (len <= 1)
		return;

	__heapify_data(array, len, elemsize, cmpd_func, data);
}

static void __heapsort_data(void *array, usize len, usize elemsize, TrbCmpDataFunc cmpd_func, void *data)
{
	usize end = len - 1;

	if (len <= 1)
		return;

	__heapify_data(array, len, elemsize, cmpd_func, data);

	while (end > 0) {
		trb_array_swap(trb_array_cell(array, elemsize, 0), trb_array_cell(array, elemsize, end), elemsize);
		end--;
		__heap_data(array, 0, end, elemsize, cmpd_func, data);
	}
}

void trb_heapsort_data(void *array, usize len, usize elemsize, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_if_fail(array != NULL);
	trb_return_if_fail(elemsize != 0);
	trb_return_if_fail(cmpd_func != NULL);

	__heapsort_data(array, len, elemsize, cmpd_func, data);
}

/* Based on Knuth vol. 3 */
static usize __quicksort_partition(void *array, usize left, usize right, usize pivot, usize elemsize, TrbCmpFunc cmp_func)
{
	usize i = left + 1;
	usize j = right;

	if (pivot != left)
		trb_array_swap(trb_array_cell(array, elemsize, left), trb_array_cell(array, elemsize, pivot), elemsize);

	while (1) {
		while (cmp_func(trb_array_cell(array, elemsize, i), trb_array_cell(array, elemsize, left)) < 0)
			i++;

		while (cmp_func(trb_array_cell(array, elemsize, left), trb_array_cell(array, elemsize, j)) < 0)
			j--;

		if (j <= i) {
			trb_array_swap(trb_array_cell(array, elemsize, j), trb_array_cell(array, elemsize, left), elemsize);
			return j;
		}

		trb_array_swap(trb_array_cell(array, elemsize, i), trb_array_cell(array, elemsize, j), elemsize);

		i++;
		j--;
	}

	return 0;
}

static usize __find_median(void *array, usize a, usize b, usize c, usize elemsize, TrbCmpFunc cmp_func)
{
	if (cmp_func(trb_array_cell(array, elemsize, a), trb_array_cell(array, elemsize, b)) > 0) {
		if (cmp_func(trb_array_cell(array, elemsize, b), trb_array_cell(array, elemsize, c)) > 0)
			return b;
		else if (cmp_func(trb_array_cell(array, elemsize, a), trb_array_cell(array, elemsize, c)) > 0)
			return c;
		else
			return a;
	} else {
		if (cmp_func(trb_array_cell(array, elemsize, a), trb_array_cell(array, elemsize, c)) > 0)
			return a;
		else if (cmp_func(trb_array_cell(array, elemsize, b), trb_array_cell(array, elemsize, c)) > 0)
			return c;
		else
			return b;
	}
}

static void __quicksort_recursive(void *array, usize left, usize right, usize elemsize, TrbCmpFunc cmp_func)
{
	usize mid;
	usize pivot;
	usize new_pivot;

	i32 loop_count = 0;
	i32 max_loops = 32 - __builtin_clzl(right - left);

	while (1) {
		if (right <= left)
			return;

		if ((right - left + 1) <= SORT_LEN_THRESHOLD) {
			__inssort(trb_array_cell(array, elemsize, left), right - left + 1, elemsize, cmp_func);
			return;
		}

		if (++loop_count >= max_loops) {
			__heapsort(trb_array_cell(array, elemsize, left), right - left + 1, elemsize, cmp_func);
			return;
		}

		mid = left + ((right - left) >> 1);
		pivot = __find_median(array, left, mid, right, elemsize, cmp_func);
		new_pivot = __quicksort_partition(array, left, right, pivot, elemsize, cmp_func);

		if (new_pivot == 0)
			return;

		if ((new_pivot - left - 1) > (right - new_pivot - 1)) {
			__quicksort_recursive(array, new_pivot + 1, right, elemsize, cmp_func);
			right = new_pivot - 1;
		} else {
			__quicksort_recursive(array, left, new_pivot - 1, elemsize, cmp_func);
			left = new_pivot + 1;
		}
	}
}

void trb_quicksort(void *array, usize len, usize elemsize, TrbCmpFunc cmp_func)
{
	trb_return_if_fail(array != NULL);
	trb_return_if_fail(elemsize != 0);
	trb_return_if_fail(cmp_func != NULL);

	__quicksort_recursive(array, 0, len - 1, elemsize, cmp_func);
}

/* Quicksort with data */
static usize __quicksort_partition_data(
	void *array,
	usize left,
	usize right,
	usize pivot,
	usize elemsize,
	TrbCmpDataFunc cmpd_func,
	void *data
)
{
	usize i = left + 1;
	usize j = right;

	if (pivot != left)
		trb_array_swap(trb_array_cell(array, elemsize, left), trb_array_cell(array, elemsize, pivot), elemsize);

	while (1) {
		while (cmpd_func(trb_array_cell(array, elemsize, i), trb_array_cell(array, elemsize, left), data) < 0)
			i++;

		while (cmpd_func(trb_array_cell(array, elemsize, left), trb_array_cell(array, elemsize, j), data) < 0)
			j--;

		if (j <= i) {
			trb_array_swap(trb_array_cell(array, elemsize, j), trb_array_cell(array, elemsize, left), elemsize);
			return j;
		}

		trb_array_swap(trb_array_cell(array, elemsize, i), trb_array_cell(array, elemsize, j), elemsize);

		i++;
		j--;
	}

	return 0;
}

static usize __find_median_data(
	void *array,
	usize a,
	usize b,
	usize c,
	usize elemsize,
	TrbCmpDataFunc cmpd_func,
	void *data
)
{
	if (cmpd_func(trb_array_cell(array, elemsize, a), trb_array_cell(array, elemsize, b), data) > 0) {
		if (cmpd_func(trb_array_cell(array, elemsize, b), trb_array_cell(array, elemsize, c), data) > 0)
			return b;
		else if (cmpd_func(trb_array_cell(array, elemsize, a), trb_array_cell(array, elemsize, c), data) > 0)
			return c;
		else
			return a;
	} else {
		if (cmpd_func(trb_array_cell(array, elemsize, a), trb_array_cell(array, elemsize, c), data) > 0)
			return a;
		else if (cmpd_func(trb_array_cell(array, elemsize, b), trb_array_cell(array, elemsize, c), data) > 0)
			return c;
		else
			return b;
	}
}

static void __quicksort_recursive_data(
	void *array,
	usize left,
	usize right,
	usize elemsize,
	TrbCmpDataFunc cmpd_func,
	void *data
)
{
	usize mid;
	usize pivot;
	usize new_pivot;

	i32 loop_count = 0;
	i32 max_loops = 32 - __builtin_clzl(right - left);

	while (1) {
		if (right <= left)
			return;

		if ((right - left + 1) <= SORT_LEN_THRESHOLD) {
			__inssort_data(trb_array_cell(array, elemsize, left), right - left + 1, elemsize, cmpd_func, data);
			return;
		}

		if (++loop_count >= max_loops) {
			__heapsort_data(trb_array_cell(array, elemsize, left), right - left + 1, elemsize, cmpd_func, data);
			return;
		}

		mid = left + ((right - left) >> 1);
		pivot = __find_median_data(array, left, mid, right, elemsize, cmpd_func, data);
		new_pivot = __quicksort_partition_data(array, left, right, pivot, elemsize, cmpd_func, data);

		if (new_pivot == 0)
			return;

		if ((new_pivot - left - 1) > (right - new_pivot - 1)) {
			__quicksort_recursive_data(array, new_pivot + 1, right, elemsize, cmpd_func, data);
			right = new_pivot - 1;
		} else {
			__quicksort_recursive_data(array, left, new_pivot - 1, elemsize, cmpd_func, data);
			left = new_pivot + 1;
		}
	}
}

void trb_quicksort_data(void *array, usize len, usize elemsize, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_if_fail(array != NULL);
	trb_return_if_fail(elemsize != 0);
	trb_return_if_fail(cmpd_func != NULL);

	__quicksort_recursive_data(array, 0, len - 1, elemsize, cmpd_func, data);
}

/* Binary search */
bool trb_binary_search(const void *array, const void *target, usize len, usize elemsize, TrbCmpFunc cmp_func, usize *index)
{
	trb_return_val_if_fail(array != NULL, FALSE);
	trb_return_val_if_fail(elemsize != 0, FALSE);
	trb_return_val_if_fail(cmp_func != NULL, FALSE);

	if (len == 0)
		return FALSE;

	usize left = 0;
	usize right = len - 1;

	while (left <= right) {
		usize mid = left + ((right - left) >> 1);

		if (cmp_func(trb_array_cell(array, elemsize, mid), target) == 0) {
			if (index != NULL)
				*index = mid;

			return TRUE;
		}

		if (cmp_func(trb_array_cell(array, elemsize, mid), target) < 0)
			left = mid + 1;
		else
			right = mid - 1;
	}

	return FALSE;
}

bool trb_binary_search_data(
	const void *array,
	const void *target,
	usize len,
	usize elemsize,
	TrbCmpDataFunc cmpd_func,
	void *data,
	usize *index
)
{
	trb_return_val_if_fail(array != NULL, FALSE);
	trb_return_val_if_fail(elemsize != 0, FALSE);
	trb_return_val_if_fail(cmpd_func != NULL, FALSE);

	if (len == 0)
		return FALSE;

	usize left = 0;
	usize right = len - 1;

	while (left <= right) {
		usize mid = left + ((right - left) >> 1);

		if (cmpd_func(trb_array_cell(array, elemsize, mid), target, data) == 0) {
			if (index != NULL)
				*index = mid;

			return TRUE;
		}

		if (cmpd_func(trb_array_cell(array, elemsize, mid), target, data) < 0)
			left = mid + 1;
		else
			right = mid - 1;
	}

	return FALSE;
}

/* Compare functions */
i32 trb_u8cmp(const u8 *a, const u8 *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}

i32 trb_u16cmp(const u16 *a, const u16 *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}

i32 trb_u32cmp(const u32 *a, const u32 *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}

i32 trb_u64cmp(const u64 *a, const u64 *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}

i32 trb_usizecmp(const usize *a, const usize *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}

i32 trb_i8cmp(const i8 *a, const i8 *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}

i32 trb_i16cmp(const i16 *a, const i16 *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}

i32 trb_i32cmp(const i32 *a, const i32 *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}

i32 trb_i64cmp(const i64 *a, const i64 *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}

i32 trb_isizecmp(const isize *a, const isize *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}

i32 trb_f32cmp(const f32 *a, const f32 *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}

i32 trb_f64cmp(const f64 *a, const f64 *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}

i32 trb_realcmp(const real *a, const real *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
}
