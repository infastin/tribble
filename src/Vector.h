#ifndef VECTOR_H_KSABYJ3T
#define VECTOR_H_KSABYJ3T

#include "Types.h"

#include <stdint.h>

typedef struct _TrbVector TrbVector;

/**
 * TrbVector:
 * @data: The vector buffer.
 * @len: The size of the vector.
 * @capacity: The capacity of the vector buffer.
 * @elemsize: The size of each element.
 * @zero_terminated: Indicates whether vector is zero-terminated or not.
 * @clear: Indicates whether elements should be cleared to 0 when allocated or not.
 * @sorted: Indicates whether vector is sorted or not.
 *
 * A dynamic size array.
 **/
struct _TrbVector {
	void *data;
	usize len;
	usize capacity;
	usize elemsize;
	bool zero_terminated;
	bool clear;
	bool sorted;
};

/**
 * trb_vector_init:
 * @self: (nullable): The pointer to the `TrbVector` to be initialized.
 * @clear: %TRUE if elements should be cleared to 0 when allocated.
 * @zero_terminated: %TRUE if the array should have terminating zero.
 * @elemsize: The size of each element in bytes.
 *
 * Creates a new dynamic size array.
 *
 * Returns: (nullable): A new dynamic size array.
 * Can return %NULL if an error occurs.
 **/
TrbVector *trb_vector_init(TrbVector *self, bool clear, bool zero_terminated, usize elemsize);

/**
 * trb_vector_copy:
 * @dst: (optional) (inout): The pointer to the destination array.
 * @src: The array to be copied.
 *
 * Creates a copy of the array.
 *
 * Returns: (nullable): A copy of the array.
 * Can return %NULL if an allocation error occurs.
 **/
TrbVector *trb_vector_copy(TrbVector *dst, const TrbVector *src);

/**
 * trb_vector_push_back:
 * @self: The array where to add the element.
 * @data: The pointer to the data to be added.
 *
 * Adds the element to the end of the array.
 * If @data is %NULL, adds a zero to the end of the array.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_push_back(TrbVector *self, const void *data);

/**
 * trb_vector_push_back_many:
 * @self: The array where to add elements.
 * @data: The pointer to the elements to be added.
 * @len: The number of elements to be added.
 *
 * Adds elements to the end of the array.
 * If @data is %NULL, adds zeroes to the end of the array.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_push_back_many(TrbVector *self, const void *data, usize len);

/**
 * trb_vector_push_front:
 * @self: The array where to add the element.
 * @data: The pointer to the data to be added.
 *
 * Adds the element to the beginning of the array.
 * If @data is %NULL, adds a zero to the beginning of the array.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_push_front(TrbVector *self, const void *data);

/**
 * trb_vector_push_front_many:
 * @self: The array where to add elements.
 * @data: The pointer to elements to be added.
 * @len: The number of elements to be added.
 *
 * Adds elements to the beginning of the array.
 * If @data is %NULL, adds zeroes to the beginning of the array.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_push_front_many(TrbVector *self, const void *data, usize len);

/**
 * trb_vector_insert:
 * @self: The array where to insert.
 * @index: The position to place the element at.
 * @data: The pointer to the data to be inserted.
 *
 * Inserts the element into the array at the given index.
 * If @data is %NULL, inserts a zero into the array.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_insert(TrbVector *self, usize index, const void *data);

/**
 * trb_vector_insert_many:
 * @self: The array where to insert.
 * @index: The position to place elements at.
 * @data: The pointer to elements to be inserted.
 * @len: The number of elements to be inserted.
 *
 * Inserts elements into the array at the given index.
 * If @data is %NULL, inserts zeroes into the array.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_insert_many(TrbVector *self, usize index, const void *data, usize len);

/**
 * trb_vector_set:
 * @self: The array where to change.
 * @index: The position of the entry in which to change the value.
 * @data: The data to be set.
 *
 * Changes the value of the entry at the given index in the array
 * with the given data.
 * If @data is %NULL, changes the value with a zero.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_set(TrbVector *self, usize index, const void *data);

/**
 * trb_vector_set_range:
 * @self: The array where to overwrite.
 * @index: The position of the entry in which to overwrite the values.
 * @data: The data to be set.
 * @len The number of elements to be overwriten.
 *
 * Overwrites values of the entries at the given index in the
 * array with the given data.
 * If @data is %NULL, overwrites values with zeroes.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_set_range(TrbVector *self, usize index, const void *data, usize len);

/**
 * trb_vector_remove_index:
 * @self: The array where to remove.
 * @index: The position of the entry to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the entry at the given index from the array.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_remove_index(TrbVector *self, usize index, void *ret);

/**
 * trb_vector_pop_back:
 * @self: The array where to remove.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the last element from the array.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_pop_back(TrbVector *self, void *ret);

/**
 * trb_vector_pop_front:
 * @self: The array where to remove.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the first element from the array.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_pop_front(TrbVector *self, void *ret);

/**
 * trb_vector_reverse:
 * @self: The array to be reversed.
 *
 * Reverses the array.
 **/
void trb_vector_reverse(TrbVector *self);

/**
 * trb_vector_remove_range:
 * @self: The vec where to remove.
 * @index: The index of the first element to be removed.
 * @len: The number of elements to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the range of elements from the array.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_remove_range(TrbVector *self, usize index, usize len, void *ret);

/**
 * trb_vector_remove_all:
 * @self: The vec where to remove.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes all elements from the array.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_remove_all(TrbVector *self, void *ret);

/**
 * trb_vector_sort:
 * @self: The array to be sorted.
 * @cmp_func: (scope call): The comparison function for sorting entries.
 *
 * Sorts the array.
 **/
void trb_vector_sort(TrbVector *self, TrbCmpFunc cmp_func);

/**
 * trb_vector_sort_data:
 * @self: The array to be sorted.
 * @cmpd_func: (scope call): The comparison function for sorting entries.
 * @data: User data.
 *
 * Sorts the array using user data.
 **/
void trb_vector_sort_data(TrbVector *self, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_vector_get:
 * @self: The array where to get.
 * @index: The position of the entry which value is to be got.
 * @ret: (out): The pointer to retrieve value of the entry.
 *
 * Gets the value of the entry in the array at the given index.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_get(const TrbVector *self, usize index, void *ret);

/**
 * trb_vector_get_range:
 * @self: The array where to get.
 * @index: The index of the first element to be got.
 * @len: The number of elements to be got.
 * @ret: (out): The pointer to retrieve the data.
 *
 * Gets the range of elements in the array.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_get_range(const TrbVector *self, usize index, usize len, void *ret);

/**
 * trb_vector_steal:
 * @self: The array where to steal the buffer.
 * @len: (optional) (out): The pointer to retrieve the number of elements in the array.
 *
 * Steals the array buffer.
 * Array creates a new buffer.
 *
 * Returns: (transfer full) (nullable): The buffer on success.
 * Can return %NULL on failure.
 **/
void *trb_vector_steal(TrbVector *self, usize *len);

/**
 * trb_vector_steal0:
 * @self: The array where to steal the buffer.
 * @len: (optional) (out): The pointer to retrieve the number of the elements in the array.
 *
 * Steals the array buffer.
 * Array's buffer becomes %NULL.
 *
 * Returns: (transfer full) (nullable): The buffer on success.
 * Can return %NULL on failure.
 **/
void *trb_vector_steal0(TrbVector *self, usize *len);

/**
 * trb_vector_require:
 * @self: The array where memory is to be reserved.
 * @newcap: A new capacity of the array.
 *
 * Reserves memory in the array buffer.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_require(TrbVector *self, usize newcap);

/**
 * trb_vector_shrink:
 * @self: The array which buffer is to shrunk.
 *
 * Frees unused memory of the array buffer.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_shrink(TrbVector *self);

/**
 * trb_vector_destroy:
 * @self: The array which buffer is to be freed.
 * @free_func: (scope call) (nullable): The function for freeing elements.
 *
 * Frees the array buffer.
 **/
void trb_vector_destroy(TrbVector *self, TrbFreeFunc free_func);

/**
 * trb_vector_free:
 * @self: The array to be freed.
 * @free_func: (scope call) (nullable): The function for freeing elements.
 *
 * Frees the array completely.
 **/
void trb_vector_free(TrbVector *self, TrbFreeFunc free_func);

/**
 * trb_vector_search:
 * @self: The array where to search.
 * @target: The pointer to the data to be found.
 * @cmp_func: (scope call): The function for comparing values.
 * @index: (optional) (out): The pointer to retrieve the index of found value.
 *
 * Searches for the entry in the array.
 *
 * Returns: %TRUE if found, %FALSE if not.
 **/
bool trb_vector_search(TrbVector *self, const void *target, TrbCmpFunc cmp_func, usize *index);

/**
 * trb_vector_search_data:
 * @self: The array where to search.
 * @target: The pointer to the data to be found.
 * @cmpd_func: (scope call): The function for comparing values.
 * @data: User data.
 * @index: (optional) (out): The pointer to retrieve the index of found value.
 *
 * Searches for the entry in the array using user data.
 *
 * Returns: %TRUE if found, %FALSE if not.
 **/
bool trb_vector_search_data(TrbVector *self, const void *target, TrbCmpDataFunc cmpd_func, void *data, usize *index);

/**
 * trb_vector_get_unsafe:
 * @self: The array where to get.
 * @type: The type to be got.
 * @index: The position of the entry which value is to be got.
 *
 * Unsafe version of `trb_vector_get()`.
 *
 * Returns: The value of the entry.
 **/
#define trb_vector_get_unsafe(self, type, index) (*(type *) &((char *) ((self)->data))[(index) * (self)->elemsize])

#endif /* end of include guard: ARRAY_H_KSABYJ3T */
