#ifndef UTILS_H_ATMQZMHO
#define UTILS_H_ATMQZMHO

#include "Macros.h"
#include "Types.h"

#include <stdarg.h>

/**
 * strfmt:
 * @buf: The buffer where the formatted string is to be placed.
 * @fmt: The format of the string.
 * @...: Arguments.
 *
 * It is analogous to `snprintf()`, except it allocates a string large enough to
 * hold the output including terminating null byte.
 *
 * Returns: The length of the formatted string. Can return -1 if an error occurs.
 **/
usize strfmt(char **buf, const char *fmt, ...) FORMAT(printf, 2, 3);

/**
 * vstrfmt:
 * @buf: The buffer where the formatted string is to be placed.
 * @fmt: The format of the string.
 * @args: The argument list.
 *
 * It is analogous to `vsnprintf()`, except it allocates a string large enough to
 * hold the output including terminating null byte.
 *
 * Returns: The length of the formatted string. Can return -1 if an error occurs.
 **/
usize vstrfmt(char **buf, const char *fmt, va_list args) FORMAT(printf, 2, 0);

/**
 * strdup_fmt:
 * @fmt: The format of the string.
 * @...: Arguments.
 *
 * It is analogous to `snprintf()`, except it allocates a string large enough to
 * hold the output including terminating null byte.
 *
 * Returns: The string with terminating null byte. Can return `NULL` if an error occurs.
 **/
char *strdup_fmt(const char *fmt, ...) FORMAT(printf, 1, 2);

/**
 * strdup_vfmt:
 * @fmt: The format of the string.
 * @args: The argument list.
 *
 * It is analogous to `vsnprintf()`, except it allocates a string large enough to
 * hold the output including terminating null byte.
 *
 * Returns: The string with terminating null byte. Can return `NULL` if error occurs.
 **/
char *strdup_vfmt(const char *fmt, va_list args) FORMAT(printf, 1, 0);

/**
 * pow2_32:
 * @value: The 32-bit integer.
 *
 * Rounds up to the next power of 2.
 *
 * Returns: The next power of 2.
 **/
u32 pow2_32(u32 value);

/**
 * pow2_64:
 * @value: The 64-bit integer.
 *
 * Rounds up to the next power of 2.
 *
 * Returns: The next power of 2.
 **/
u64 pow2_64(u64 value);

/**
 * pow2:
 * @value: The 32/64-bit integer.
 *
 * Rounds up to the next power of 2.
 *
 * Returns: The next power of 2.
 **/
usize pow2(usize value);

/**
 * heapify;
 * @array: The array to be heapified.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmp_func: The compare function for comparing elements.
 *
 * Heapifies the array.
 **/
void heapify(void *array, usize len, usize elemsize, CmpFunc cmp_func);

/**
 * inssort:
 * @array: The array to be sorted.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmp_func: The compare function for comparing elements.
 *
 * Sorts the array using Insertion Sort.
 **/
void inssort(void *array, usize len, usize elemsize, CmpFunc cmp_func);

/**
 * heapsort:
 * @array: The array to be sorted.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmp_func: The compare function for comparing elements.
 *
 * Sorts the array using Heap Sort.
 **/
void heapsort(void *array, usize len, usize elemsize, CmpFunc cmp_func);

/**
 * quicksort:
 * @array: The array to be sorted.
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmp_func: The compare function for comparing elements.
 *
 * Sorts the array using Quicksort by Knuth.
 * [Reference](http://math.utoledo.edu/~codenth/Fall_14/4380/Notes/quicksort.pdf).
 **/
void quicksort(void *array, usize len, usize elemsize, CmpFunc cmp_func);

/**
 * binary_search:
 * @array: The array to be searchen.
 * @target: The pointer to the data to be found (can be `NULL`).
 * @len: The length of the array.
 * @elemsize: The size of each element in the array.
 * @cmp_func: The compare function for comparing elements.
 * @index: The pointer to retrieve the index of found value (can be `NULL`).
 *
 * Searches for the entry in the array using binary search.
 * The array should be sorted if you want to use this function.
 *
 * Returns: `TRUE` if found, `FALSE` if not.
 **/
bool binary_search(const void *array, const void *target, usize len, usize elemsize, CmpFunc cmp_func, usize *index);

/**
 * u8cmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %u8.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 u8cmp(const u8 *a, const u8 *b);

/**
 * u16cmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %u16.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 u16cmp(const u16 *a, const u16 *b);

/**
 * u32cmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %u32.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 u32cmp(const u32 *a, const u32 *b);

/**
 * u64cmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %u64.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 u64cmp(const u64 *a, const u64 *b);

/**
 * usizecmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %usize.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 usizecmp(const usize *a, const usize *b);

/**
 * i8cmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %i8.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 i8cmp(const i8 *a, const i8 *b);

/**
 * i16cmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %i16.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 i16cmp(const i16 *a, const i16 *b);

/**
 * i32cmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %i32.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 i32cmp(const i32 *a, const i32 *b);

/**
 * i64cmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %i64.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 i64cmp(const i64 *a, const i64 *b);

/**
 * isizecmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %isize.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 isizecmp(const isize *a, const isize *b);

/**
 * f32cmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %f32.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 f32cmp(const f32 *a, const f32 *b);

/**
 * f64cmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %f64.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 f64cmp(const f64 *a, const f64 *b);

/**
 * realcmp:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * Compares two %real.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 realcmp(const real *a, const real *b);

#endif /* end of include guard: UTILS_H_ATMQZMHO */
