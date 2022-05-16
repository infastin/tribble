#ifndef VECTOR_H_KSABYJ3T
#define VECTOR_H_KSABYJ3T

#include "Types.h"

#include <stdint.h>

typedef struct _Vector Vector;

struct _Vector {
	void *data;
	usize len;
	usize capacity;
	usize elemsize;
	usize zero_terminated : 1;
	usize sorted : 1;
	usize clear : 1;
};

/**
 * vector_init:
 * @vec: The pointer to the `Vector` to be initialized (can be `NULL`).
 * @clear: `TRUE` if elements should be cleared to 0 when allocated.
 * @zero_terminated: `TRUE` if the array should have terminating zero.
 * @elemsize: The size of each element in bytes.
 *
 * Creates a new dynamic size array.
 *
 * Returns: A new dynamic size array. Can return `NULL` if an error occurs.
 **/
Vector *vector_init(Vector *vec, bool clear, bool zero_terminated, usize elemsize);

/**
 * vector_copy:
 * @dst: The pointer to the destination array (can be `NULL`).
 * @src: The array to be copied.
 *
 * Creates a copy of the array.
 *
 * Returns: A copy of the array. Can return `NULL` if an allocation error occurs.
 **/
Vector *vector_copy(Vector *dst, const Vector *src);

/**
 * vector_push_back:
 * @vec: The array where to add the element.
 * @data: The pointer to the data to be added (can be `NULL`).
 *
 * Adds the element to the end of the array.
 * If @data is `NULL`, adds a zero to the end of the array.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_push_back(Vector *vec, const void *data);

/**
 * vector_push_back_many:
 * @vec: The array where to add elements.
 * @data: The pointer to the elements to be added (can be `NULL`).
 * @len: The number of elements to be added.
 *
 * Adds elements to the end of the array.
 * If @data is `NULL`, adds zeroes to the end of the array.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_push_back_many(Vector *vec, const void *data, usize len);

/**
 * vector_push_front:
 * @vec: The array where to add the element.
 * @data: The pointer to the data to be added (can be `NULL`).
 *
 * Adds the element to the beginning of the array.
 * If @data is `NULL`, adds a zero to the beginning of the array.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_push_front(Vector *vec, const void *data);

/**
 * vector_push_front_many:
 * @vec: The array where to add elements.
 * @data: The pointer to elements to be added (can be `NULL`).
 * @len: The number of elements to be added.
 *
 * Adds elements to the beginning of the array.
 * If @data is `NULL`, adds zeroes to the beginning of the array.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_push_front_many(Vector *vec, const void *data, usize len);

/**
 * vector_insert:
 * @vec: The array where to insert.
 * @index: The position to place the element at.
 * @data: The pointer to the data to be inserted (can be `NULL`).
 *
 * Inserts the element into the array at the given index.
 * If @data is `NULL`, inserts a zero into the array.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_insert(Vector *vec, usize index, const void *data);

/**
 * vector_insert:
 * @vec: The array where to insert.
 * @index: The position to place elements at.
 * @data: The pointer to elements to be inserted (can be `NULL`).
 * @len: The number of elements to be inserted.
 *
 * Inserts elements into the array at the given index.
 * If @data is `NULL`, inserts zeroes into the array.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_insert_many(Vector *vec, usize index, const void *data, usize len);

/**
 * vector_set:
 * @vec: The array where to change.
 * @index: The position of the entry in which to change the value.
 * @data: The data to be set (can be `NULL`).
 *
 * Changes the value of the entry at the given index in the array
 * with the given data.
 * If @data is `NULL`, changes the value with a zero.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_set(Vector *vec, usize index, const void *data);

/**
 * vector_set_range:
 * @vec: The array where to overwrite.
 * @index: The position of the entry in which to overwrite the values.
 * @data: The data to be set (can be `NULL`).
 * @len The number of elements to be overwriten.
 *
 * Overwrites values of the entries at the given index in the
 * array with the given data.
 * If @data is `NULL`, overwrites values with zeroes.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_set_range(Vector *vec, usize index, const void *data, usize len);

/**
 * vector_remove_index:
 * @vec: The array where to remove.
 * @index: The position of the entry to be removed.
 * @ret: The pointer to retrieve removed data (can be `NULL`).
 *
 * Removes the entry at the given index from the array.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_remove_index(Vector *vec, usize index, void *ret);

/**
 * vector_pop_back:
 * @vec: The array where to remove.
 * @ret: The pointer to retrieve removed data (can be `NULL`).
 *
 * Removes the last element from the array.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_pop_back(Vector *vec, void *ret);

/**
 * vector_pop_front:
 * @vec: The array where to remove.
 * @ret: The pointer to retrieve removed data (can be `NULL`).
 *
 * Removes the first element from the array.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_pop_front(Vector *vec, void *ret);

/**
 * vector_reverse:
 * @vec: The array to be reversed.
 *
 * Reverses the array.
 **/
void vector_reverse(Vector *vec);

/**
 * vector_remove_range:
 * @vec: The vec where to remove.
 * @index: The index of the first element to be removed.
 * @len: The number of elements to be removed.
 * @ret: The pointer to retrieve removed data (can be `NULL`).
 *
 * Removes the range of elements from the array.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_remove_range(Vector *vec, usize index, usize len, void *ret);

/**
 * vector_remove_all:
 * @vec: The vec where to remove.
 * @ret: The pointer to retrieve removed data (can be `NULL`).
 *
 * Removes all elements from the array.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_remove_all(Vector *vec, void *ret);

/**
 * vector_sort:
 * @vec: The array to be sorted.
 * @cmp_func: The compare function for sorting entries.
 *
 * Sorts the array.
 **/
void vector_sort(Vector *vec, CmpFunc cmp_func);

/**
 * vector_get:
 * @vec: The array where to get.
 * @index: The position of the entry which value is to be got.
 * @ret: The pointer to retrieve value of the entry.
 *
 * Gets the value of the entry in the array at the given index.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_get(const Vector *vec, usize index, void *ret);

/**
 * vector_get_range:
 * @vec: The array where to get.
 * @index: The index of the first element to be got.
 * @len: The number of elements to be got.
 * @ret: The pointer to retrieve the data.
 *
 * Gets the range of elements in the array.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_get_range(const Vector *vec, usize index, usize len, void *ret);

/**
 * vector_steal:
 * @vec: The array where to steal the buffer.
 * @len: The pointer to retrieve the number of elements in the array (can be `NULL`).
 *
 * Steals the array buffer.
 * Array creates a new buffer.
 *
 * Returns: The buffer on success.
 **/
void *vector_steal(Vector *vec, usize *len);

/**
 * vector_steal0:
 * @vec: The array where to steal the buffer.
 * @len: The pointer to retrieve the number of the elements in the array (can be `NULL`).
 *
 * Steals the array buffer.
 * Array's buffer becomes `NULL`.
 *
 * Returns: The buffer on success.
 **/
void *vector_steal0(Vector *vec, usize *len);

/**
 * vector_require:
 * @vec: The array where memory is to be reserved.
 * @newcap: A new capacity of the array.
 *
 * Reserves memory in the array buffer.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_require(Vector *vec, usize newcap);

/**
 * vector_shrink:
 * @vec: The array which buffer is to shrunk.
 *
 * Frees unused memory of the array buffer.
 *
 * Returns: `TRUE` on success.
 **/
bool vector_shrink(Vector *vec);

/**
 * vector_destroy:
 * @vec: The array which buffer is to be freed.
 * @free_func: The function for freeing elements (can be `NULL`).
 *
 * Frees the array buffer.
 **/
void vector_destroy(Vector *vec, FreeFunc free_func);

/**
 * vector_free:
 * @vec: The array to be freed.
 * @free_func: The function for freeing elements (can be `NULL`).
 *
 * Frees the array completely.
 **/
void vector_free(Vector *vec, FreeFunc free_func);

/**
 * vector_search:
 * @vec: The array where to search.
 * @target: The pointer to the data to be found (can be `NULL`).
 * @cmp_func: The function for comparing values.
 * @index: The pointer to retrieve the index of found value (can be `NULL`).
 *
 * Searches for the entry in the array.
 *
 * Returns: `TRUE` if found, `FALSE` if not.
 **/
bool vector_search(Vector *vec, const void *target, CmpFunc cmp_func, usize *index);

/**
 * vector_get_unsafe:
 * @vec: The array where to get.
 * @type: The type to be got.
 * @index: The position of the entry which value is to be got.
 *
 * Unsafe version of vector_get.
 *
 * Returns: The value of the entry.
 **/
#define vector_get_unsafe(vec, type, index) (*(type *) &((char *) ((vec)->data))[(index) * (vec)->elemsize])

#endif /* end of include guard: ARRAY_H_KSABYJ3T */
