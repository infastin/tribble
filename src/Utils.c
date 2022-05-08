#include "Utils.h"

#include "Macros.h"
#include "Messages.h"
#include "Types.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define SORT_LEN_THRESHOLD 16

char *strdup_printf(const char *fmt, ...)
{
	return_val_if_fail(fmt != NULL, NULL);

	va_list ap, ap_copy;
	va_start(ap, fmt);
	va_copy(ap_copy, ap);

	i32 len = vsnprintf(NULL, 0, fmt, ap_copy);
	char *result = (char *) calloc(sizeof(char), len + 1);
	vsnprintf(result, len + 1, fmt, ap);

	va_end(ap_copy);
	va_end(ap);

	return result;
}

char *strdup_vprintf(const char *fmt, va_list *ap)
{
	return_val_if_fail(fmt != NULL, NULL);
	return_val_if_fail(ap != NULL, NULL);

	va_list ap_copy;
	va_copy(ap_copy, *ap);

	i32 len = vsnprintf(NULL, 0, fmt, ap_copy);
	char *result = (char *) calloc(sizeof(char), len + 1);
	vsnprintf(result, len + 1, fmt, *ap);

	va_end(ap_copy);

	return result;
}

u32 pow2_32(u32 value)
{
	return (1 << (32 - __builtin_clz(value)));
}

u64 pow2_64(u64 value)
{
	return (1 << (64 - __builtin_clzl(value)));
}

static inline void __inssort(void *array, usize len, usize elemsize, CmpFunc cmp_func)
{
	for (i32 i = 1; i < len; ++i) {
		usize cur = i;

		for (usize j = i - 1; j >= 0; --j) {
			if (cmp_func(array_cell(array, elemsize, j), array_cell(array, elemsize, cur)) <= 0)
				break;

			array_swap(array_cell(array, elemsize, j), array_cell(array, elemsize, cur), elemsize);

			cur = j;

			if (j == 0)
				break;
		}
	}
}

void inssort(void *array, usize len, usize elemsize, CmpFunc cmp_func)
{
	return_if_fail(array != NULL);
	return_if_fail(elemsize != 0);
	return_if_fail(cmp_func != NULL);

	__inssort(array, len, elemsize, cmp_func);
}

/* Heapsort */
static inline void heap(void *array, usize start, usize end, usize elemsize, CmpFunc cmp_func)
{
	usize root = start;

	while ((root << 1) < end) {
		usize child = (root << 1) + 1;

		if ((child < end) && cmp_func(array_cell(array, elemsize, child), array_cell(array, elemsize, child + 1)) < 0)
			child++;

		if (cmp_func(array_cell(array, elemsize, root), array_cell(array, elemsize, child)) < 0) {
			array_swap(array_cell(array, elemsize, root), array_cell(array, elemsize, child), elemsize);
			root = child;
		} else
			return;
	}
}

static inline void __heapify(void *array, usize len, usize elemsize, CmpFunc cmp_func)
{
	usize start = (len - 1) >> 1;

	while (start >= 0) {
		heap(array, start, len - 1, elemsize, cmp_func);

		if (start == 0)
			break;

		start--;
	}
}

void heapify(void *array, usize len, usize elemsize, CmpFunc cmp_func)
{
	return_if_fail(array != NULL);
	return_if_fail(elemsize != 0);
	return_if_fail(cmp_func != NULL);

	if (len <= 1)
		return;

	__heapify(array, len, elemsize, cmp_func);
}

static inline void __heapsort(void *array, usize len, usize elemsize, CmpFunc cmp_func)
{
	usize end = len - 1;

	if (len <= 1)
		return;

	__heapify(array, len, elemsize, cmp_func);

	while (end > 0) {
		array_swap(array_cell(array, elemsize, 0), array_cell(array, elemsize, end), elemsize);
		end--;
		heap(array, 0, end, elemsize, cmp_func);
	}
}

void heapsort(void *array, usize len, usize elemsize, CmpFunc cmp_func)
{
	return_if_fail(array != NULL);
	return_if_fail(elemsize != 0);
	return_if_fail(cmp_func != NULL);

	__heapsort(array, len, elemsize, cmp_func);
}

/* Based on Knuth vol. 3 */
static inline usize quicksort_partition(void *array, usize left, usize right, usize pivot, usize elemsize, CmpFunc cmp_func)
{
	usize i = left + 1;
	usize j = right;

	if (pivot != left)
		array_swap(array_cell(array, elemsize, left), array_cell(array, elemsize, pivot), elemsize);

	while (1) {
		while (cmp_func(array_cell(array, elemsize, i), array_cell(array, elemsize, left)) < 0)
			i++;

		while (cmp_func(array_cell(array, elemsize, left), array_cell(array, elemsize, j)) < 0)
			j--;

		if (j <= i) {
			array_swap(array_cell(array, elemsize, j), array_cell(array, elemsize, left), elemsize);
			return j;
		}

		array_swap(array_cell(array, elemsize, i), array_cell(array, elemsize, j), elemsize);

		i++;
		j--;
	}

	return 0;
}

static inline usize find_median(void *array, usize a, usize b, usize c, usize elemsize, CmpFunc cmp_func)
{
	if (cmp_func(array_cell(array, elemsize, a), array_cell(array, elemsize, b)) > 0) {
		if (cmp_func(array_cell(array, elemsize, b), array_cell(array, elemsize, c)) > 0)
			return b;
		else if (cmp_func(array_cell(array, elemsize, a), array_cell(array, elemsize, c)) > 0)
			return c;
		else
			return a;
	} else {
		if (cmp_func(array_cell(array, elemsize, a), array_cell(array, elemsize, c)) > 0)
			return a;
		else if (cmp_func(array_cell(array, elemsize, b), array_cell(array, elemsize, c)) > 0)
			return c;
		else
			return b;
	}
}

static inline void quicksort_recursive(void *array, usize left, usize right, usize elemsize, CmpFunc cmp_func)
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
			__inssort(array_cell(array, elemsize, left), right - left + 1, elemsize, cmp_func);
			return;
		}

		if (++loop_count >= max_loops) {
			__heapsort(array_cell(array, elemsize, left), right - left + 1, elemsize, cmp_func);
			return;
		}

		mid = left + ((right - left) >> 1);
		pivot = find_median(array, left, mid, right, elemsize, cmp_func);
		new_pivot = quicksort_partition(array, left, right, pivot, elemsize, cmp_func);

		if (new_pivot == 0)
			return;

		if ((new_pivot - left - 1) > (right - new_pivot - 1)) {
			quicksort_recursive(array, new_pivot + 1, right, elemsize, cmp_func);
			right = new_pivot - 1;
		} else {
			quicksort_recursive(array, left, new_pivot - 1, elemsize, cmp_func);
			left = new_pivot + 1;
		}
	}
}

void quicksort(void *array, usize len, usize elemsize, CmpFunc cmp_func)
{
	return_if_fail(array != NULL);
	return_if_fail(elemsize != 0);
	return_if_fail(cmp_func != NULL);

	quicksort_recursive(array, 0, len - 1, elemsize, cmp_func);
}

bool linear_search(const void *array, const void *target, usize len, usize elemsize, CmpFunc cmp_func, usize *index)
{
	return_val_if_fail(array != NULL, FALSE);
	return_val_if_fail(elemsize != 0, FALSE);
	return_val_if_fail(cmp_func != NULL, FALSE);

	for (usize i = 0; i < len; ++i) {
		if (cmp_func(array_cell(array, elemsize, i), target) == 0) {
			if (index != NULL)
				*index = i;

			return TRUE;
		}
	}

	return FALSE;
}

bool binary_search(const void *array, const void *target, usize len, usize elemsize, CmpFunc cmp_func, usize *index)
{
	return_val_if_fail(array != NULL, FALSE);
	return_val_if_fail(elemsize != 0, FALSE);
	return_val_if_fail(cmp_func != NULL, FALSE);

	if (len == 0)
		return FALSE;

	usize left = 0;
	usize right = len - 1;

	while (left <= right) {
		usize mid = left + ((right - left) >> 1);

		if (cmp_func(array_cell(array, elemsize, mid), target) == 0) {
			if (index != NULL)
				*index = mid;

			return TRUE;
		}

		if (cmp_func(array_cell(array, elemsize, mid), target) < 0)
			left = mid + 1;
		else
			right = mid - 1;
	}

	return FALSE;
}

i32 u8cmp(const void *a, const void *b)
{
	const u8 *ua = a;
	const u8 *ub = b;

	if (*ua > *ub)
		return 1;

	if (*ua < *ub)
		return -1;

	return 0;
}

i32 u16cmp(const void *a, const void *b)
{
	const u16 *ua = a;
	const u16 *ub = b;

	if (*ua > *ub)
		return 1;

	if (*ua < *ub)
		return -1;

	return 0;
}

i32 u32cmp(const void *a, const void *b)
{
	const u32 *ua = a;
	const u32 *ub = b;

	if (*ua > *ub)
		return 1;

	if (*ua < *ub)
		return -1;

	return 0;
}

i32 u64cmp(const void *a, const void *b)
{
	const u64 *ua = a;
	const u64 *ub = b;

	if (*ua > *ub)
		return 1;

	if (*ua < *ub)
		return -1;

	return 0;
}

i32 usizecmp(const void *a, const void *b)
{
	const usize *ua = a;
	const usize *ub = b;

	if (*ua > *ub)
		return 1;

	if (*ua < *ub)
		return -1;

	return 0;
}

i32 i8cmp(const void *a, const void *b)
{
	const i8 *ia = a;
	const i8 *ib = b;

	if (*ia > *ib)
		return 1;

	if (*ia < *ib)
		return -1;

	return 0;
}

i32 i16cmp(const void *a, const void *b)
{
	const i16 *ia = a;
	const i16 *ib = b;

	if (*ia > *ib)
		return 1;

	if (*ia < *ib)
		return -1;

	return 0;
}

i32 i32cmp(const void *a, const void *b)
{
	const i32 *ia = a;
	const i32 *ib = b;

	if (*ia > *ib)
		return 1;

	if (*ia < *ib)
		return -1;

	return 0;
}

i32 i64cmp(const void *a, const void *b)
{
	const i64 *ia = a;
	const i64 *ib = b;

	if (*ia > *ib)
		return 1;

	if (*ia < *ib)
		return -1;

	return 0;
}

i32 f32cmp(const void *a, const void *b)
{
	const f32 *fa = a;
	const f32 *fb = b;

	if (*fa > *fb)
		return 1;

	if (*fa < *fb)
		return -1;

	return 0;
}

i32 f64cmp(const void *a, const void *b)
{
	const f64 *fa = a;
	const f64 *fb = b;

	if (*fa > *fb)
		return 1;

	if (*fa < *fb)
		return -1;

	return 0;
}

i32 isizecmp(const void *a, const void *b)
{
	const isize *ua = a;
	const isize *ub = b;

	if (*ua > *ub)
		return 1;

	if (*ua < *ub)
		return -1;

	return 0;
}

i32 f128cmp(const void *a, const void *b)
{
	const f128 *fa = a;
	const f128 *fb = b;

	if (*fa > *fb)
		return 1;

	if (*fa < *fb)
		return -1;

	return 0;
}
