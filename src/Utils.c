#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "Utils.h"
#include "Types.h"
#include "Macros.h"

#define SORT_LEN_THRESHOLD 16

char* strdup_printf(const char *fmt, ...)
{
	va_list ap, ap_copy;
	va_start(ap, fmt);
	va_copy(ap_copy, ap);


	int len = vsnprintf(NULL, 0, fmt, ap_copy);
	char *result = (char*)calloc(sizeof(char), len + 1);
	vsnprintf(result, len + 1, fmt, ap);

	va_end(ap_copy);
	va_end(ap);

	return result;
}

char* strdup_vprintf(const char *fmt, va_list *ap)
{
	va_list ap_copy;
	va_copy(ap_copy, *ap);


	int len = vsnprintf(NULL, 0, fmt, ap_copy);
	char *result = (char*)calloc(sizeof(char), len + 1);
	vsnprintf(result, len + 1, fmt, *ap);

	va_end(ap_copy);

	return result;
}

uint pow2(uint value)
{
	return (1 << (UINT_BIT - __builtin_clz(value)));
}

ulong pow2l(ulong value)
{
	return (1 << (ULONG_BIT - __builtin_clzl(value)));
}

ullong pow2ll(ullong value)
{
	return (1 << (ULLONG_BIT - __builtin_clzl(value)));
}

void inssort(void *mass, uint len, uint elemsize, CmpFunc cmp_func)
{
	for (int i = 1; i < len; ++i) 
	{
		uint cur = i;

		for (uint j = i - 1; j >= 0; --j) 
		{
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
static inline void heap(void *mass, uint start, uint end, uint elemsize, CmpFunc cmp_func)
{
	uint root = start;

	while ((root << 1) < end)
	{
		uint child = (root << 1) + 1;

		if ((child < end) && cmp_func(mass_cell(mass, elemsize, child), mass_cell(mass, elemsize, child + 1)) < 0)
			child++;

		if (cmp_func(mass_cell(mass, elemsize, root), mass_cell(mass, elemsize, child)) < 0)
		{
			mass_swap(mass_cell(mass, elemsize, root), mass_cell(mass, elemsize, child), elemsize);
			root = child;
		}
		else
			return;
	}
}

static inline void heapify(void *mass, uint len, uint elemsize, CmpFunc cmp_func)
{
	uint start = (len - 1) >> 1;

	while (start >= 0) 
	{
		heap(mass, start, len - 1, elemsize, cmp_func);

		if (start == 0)
			break;

		start--;
	}
}

void heapsort(void *mass, uint len, uint elemsize, CmpFunc cmp_func)
{
	uint end = len - 1;

	if (len <= 1)
		return;

	heapify(mass, len, elemsize, cmp_func);

	while (end > 0)
	{
		mass_swap(mass_cell(mass, elemsize, 0), mass_cell(mass, elemsize, end), elemsize);
		end--;
		heap(mass, 0, end, elemsize, cmp_func);
	}
}

/* Based on Knuth vol. 3 */
static inline uint quicksort_partition(void *mass, uint left, uint right, uint pivot, uint elemsize, CmpFunc cmp_func)
{
	uint i = left + 1;
	uint j = right;

	if (pivot != left)
		mass_swap(mass_cell(mass, elemsize, left), mass_cell(mass, elemsize, pivot), elemsize);

	while (1) 
	{
		while (cmp_func(mass_cell(mass, elemsize, i), mass_cell(mass, elemsize, left)) < 0)
			i++;

		while (cmp_func(mass_cell(mass, elemsize, left), mass_cell(mass, elemsize, j)) < 0)
			j--;

		if (j <= i)
		{
			mass_swap(mass_cell(mass, elemsize, j), mass_cell(mass, elemsize, left), elemsize);
			return j;
		}

		mass_swap(mass_cell(mass, elemsize, i), mass_cell(mass, elemsize, j), elemsize);

		i++;
		j--;
	}

	return 0;
}

static inline uint find_median(void *mass, uint a, uint b, uint c, uint elemsize, CmpFunc cmp_func)
{
	if (cmp_func(mass_cell(mass, elemsize, a), mass_cell(mass, elemsize, b)) > 0)
	{
		if (cmp_func(mass_cell(mass, elemsize, b), mass_cell(mass, elemsize, c)) > 0)
			return b;
		else if (cmp_func(mass_cell(mass, elemsize, a), mass_cell(mass, elemsize, c)) > 0)
			return c;
		else
			return a;
	}
	else
	{
		if (cmp_func(mass_cell(mass, elemsize, a), mass_cell(mass, elemsize, c)) > 0)
			return a;
		else if (cmp_func(mass_cell(mass, elemsize, b), mass_cell(mass, elemsize, c)) > 0)
			return c;
		else 
			return b;
	}
}

static inline void quicksort_recursive(void *mass, uint left, uint right, uint elemsize, CmpFunc cmp_func)
{
	uint mid;
	uint pivot;
	uint new_pivot;

	int loop_count = 0;
	int max_loops = ULONG_BIT - __builtin_clzl(right - left);

	while (1) 
	{
		if (right <= left)
			return;

		if ((right - left + 1) <= SORT_LEN_THRESHOLD)
		{
			inssort(mass_cell(mass, elemsize, left), right - left + 1, elemsize, cmp_func);
			return;
		}

		if (++loop_count >= max_loops)
		{
			heapsort(mass_cell(mass, elemsize, left), right - left + 1, elemsize, cmp_func);
			return;
		}

		mid = left + ((right - left) >> 1);
		pivot = find_median(mass, left, mid, right, elemsize, cmp_func);
		new_pivot = quicksort_partition(mass, left, right, pivot, elemsize, cmp_func);

		if (new_pivot == 0)
			return;

		if ((new_pivot - left - 1) > (right - new_pivot - 1))
		{
			quicksort_recursive(mass, new_pivot + 1, right, elemsize, cmp_func);
			right = new_pivot - 1;
		}
		else 
		{
			quicksort_recursive(mass, left, new_pivot - 1, elemsize, cmp_func);
			left = new_pivot + 1;
		}
	}
}

void quicksort(void *mass, uint len, uint elemsize, CmpFunc cmp_func)
{
	quicksort_recursive(mass, 0, len - 1, elemsize, cmp_func);
}

bool linear_search(void *mass, const void *target, uint len, uint elemsize, CmpFunc cmp_func, uint *index)
{
	for (uint i = 0; i < len; ++i) 
	{
		if (cmp_func(mass_cell(mass, elemsize, i), target) == 0)
		{
			if (index != NULL)
				*index = i;

			return TRUE;
		}
	}

	return FALSE;
}

bool binary_search(void *mass, const void *target, uint len, uint elemsize, CmpFunc cmp_func, uint *index)
{
	if (len == 0)
		return FALSE;

	uint left = 0;
	uint right = len - 1;

	while (left <= right) 
	{
		uint mid = left + ((right - left) >> 1);

		if (cmp_func(mass_cell(mass, elemsize, mid), target) == 0)
		{
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
