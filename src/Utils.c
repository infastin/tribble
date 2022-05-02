#include "Utils.h"

#include "Macros.h"
#include "Types.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define SORT_LEN_THRESHOLD 16

char *strdup_printf(const char *fmt, ...)
{
	va_list ap, ap_copy;
	va_start(ap, fmt);
	va_copy(ap_copy, ap);

	int32_t len = vsnprintf(NULL, 0, fmt, ap_copy);
	char *result = (char *) calloc(sizeof(char), len + 1);
	vsnprintf(result, len + 1, fmt, ap);

	va_end(ap_copy);
	va_end(ap);

	return result;
}

char *strdup_vprintf(const char *fmt, va_list *ap)
{
	va_list ap_copy;
	va_copy(ap_copy, *ap);

	int32_t len = vsnprintf(NULL, 0, fmt, ap_copy);
	char *result = (char *) calloc(sizeof(char), len + 1);
	vsnprintf(result, len + 1, fmt, *ap);

	va_end(ap_copy);

	return result;
}

uint32_t pow2_32(uint32_t value)
{
	return (1 << (32 - __builtin_clz(value)));
}

uint64_t pow2_64(uint64_t value)
{
	return (1 << (64 - __builtin_clzl(value)));
}

void inssort(void *mass, uint32_t len, uint32_t elemsize, CmpFunc cmp_func)
{
	for (int32_t i = 1; i < len; ++i) {
		uint32_t cur = i;

		for (uint32_t j = i - 1; j >= 0; --j) {
			if (cmp_func(mass_cell(mass, elemsize, j), mass_cell(mass, elemsize, cur)) <= 0)
				break;

			mass_swap(mass_cell(mass, elemsize, j), mass_cell(mass, elemsize, cur), elemsize);

			cur = j;

			if (j == 0)
				break;
		}
	}
}

/* Heapsort */
static inline void heap(void *mass, uint32_t start, uint32_t end, uint32_t elemsize, CmpFunc cmp_func)
{
	uint32_t root = start;

	while ((root << 1) < end) {
		uint32_t child = (root << 1) + 1;

		if ((child < end) && cmp_func(mass_cell(mass, elemsize, child), mass_cell(mass, elemsize, child + 1)) < 0)
			child++;

		if (cmp_func(mass_cell(mass, elemsize, root), mass_cell(mass, elemsize, child)) < 0) {
			mass_swap(mass_cell(mass, elemsize, root), mass_cell(mass, elemsize, child), elemsize);
			root = child;
		} else
			return;
	}
}

static inline void heapify(void *mass, uint32_t len, uint32_t elemsize, CmpFunc cmp_func)
{
	uint32_t start = (len - 1) >> 1;

	while (start >= 0) {
		heap(mass, start, len - 1, elemsize, cmp_func);

		if (start == 0)
			break;

		start--;
	}
}

void heapsort(void *mass, uint32_t len, uint32_t elemsize, CmpFunc cmp_func)
{
	uint32_t end = len - 1;

	if (len <= 1)
		return;

	heapify(mass, len, elemsize, cmp_func);

	while (end > 0) {
		mass_swap(mass_cell(mass, elemsize, 0), mass_cell(mass, elemsize, end), elemsize);
		end--;
		heap(mass, 0, end, elemsize, cmp_func);
	}
}

/* Based on Knuth vol. 3 */
static inline uint32_t quicksort_partition(void *mass, uint32_t left, uint32_t right, uint32_t pivot, uint32_t elemsize, CmpFunc cmp_func)
{
	uint32_t i = left + 1;
	uint32_t j = right;

	if (pivot != left)
		mass_swap(mass_cell(mass, elemsize, left), mass_cell(mass, elemsize, pivot), elemsize);

	while (1) {
		while (cmp_func(mass_cell(mass, elemsize, i), mass_cell(mass, elemsize, left)) < 0)
			i++;

		while (cmp_func(mass_cell(mass, elemsize, left), mass_cell(mass, elemsize, j)) < 0)
			j--;

		if (j <= i) {
			mass_swap(mass_cell(mass, elemsize, j), mass_cell(mass, elemsize, left), elemsize);
			return j;
		}

		mass_swap(mass_cell(mass, elemsize, i), mass_cell(mass, elemsize, j), elemsize);

		i++;
		j--;
	}

	return 0;
}

static inline uint32_t find_median(void *mass, uint32_t a, uint32_t b, uint32_t c, uint32_t elemsize, CmpFunc cmp_func)
{
	if (cmp_func(mass_cell(mass, elemsize, a), mass_cell(mass, elemsize, b)) > 0) {
		if (cmp_func(mass_cell(mass, elemsize, b), mass_cell(mass, elemsize, c)) > 0)
			return b;
		else if (cmp_func(mass_cell(mass, elemsize, a), mass_cell(mass, elemsize, c)) > 0)
			return c;
		else
			return a;
	} else {
		if (cmp_func(mass_cell(mass, elemsize, a), mass_cell(mass, elemsize, c)) > 0)
			return a;
		else if (cmp_func(mass_cell(mass, elemsize, b), mass_cell(mass, elemsize, c)) > 0)
			return c;
		else
			return b;
	}
}

static inline void quicksort_recursive(void *mass, uint32_t left, uint32_t right, uint32_t elemsize, CmpFunc cmp_func)
{
	uint32_t mid;
	uint32_t pivot;
	uint32_t new_pivot;

	int32_t loop_count = 0;
	int32_t max_loops = 32 - __builtin_clzl(right - left);

	while (1) {
		if (right <= left)
			return;

		if ((right - left + 1) <= SORT_LEN_THRESHOLD) {
			inssort(mass_cell(mass, elemsize, left), right - left + 1, elemsize, cmp_func);
			return;
		}

		if (++loop_count >= max_loops) {
			heapsort(mass_cell(mass, elemsize, left), right - left + 1, elemsize, cmp_func);
			return;
		}

		mid = left + ((right - left) >> 1);
		pivot = find_median(mass, left, mid, right, elemsize, cmp_func);
		new_pivot = quicksort_partition(mass, left, right, pivot, elemsize, cmp_func);

		if (new_pivot == 0)
			return;

		if ((new_pivot - left - 1) > (right - new_pivot - 1)) {
			quicksort_recursive(mass, new_pivot + 1, right, elemsize, cmp_func);
			right = new_pivot - 1;
		} else {
			quicksort_recursive(mass, left, new_pivot - 1, elemsize, cmp_func);
			left = new_pivot + 1;
		}
	}
}

void quicksort(void *mass, uint32_t len, uint32_t elemsize, CmpFunc cmp_func)
{
	quicksort_recursive(mass, 0, len - 1, elemsize, cmp_func);
}

bool linear_search(void *mass, const void *target, uint32_t len, uint32_t elemsize, CmpFunc cmp_func, uint32_t *index)
{
	for (uint32_t i = 0; i < len; ++i) {
		if (cmp_func(mass_cell(mass, elemsize, i), target) == 0) {
			if (index != NULL)
				*index = i;

			return TRUE;
		}
	}

	return FALSE;
}

bool binary_search(void *mass, const void *target, uint32_t len, uint32_t elemsize, CmpFunc cmp_func, uint32_t *index)
{
	if (len == 0)
		return FALSE;

	uint32_t left = 0;
	uint32_t right = len - 1;

	while (left <= right) {
		uint32_t mid = left + ((right - left) >> 1);

		if (cmp_func(mass_cell(mass, elemsize, mid), target) == 0) {
			if (index != NULL)
				*index = mid;

			return TRUE;
		}

		if (cmp_func(mass_cell(mass, elemsize, mid), target) < 0)
			left = mid + 1;
		else
			right = mid - 1;
	}

	return FALSE;
}
