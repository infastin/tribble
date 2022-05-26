#ifndef UTILS_H_ATMQZMHO
#define UTILS_H_ATMQZMHO

#include "Macros.h"
#include "Types.h"

#include <stdarg.h>

/**
 * trb_strfmt:
 * @buf: The buffer where the formatted string is to be placed.
 * @fmt: The format of the string.
 * @...: Arguments.
 *
 * It is analogous to `snprintf()`, except it allocates a string large enough to
 * hold the output including terminating null byte.
 *
 * Returns: The length of the formatted string. Can return -1 if an error occurs.
 **/
usize trb_strfmt(char **buf, const char *fmt, ...) TRB_FORMAT(printf, 2, 3);

/**
 * trb_vstrfmt:
 * @buf: The buffer where the formatted string is to be placed.
 * @fmt: The format of the string.
 * @args: The argument list.
 *
 * It is analogous to `vsnprintf()`, except it allocates a string large enough to
 * hold the output including terminating null byte.
 *
 * Returns: The length of the formatted string. Can return -1 if an error occurs.
 **/
usize trb_vstrfmt(char **buf, const char *fmt, va_list args) TRB_FORMAT(printf, 2, 0);

/**
 * trb_strdup_fmt:
 * @fmt: The format of the string.
 * @...: Arguments.
 *
 * It is analogous to `snprintf()`, except it allocates a string large enough to
 * hold the output including terminating null byte.
 *
 * Returns: The string with terminating null byte. Can return %NULL if an error occurs.
 **/
char *trb_strdup_fmt(const char *fmt, ...) TRB_FORMAT(printf, 1, 2);

/**
 * trb_strdup_vfmt:
 * @fmt: The format of the string.
 * @args: The argument list.
 *
 * It is analogous to `vsnprintf()`, except it allocates a string large enough to
 * hold the output including terminating null byte.
 *
 * Returns: The string with terminating null byte. Can return %NULL if error occurs.
 **/
char *trb_strdup_vfmt(const char *fmt, va_list args) TRB_FORMAT(printf, 1, 0);

/**
 * trb_heapify:
 * @array: The array to be heapified.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmp_func: (scope call): The function for comparing elements.
 *
 * Heapifies the array.
 **/
void trb_heapify(void *array, usize len, usize elemsize, TrbCmpFunc cmp_func);

/**
 * trb_heapify_data:
 * @array: The array to be heapified.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmpd_func: (scope call): The function for comparing elements using user data.
 * @data: User data.
 *
 * Heapifies the array using user data.
 **/
void trb_heapify_data(void *array, usize len, usize elemsize, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_inssort:
 * @array: The array to be sorted.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmp_func: (scope call): The function for comparing elements.
 *
 * Sorts the array using Insertion Sort.
 **/
void trb_inssort(void *array, usize len, usize elemsize, TrbCmpFunc cmp_func);

/**
 * trb_inssort_data:
 * @array: The array to be sorted.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmpd_func: (scope call): The function for comparing elements.
 * @data: User data.
 *
 * Sorts the array using Insertion Sort and user data.
 **/
void trb_inssort_data(void *array, usize len, usize elemsize, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_heapsort:
 * @array: The array to be sorted.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmp_func: (scope call): The function for comparing elements.
 *
 * Sorts the array using Heap Sort.
 **/
void trb_heapsort(void *array, usize len, usize elemsize, TrbCmpFunc cmp_func);

/**
 * trb_heapsort_data:
 * @array: The array to be sorted.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmpd_func: (scope call): The function for comparing elements.
 * @data: User data.
 *
 * Sorts the array using Heap Sort and user data.
 **/
void trb_heapsort_data(void *array, usize len, usize elemsize, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_quicksort:
 * @array: The array to be sorted.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmp_func: (scope call): The function for comparing elements.
 *
 * Sorts the array using Quicksort by Knuth.
 * [Reference](http://math.utoledo.edu/~codenth/Fall_14/4380/Notes/trb_quicksort.pdf).
 **/
void trb_quicksort(void *array, usize len, usize elemsize, TrbCmpFunc cmp_func);

/**
 * trb_quicksort_data:
 * @array: The array to be sorted.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmpd_func: (scope call): The function for comparing elements.
 * @data: User data.
 *
 * Sorts the array using Quicksort by Knuth.
 * [Reference](http://math.utoledo.edu/~codenth/Fall_14/4380/Notes/trb_quicksort.pdf).
 **/
void trb_quicksort_data(void *array, usize len, usize elemsize, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_binary_search:
 * @array: The array to be searchen.
 * @target: The pointer to the data to be found.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmp_func: (scope call): The function for comparing elements.
 * @index: (optional) (out): The pointer to retrieve the index of found value.
 *
 * Searches for the entry in the array using binary search.
 *
 * The array should be sorted if you want to use this function.
 *
 * Returns: %TRUE if found, %FALSE if not.
 **/
bool trb_binary_search(const void *array, const void *target, usize len, usize elemsize, TrbCmpFunc cmp_func, usize *index);

/**
 * trb_binary_search_data:
 * @array: The array to be searchen.
 * @target: The pointer to the data to be found.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmpd_func: (scope call): The function for comparing elements.
 * @data: User data.
 * @index: (optional) (out): The pointer to retrieve the index of found value.
 *
 * Searches for the entry in the array using binary search and user data.
 *
 * The array should be sorted if you want to use this function.
 *
 * Returns: %TRUE if found, %FALSE if not.
 **/
bool trb_binary_search_data(
	const void *array,
	const void *target,
	usize len,
	usize elemsize,
	TrbCmpDataFunc cmpd_func,
	void *data,
	usize *index
);

/**
 * trb_2alloc:
 * @rows: The number of rows in the array.
 * @cols: The number of columns in the array.
 * @elemsize: The size of each element in the array.
 *
 * Allocates a two-dimensional array.
 * The array should be freed with `free()`.
 *
 * Returns: (nullable): The two-dimensional array, or %NULL if an error occurs.
 **/
void **trb_2alloc(usize rows, usize cols, usize elemsize);

/**
 * trb_u8cmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `u8`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_u8cmp(const u8 *a, const u8 *b);

/**
 * trb_u16cmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `u16`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_u16cmp(const u16 *a, const u16 *b);

/**
 * trb_u32cmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `u32`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_u32cmp(const u32 *a, const u32 *b);

/**
 * trb_u64cmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `u64`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_u64cmp(const u64 *a, const u64 *b);

/**
 * trb_usizecmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `usize`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_usizecmp(const usize *a, const usize *b);

/**
 * trb_i8cmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `i8`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_i8cmp(const i8 *a, const i8 *b);

/**
 * trb_i16cmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `i16`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_i16cmp(const i16 *a, const i16 *b);

/**
 * trb_i32cmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `i32`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_i32cmp(const i32 *a, const i32 *b);

/**
 * trb_i64cmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `i64`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_i64cmp(const i64 *a, const i64 *b);

/**
 * trb_isizecmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `isize`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_isizecmp(const isize *a, const isize *b);

/**
 * trb_f32cmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `f32`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_f32cmp(const f32 *a, const f32 *b);

/**
 * trb_f64cmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `f64`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_f64cmp(const f64 *a, const f64 *b);

/**
 * trb_realcmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `real`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_realcmp(const real *a, const real *b);

#endif /* end of include guard: UTILS_H_ATMQZMHO */
