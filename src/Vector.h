#ifndef VECTOR_H_KSABYJ3T
#define VECTOR_H_KSABYJ3T

#include "Slice.h"
#include "Types.h"

typedef struct _TrbVector TrbVector;

/**
 * TrbVector:
 * @data: The vector buffer.
 * @len: The size of the vector.
 * @capacity: The capacity of the vector buffer.
 * @elemsize: The size of each element.
 * @clear: Indicates whether elements should be cleared to 0 when allocated or not.
 *
 * A dynamic size array.
 **/
struct _TrbVector {
	void *data;
	usize len;
	usize capacity;
	usize elemsize;
	bool clear;
};

/**
 * trb_vector_init:
 * @self: (nullable): The pointer to the #TrbVector to be initialized.
 * @clear: %TRUE if elements should be cleared to 0 when allocated.
 * @elemsize: The size of each element in bytes.
 *
 * Creates a new dynamic size array.
 *
 * Returns: (nullable): A new dynamic size array.
 * Can return %NULL if an error occurs.
 **/
TrbVector *trb_vector_init(TrbVector *self, bool clear, usize elemsize);

/**
 * trb_vector_copy:
 * @src: The vector to be copied.
 * @dst: (optional) (inout): The pointer to the destination array.
 *
 * Creates a copy of the vector.
 *
 * Returns: (nullable): A copy of the vector.
 * Can return %NULL if an allocation error occurs.
 **/
TrbVector *trb_vector_copy(const TrbVector *src, TrbVector *dst);

/**
 * trb_vector_push_back:
 * @self: The vector where to add the element.
 * @data: The pointer to the data to be added.
 *
 * Adds the element to the end of the vector.
 * If @data is %NULL, adds a zero to the end of the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_push_back(TrbVector *self, const void *data);

/**
 * trb_vector_push_back_many:
 * @self: The vector where to add elements.
 * @data: The pointer to the elements to be added.
 * @len: The number of elements to be added.
 *
 * Adds elements to the end of the vector.
 * If @data is %NULL, adds zeroes to the end of the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_push_back_many(TrbVector *self, const void *data, usize len);

/**
 * trb_vector_push_front:
 * @self: The vector where to add the element.
 * @data: The pointer to the data to be added.
 *
 * Adds the element to the beginning of the vector.
 * If @data is %NULL, adds a zero to the beginning of the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_push_front(TrbVector *self, const void *data);

/**
 * trb_vector_push_front_many:
 * @self: The vector where to add elements.
 * @data: The pointer to elements to be added.
 * @len: The number of elements to be added.
 *
 * Adds elements to the beginning of the vector.
 * If @data is %NULL, adds zeroes to the beginning of the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_push_front_many(TrbVector *self, const void *data, usize len);

/**
 * trb_vector_insert:
 * @self: The vector where to insert.
 * @index: The position to place the element at.
 * @data: The pointer to the data to be inserted.
 *
 * Inserts the element into the vector at the given index.
 * If @data is %NULL, inserts a zero into the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_insert(TrbVector *self, usize index, const void *data);

/**
 * trb_vector_insert_many:
 * @self: The vector where to insert.
 * @index: The position to place elements at.
 * @data: The pointer to elements to be inserted.
 * @len: The number of elements to be inserted.
 *
 * Inserts elements into the vector at the given index.
 * If @data is %NULL, inserts zeroes into the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_insert_many(TrbVector *self, usize index, const void *data, usize len);

/**
 * trb_vector_remove:
 * @self: The vector where to remove.
 * @index: The position of the entry to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the entry at the given index from the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_remove(TrbVector *self, usize index, void *ret);

/**
 * trb_vector_pop_back:
 * @self: The vector where to remove.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the last element from the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_pop_back(TrbVector *self, void *ret);

/**
 * trb_vector_pop_back_many:
 * @self: The vector where to remove.
 * @len: The number of elements to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes last @len elements from the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_pop_back_many(TrbVector *self, usize len, void *ret);

/**
 * trb_vector_pop_front:
 * @self: The vector where to remove.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the first element from the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_pop_front(TrbVector *self, void *ret);

/**
 * trb_vector_pop_front_many:
 * @self: The vector where to remove.
 * @len: The number of elements to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes first @len elements from the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_pop_front_many(TrbVector *self, usize len, void *ret);

/**
 * trb_vector_remove_range:
 * @self: The vector where to remove.
 * @index: The index of the first element to be removed.
 * @len: The number of elements to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the range of elements from the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_remove_range(TrbVector *self, usize index, usize len, void *ret);

/**
 * trb_vector_remove_all:
 * @self: The vector where to remove.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes all elements from the vector.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_remove_all(TrbVector *self, void *ret);

/**
 * trb_vector_steal:
 * @self: The vector where to steal the buffer.
 * @len: (optional) (out): The pointer to retrieve the number of elements in the vector.
 *
 * Steals the vector buffer.
 * Array creates a new buffer.
 *
 * Returns: (transfer full) (nullable): The buffer on success.
 * Can return %NULL on failure.
 **/
void *trb_vector_steal(TrbVector *self, usize *len);

/**
 * trb_vector_steal0:
 * @self: The vector where to steal the buffer.
 * @len: (optional) (out): The pointer to retrieve the number of the elements in the vector.
 *
 * Steals the vector buffer.
 * Array's buffer becomes %NULL.
 *
 * Returns: (transfer full) (nullable): The buffer on success.
 * Can return %NULL on failure.
 **/
void *trb_vector_steal0(TrbVector *self, usize *len);

/**
 * trb_vector_require:
 * @self: The vector where memory is to be reserved.
 * @newcap: A new capacity of the vector.
 *
 * Reserves memory in the vector buffer.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_require(TrbVector *self, usize newcap);

/**
 * trb_vector_shrink:
 * @self: The vector which buffer is to shrunk.
 *
 * Frees unused memory of the vector buffer.
 *
 * Returns: %TRUE on success.
 **/
bool trb_vector_shrink(TrbVector *self);

/**
 * trb_vector_destroy:
 * @self: The vector which buffer is to be freed.
 * @free_func: (scope call) (nullable): The function for freeing elements.
 *
 * Frees the vector buffer.
 **/
void trb_vector_destroy(TrbVector *self, TrbFreeFunc free_func);

/**
 * trb_vector_free:
 * @self: The vector to be freed.
 * @free_func: (scope call) (nullable): The function for freeing elements.
 *
 * Frees the vector completely.
 **/
void trb_vector_free(TrbVector *self, TrbFreeFunc free_func);

/**
 * trb_vector_search:
 * @self: The vector where to search.
 * @target: The pointer to the data to be found.
 * @cmp_func: (scope call): The function for comparing values.
 * @index: (optional) (out): The pointer to retrieve the index of found value.
 *
 * Searches for the entry in the vector.
 *
 * Returns: %TRUE if found, %FALSE if not.
 **/
bool trb_vector_search(const TrbVector *self, const void *target, TrbCmpFunc cmp_func, usize *index);

/**
 * trb_vector_search_data:
 * @self: The vector where to search.
 * @target: The pointer to the data to be found.
 * @cmpd_func: (scope call): The function for comparing values.
 * @data: User data.
 * @index: (optional) (out): The pointer to retrieve the index of found value.
 *
 * Searches for the entry in the vector using user data.
 *
 * Returns: %TRUE if found, %FALSE if not.
 **/
bool trb_vector_search_data(const TrbVector *self, const void *target, TrbCmpDataFunc cmpd_func, void *data, usize *index);

/**
 * trb_vector_slice:
 * @self: The vector to be sliced.
 * @slice: (nullable): The pointer to the slice to be initialized.
 * @start: The start position in the vector.
 * @end: The end position in the vector.
 *
 * Slices the #TrbVector.
 *
 * Returns: A new #TrbSlice.
 **/
TrbSlice *trb_vector_slice(TrbVector *self, TrbSlice *slice, usize start, usize end);

/**
 * trb_vector_ptr:
 * @self: The vector where to get.
 * @type: The type of the element.
 * @index: The position of the entry.
 *
 * Gets the pointer to the entry in the vector at the given index.
 **/
#define trb_vector_ptr(self, type, index) ((type *) &((char *) ((self)->data))[(index) * (self)->elemsize])

/**
 * trb_vector_get:
 * @self: The vector where to get.
 * @type: The type of the element.
 * @index: The position of the entry.
 *
 * Gets the value of the entry in the vector at the given index.
 **/
#define trb_vector_get(self, type, index) (*trb_vector_ptr(self, type, index))

#endif /* end of include guard: ARRAY_H_KSABYJ3T */
