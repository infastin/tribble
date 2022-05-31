#ifndef UTILS_H_ATMQZMHO
#define UTILS_H_ATMQZMHO

#include "Macros.h"
#include "Slice.h"
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
 * @slice: The slice to be heapified.
 * @cmp_func: (scope call): The function for comparing elements.
 *
 * Heapifies the slice.
 **/
void trb_heapify(TrbSlice *slice, TrbCmpFunc cmp_func);

/**
 * trb_heapify_data:
 * @slice: The slice to be heapified.
 * @cmpd_func: (scope call): The function for comparing elements using user data.
 * @data: User data.
 *
 * Heapifies the slice using user data.
 **/
void trb_heapify_data(TrbSlice *slice, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_inssort:
 * @slice: The slice to be sorted.
 * @cmp_func: (scope call): The function for comparing elements.
 *
 * Sorts the slice using Insertion Sort.
 **/
void trb_inssort(TrbSlice *slice, TrbCmpFunc cmp_func);

/**
 * trb_inssort_data:
 * @slice: The slice to be sorted.
 * @cmpd_func: (scope call): The function for comparing elements.
 * @data: User data.
 *
 * Sorts the slice using Insertion Sort and user data.
 **/
void trb_inssort_data(TrbSlice *slice, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_heapsort:
 * @slice: The slice to be sorted.
 * @cmp_func: (scope call): The function for comparing elements.
 *
 * Sorts the slice using Heap Sort.
 **/
void trb_heapsort(TrbSlice *slice, TrbCmpFunc cmp_func);

/**
 * trb_heapsort_data:
 * @slice: The slice to be sorted.
 * @cmpd_func: (scope call): The function for comparing elements.
 * @data: User data.
 *
 * Sorts the slice using Heap Sort and user data.
 **/
void trb_heapsort_data(TrbSlice *slice, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_quicksort:
 * @slice: The slice to be sorted.
 * @cmp_func: (scope call): The function for comparing elements.
 *
 * Sorts the slice using Quicksort by Knuth.
 * [Reference](http://math.utoledo.edu/~codenth/Fall_14/4380/Notes/trb_quicksort.pdf).
 **/
void trb_quicksort(TrbSlice *slice, TrbCmpFunc cmp_func);

/**
 * trb_quicksort_data:
 * @slice: The array to be sorted.
 * @cmpd_func: (scope call): The function for comparing elements.
 * @data: User data.
 *
 * Sorts the slice using Quicksort by Knuth.
 * [Reference](http://math.utoledo.edu/~codenth/Fall_14/4380/Notes/trb_quicksort.pdf).
 **/
void trb_quicksort_data(TrbSlice *slice, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_reverse:
 * @slice: The slice to be reversed.
 *
 * Reverses the slice.
 **/
void trb_reverse(TrbSlice *slice);

/**
 * trb_binary_search:
 * @slice: The slice to be searched.
 * @target: The pointer to the data to be found.
 * @cmp_func: (scope call): The function for comparing elements.
 * @index: (optional) (out): The pointer to retrieve the index of found value.
 *
 * Searches for the entry in the slice using binary search.
 *
 * The slice should be sorted if you want to use this function.
 *
 * Returns: %TRUE if found, %FALSE if not.
 **/
bool trb_binary_search(const TrbSlice *slice, const void *target, TrbCmpFunc cmp_func, usize *index);

/**
 * trb_binary_search_data:
 * @slice: The slice to be searched.
 * @target: The pointer to the data to be found.
 * @cmpd_func: (scope call): The function for comparing elements.
 * @data: User data.
 * @index: (optional) (out): The pointer to retrieve the index of found value.
 *
 * Searches for the entry in the slice using binary search and user data.
 *
 * The slice should be sorted if you want to use this function.
 *
 * Returns: %TRUE if found, %FALSE if not.
 **/
bool trb_binary_search_data(const TrbSlice *slice, const void *target, TrbCmpDataFunc cmpd_func, void *data, usize *index);

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
 * trb_f128cmp:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * Compares two `f128`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 trb_f128cmp(const f128 *a, const f128 *b);

#endif /* end of include guard: UTILS_H_ATMQZMHO */
