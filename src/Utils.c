#include "Utils.h"

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

/* Insertion sort */
static void __trb_inssort(TrbSlice *slice, TrbCmpFunc cmp_func)
{
	usize len = trb_slice_len(slice);

	for (usize i = 1; i < len; ++i) {
		usize cur = i;

		for (usize j = i - 1;; --j) {
			if (cmp_func(slice->at(slice, j), slice->at(slice, cur)) <= 0)
				break;

			trb_memswap(slice->at(slice, j), slice->at(slice, cur), slice->elemsize);

			cur = j;

			if (j == 0)
				break;
		}
	}
}

void trb_inssort(TrbSlice *slice, TrbCmpFunc cmp_func)
{
	trb_return_if_fail(slice != NULL);
	trb_return_if_fail(cmp_func != NULL);

	usize len = trb_slice_len(slice);
	if (len <= 1)
		return;

	__trb_inssort(slice, cmp_func);
}

/* Insertion sort with data */
static void __trb_inssort_data(TrbSlice *slice, TrbCmpDataFunc cmpd_func, void *data)
{
	usize len = trb_slice_len(slice);

	for (usize i = 1; i < len; ++i) {
		usize cur = i;

		for (usize j = i - 1;; --j) {
			if (cmpd_func(slice->at(slice, j), slice->at(slice, cur), data) <= 0)
				break;

			trb_memswap(slice->at(slice, j), slice->at(slice, cur), slice->elemsize);

			cur = j;

			if (j == 0)
				break;
		}
	}
}

void trb_inssort_data(TrbSlice *slice, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_if_fail(slice != NULL);
	trb_return_if_fail(cmpd_func != NULL);

	usize len = trb_slice_len(slice);
	if (len <= 1)
		return;

	__trb_inssort_data(slice, cmpd_func, data);
}

/* Heapsort */
static void __trb_heap(TrbSlice *slice, usize start, usize end, TrbCmpFunc cmp_func)
{
	usize root = start;

	while ((root << 1) < end) {
		usize child = (root << 1) + 1;

		if ((child < end) && cmp_func(slice->at(slice, child), slice->at(slice, child + 1)) < 0)
			child++;

		if (cmp_func(slice->at(slice, root), slice->at(slice, child)) < 0) {
			trb_memswap(slice->at(slice, root), slice->at(slice, child), slice->elemsize);
			root = child;
		} else
			return;
	}
}

static void __trb_heapify(TrbSlice *slice, TrbCmpFunc cmp_func)
{
	usize len = trb_slice_len(slice);
	usize start = (len - 1) >> 1;

	while (1) {
		__trb_heap(slice, start, len - 1, cmp_func);

		if (start == 0)
			break;

		start--;
	}
}

void trb_heapify(TrbSlice *slice, TrbCmpFunc cmp_func)
{
	trb_return_if_fail(slice != NULL);
	trb_return_if_fail(cmp_func != NULL);

	usize len = trb_slice_len(slice);
	if (len <= 1)
		return;

	__trb_heapify(slice, cmp_func);
}

static void __trb_heapsort(TrbSlice *slice, TrbCmpFunc cmp_func)
{
	usize len = trb_slice_len(slice);
	usize end = len - 1;

	__trb_heapify(slice, cmp_func);

	while (end > 0) {
		trb_memswap(slice->at(slice, 0), slice->at(slice, end), slice->elemsize);
		end--;
		__trb_heap(slice, 0, end, cmp_func);
	}
}

void trb_heapsort(TrbSlice *slice, TrbCmpFunc cmp_func)
{
	trb_return_if_fail(slice != NULL);
	trb_return_if_fail(cmp_func != NULL);

	usize len = trb_slice_len(slice);
	if (len <= 1)
		return;

	__trb_heapsort(slice, cmp_func);
}

/* Heapsort with data */
static void __trb_heap_data(TrbSlice *slice, usize start, usize end, TrbCmpDataFunc cmpd_func, void *data)
{
	usize root = start;

	while ((root << 1) < end) {
		usize child = (root << 1) + 1;

		if ((child < end) && cmpd_func(slice->at(slice, child), slice->at(slice, child + 1), data) < 0)
			child++;

		if (cmpd_func(slice->at(slice, root), slice->at(slice, child), data) < 0) {
			trb_memswap(slice->at(slice, root), slice->at(slice, child), slice->elemsize);
			root = child;
		} else
			return;
	}
}

static void __trb_heapify_data(TrbSlice *slice, TrbCmpDataFunc cmpd_func, void *data)
{
	usize len = trb_slice_len(slice);
	usize start = (len - 1) >> 1;

	while (1) {
		__trb_heap_data(slice, start, len - 1, cmpd_func, data);

		if (start == 0)
			break;

		start--;
	}
}

void trb_heapify_data(TrbSlice *slice, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_if_fail(slice != NULL);
	trb_return_if_fail(cmpd_func != NULL);

	usize len = trb_slice_len(slice);
	if (len <= 1)
		return;

	__trb_heapify_data(slice, cmpd_func, data);
}

static void __trb_heapsort_data(TrbSlice *slice, TrbCmpDataFunc cmpd_func, void *data)
{
	usize len = trb_slice_len(slice);
	usize end = len - 1;

	__trb_heapify_data(slice, cmpd_func, data);

	while (end > 0) {
		trb_memswap(slice->at(slice, 0), slice->at(slice, end), slice->elemsize);
		end--;
		__trb_heap_data(slice, 0, end, cmpd_func, data);
	}
}

void trb_heapsort_data(TrbSlice *slice, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_if_fail(slice != NULL);
	trb_return_if_fail(cmpd_func != NULL);

	usize len = trb_slice_len(slice);
	if (len <= 1)
		return;

	__trb_heapsort_data(slice, cmpd_func, data);
}

/* Based on Knuth vol. 3 */
static usize __trb_quicksort_partition(TrbSlice *slice, usize left, usize right, usize pivot, TrbCmpFunc cmp_func)
{
	usize i = left + 1;
	usize j = right;

	if (pivot != left)
		trb_memswap(slice->at(slice, left), slice->at(slice, pivot), slice->elemsize);

	while (1) {
		while (cmp_func(slice->at(slice, i), slice->at(slice, left)) < 0)
			i++;

		while (cmp_func(slice->at(slice, left), slice->at(slice, j)) < 0)
			j--;

		if (j <= i) {
			trb_memswap(slice->at(slice, j), slice->at(slice, left), slice->elemsize);
			return j;
		}

		trb_memswap(slice->at(slice, i), slice->at(slice, j), slice->elemsize);

		i++;
		j--;
	}

	return 0;
}

static usize __trb_find_median(TrbSlice *slice, usize a, usize b, usize c, TrbCmpFunc cmp_func)
{
	if (cmp_func(slice->at(slice, a), slice->at(slice, b)) > 0) {
		if (cmp_func(slice->at(slice, b), slice->at(slice, c)) > 0)
			return b;
		else if (cmp_func(slice->at(slice, a), slice->at(slice, c)) > 0)
			return c;
		else
			return a;
	} else {
		if (cmp_func(slice->at(slice, a), slice->at(slice, c)) > 0)
			return a;
		else if (cmp_func(slice->at(slice, b), slice->at(slice, c)) > 0)
			return c;
		else
			return b;
	}
}

static void __trb_quicksort_recursive(TrbSlice *slice, usize left, usize right, TrbCmpFunc cmp_func)
{
	usize mid;
	usize pivot;
	usize new_pivot;

	usize loop_count = 0;
	usize max_loops = 32 - __builtin_clzl(right - left);

	while (1) {
		if (right <= left)
			return;

		if ((right - left + 1) <= SORT_LEN_THRESHOLD) {
			TrbSlice part_slice;
			trb_slice_reslice(slice, &part_slice, left, right + 1);
			__trb_inssort(&part_slice, cmp_func);
			return;
		}

		if (++loop_count >= max_loops) {
			TrbSlice part_slice;
			trb_slice_reslice(slice, &part_slice, left, right + 1);
			__trb_heapsort(&part_slice, cmp_func);
			return;
		}

		mid = left + ((right - left) >> 1);
		pivot = __trb_find_median(slice, left, mid, right, cmp_func);
		new_pivot = __trb_quicksort_partition(slice, left, right, pivot, cmp_func);

		if (new_pivot == 0)
			return;

		if ((new_pivot - left - 1) > (right - new_pivot - 1)) {
			__trb_quicksort_recursive(slice, new_pivot + 1, right, cmp_func);
			right = new_pivot - 1;
		} else {
			__trb_quicksort_recursive(slice, left, new_pivot - 1, cmp_func);
			left = new_pivot + 1;
		}
	}
}

void trb_quicksort(TrbSlice *slice, TrbCmpFunc cmp_func)
{
	trb_return_if_fail(slice != NULL);
	trb_return_if_fail(cmp_func != NULL);

	usize len = trb_slice_len(slice);
	if (len <= 1)
		return;

	__trb_quicksort_recursive(slice, 0, len - 1, cmp_func);
}

/* Quicksort with data */
static usize __trb_quicksort_partition_data(
	TrbSlice *slice,
	usize left,
	usize right,
	usize pivot,
	TrbCmpDataFunc cmpd_func,
	void *data
)
{
	usize i = left + 1;
	usize j = right;

	if (pivot != left)
		trb_memswap(slice->at(slice, left), slice->at(slice, pivot), slice->elemsize);

	while (1) {
		while (cmpd_func(slice->at(slice, i), slice->at(slice, left), data) < 0)
			i++;

		while (cmpd_func(slice->at(slice, left), slice->at(slice, j), data) < 0)
			j--;

		if (j <= i) {
			trb_memswap(slice->at(slice, j), slice->at(slice, left), slice->elemsize);
			return j;
		}

		trb_memswap(slice->at(slice, i), slice->at(slice, j), slice->elemsize);

		i++;
		j--;
	}

	return 0;
}

static usize __trb_find_median_data(TrbSlice *slice, usize a, usize b, usize c, TrbCmpDataFunc cmpd_func, void *data)
{
	if (cmpd_func(slice->at(slice, a), slice->at(slice, b), data) > 0) {
		if (cmpd_func(slice->at(slice, b), slice->at(slice, c), data) > 0)
			return b;
		else if (cmpd_func(slice->at(slice, a), slice->at(slice, c), data) > 0)
			return c;
		else
			return a;
	} else {
		if (cmpd_func(slice->at(slice, a), slice->at(slice, c), data) > 0)
			return a;
		else if (cmpd_func(slice->at(slice, b), slice->at(slice, c), data) > 0)
			return c;
		else
			return b;
	}
}

static void __trb_quicksort_recursive_data(TrbSlice *slice, usize left, usize right, TrbCmpDataFunc cmpd_func, void *data)
{
	usize mid;
	usize pivot;
	usize new_pivot;

	usize loop_count = 0;
	usize max_loops = 32 - __builtin_clzl(right - left);

	while (1) {
		if (right <= left)
			return;

		if ((right - left + 1) <= SORT_LEN_THRESHOLD) {
			TrbSlice part_slice;
			trb_slice_reslice(slice, &part_slice, left, right + 1);
			__trb_inssort_data(&part_slice, cmpd_func, data);
			return;
		}

		if (++loop_count >= max_loops) {
			TrbSlice part_slice;
			trb_slice_reslice(slice, &part_slice, left, right + 1);
			__trb_heapsort_data(&part_slice, cmpd_func, data);
			return;
		}

		mid = left + ((right - left) >> 1);
		pivot = __trb_find_median_data(slice, left, mid, right, cmpd_func, data);
		new_pivot = __trb_quicksort_partition_data(slice, left, right, pivot, cmpd_func, data);

		if (new_pivot == 0)
			return;

		if ((new_pivot - left - 1) > (right - new_pivot - 1)) {
			__trb_quicksort_recursive_data(slice, new_pivot + 1, right, cmpd_func, data);
			right = new_pivot - 1;
		} else {
			__trb_quicksort_recursive_data(slice, left, new_pivot - 1, cmpd_func, data);
			left = new_pivot + 1;
		}
	}
}

void trb_quicksort_data(TrbSlice *slice, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_if_fail(slice != NULL);
	trb_return_if_fail(cmpd_func != NULL);

	usize len = trb_slice_len(slice);
	if (len <= 1)
		return;

	__trb_quicksort_recursive_data(slice, 0, len - 1, cmpd_func, data);
}

void trb_reverse(TrbSlice *slice)
{
	trb_return_if_fail(slice != NULL);

	usize len = trb_slice_len(slice);
	if (len <= 1)
		return;

	for (usize lo = 0, hi = len - 1; lo < hi; ++lo, --hi) {
		trb_memswap(slice->at(slice, lo), slice->at(slice, hi), slice->elemsize);
	}
}

/* Binary search */
bool trb_binary_search(const TrbSlice *slice, const void *target, TrbCmpFunc cmp_func, usize *index)
{
	trb_return_val_if_fail(slice != NULL, FALSE);
	trb_return_val_if_fail(cmp_func != NULL, FALSE);

	usize len = trb_slice_len(slice);

	if (len == 0)
		return FALSE;

	usize left = 0;
	usize right = len - 1;

	while (left <= right) {
		usize mid = left + ((right - left) >> 1);

		if (cmp_func(slice->at(slice, mid), target) == 0) {
			if (index != NULL)
				*index = mid;

			return TRUE;
		}

		if (cmp_func(slice->at(slice, mid), target) < 0)
			left = mid + 1;
		else
			right = mid - 1;
	}

	return FALSE;
}

bool trb_binary_search_data(const TrbSlice *slice, const void *target, TrbCmpDataFunc cmpd_func, void *data, usize *index)
{
	trb_return_val_if_fail(slice != NULL, FALSE);
	trb_return_val_if_fail(cmpd_func != NULL, FALSE);

	usize len = trb_slice_len(slice);

	if (len == 0)
		return FALSE;

	usize left = 0;
	usize right = len - 1;

	while (left <= right) {
		usize mid = left + ((right - left) >> 1);

		if (cmpd_func(slice->at(slice, mid), target, data) == 0) {
			if (index != NULL)
				*index = mid;

			return TRUE;
		}

		if (cmpd_func(slice->at(slice, mid), target, data) < 0)
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

i32 trb_f128cmp(const f128 *a, const f128 *b)
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
