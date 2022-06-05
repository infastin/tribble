#ifndef DEQUE_H_RVOLUALQ
#define DEQUE_H_RVOLUALQ

#include "trb-types.h"
#include "trb-vector.h"

typedef struct _TrbDeque TrbDeque;

/**
 * TrbDeque:
 * @len: The size of the deque.
 * @elemsize: The size of each element.
 * @clear: Indicates whether elements should be cleared to 0 when allocated or not.
 *
 * A double-ended queue.
 **/
struct _TrbDeque {
	/* <private> */
	TrbVector buckets;
	TrbVector unused;
	usize bucketsize;
	usize bucketcap;
	usize offset;
	/* <public> */
	usize elemsize;
	usize len;
	bool clear;
};

/**
 * trb_deque_init:
 * @self: (nullable): The pointer to the #TrbDeque to be initialized.
 * @clear: %TRUE if elements should be cleared to 0 when allocated.
 * @elemsize: The size of each element in bytes.
 *
 * Creates a new double-ended queue.
 *
 * Returns: (nullable): A new #TrbDeque.
 * Can return %NULL if an error occurs.
 **/
TrbDeque *trb_deque_init(TrbDeque *self, bool clear, usize elemsize);

/**
 * trb_deque_push_back:
 * @self: The deque where to add the element.
 * @data: The pointer to the data to be added.
 *
 * Adds the element to the end of the deque.
 * If @data is %NULL, adds a zero to the end of the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_push_back(TrbDeque *self, const void *data);

/**
 * trb_deque_push_front:
 * @self: The deque where to add the element.
 * @data: The pointer to the data to be added.
 *
 * Adds the element to the beginning of the deque.
 * If @data is %NULL, adds a zero to the beginning of the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_push_front(TrbDeque *self, const void *data);

/**
 * trb_deque_insert:
 * @self: The deque where to insert.
 * @index: The position to place the element at.
 * @data: The pointer to the data to be inserted.
 *
 * Inserts the element into the deque at the given index.
 * If @data is %NULL, inserts a zero into the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_insert(TrbDeque *self, usize index, const void *data);

/**
 * trb_deque_push_back_many:
 * @self: The deque where to add elements.
 * @data: The pointer to the elements to be added.
 * @len: The number of elements to be added.
 *
 * Adds elements to the end of the deque.
 * If @data is %NULL, adds zeroes to the end of the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_push_back_many(TrbDeque *self, const void *data, usize len);

/**
 * trb_deque_push_front_many:
 * @self: The deque where to add elements.
 * @data: The pointer to elements to be added.
 * @len: The number of elements to be added.
 *
 * Adds elements to the beginning of the deque.
 * If @data is %NULL, adds zeroes to the beginning of the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_push_front_many(TrbDeque *self, const void *data, usize len);

/**
 * trb_deque_insert_many:
 * @self: The deque where to insert.
 * @index: The position to place elements at.
 * @data: The pointer to elements to be inserted.
 * @len: The number of elements to be inserted.
 *
 * Inserts elements into the deque at the given index.
 * If @data is %NULL, inserts zeroes into the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_insert_many(TrbDeque *self, usize index, const void *data, usize len);

/**
 * trb_deque_remove:
 * @self: The deque where to remove.
 * @index: The position of the entry to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the entry at the given index from the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_remove(TrbDeque *self, usize index, void *ret);

/**
 * trb_deque_pop_back:
 * @self: The deque where to remove.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the last element from the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_pop_back(TrbDeque *self, void *ret);

/**
 * trb_deque_pop_front:
 * @self: The deque where to remove.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the first element from the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_pop_front(TrbDeque *self, void *ret);

/**
 * trb_deque_pop_back_many:
 * @self: The deque where to remove.
 * @len: The number of elements to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes last @len elements from the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_pop_back_many(TrbDeque *self, usize len, void *ret);

/**
 * trb_deque_pop_front_many:
 * @self: The deque where to remove.
 * @len: The number of elements to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes first @len elements from the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_pop_front_many(TrbDeque *self, usize len, void *ret);

/**
 * trb_deque_remove_range:
 * @self: The deque where to remove.
 * @index: The index of the first element to be removed.
 * @len: The number of elements to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the range of elements from the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_remove_range(TrbDeque *self, usize index, usize len, void *ret);

/**
 * trb_deque_remove_all:
 * @self: The deque where to remove.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes all elements from the deque.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_remove_all(TrbDeque *self, void *ret);

/**
 * trb_deque_search:
 * @self: The deque where to search.
 * @target: The pointer to the data to be found.
 * @cmp_func: (scope call): The function for comparing values.
 * @index: (optional) (out): The pointer to retrieve the index of found value.
 *
 * Searches for the entry in the deque.
 *
 * Returns: %TRUE if found, %FALSE if not.
 **/
bool trb_deque_search(const TrbDeque *self, const void *target, TrbCmpFunc cmp_func, usize *index);

/**
 * trb_deque_search_data:
 * @self: The deque where to search.
 * @target: The pointer to the data to be found.
 * @cmpd_func: (scope call): The function for comparing values.
 * @data: User data.
 * @index: (optional) (out): The pointer to retrieve the index of found value.
 *
 * Searches for the entry in the deque using user data.
 *
 * Returns: %TRUE if found, %FALSE if not.
 **/
bool trb_deque_search_data(const TrbDeque *self, const void *target, TrbCmpDataFunc cmpd_func, void *data, usize *index);

/**
 * trb_deque_slice:
 * @self: The deque to be sliced.
 * @slice: (nullable): The pointer to the slice to be initialized.
 * @start: The start position in the deque.
 * @end: The end position in the deque.
 *
 * Slices the #TrbDeque.
 * If allocated on the heap, use `free()` to release the allocated memory.
 *
 * Returns: (nullable): A new #TrbSlice.
 * Can return %NULL if an error occurs.
 **/
TrbSlice *trb_deque_slice(TrbDeque *self, TrbSlice *slice, usize start, usize end);

/**
 * trb_deque_destroy:
 * @self: The deque which buckets is to be freed.
 * @free_func: (scope call) (nullable): The function for freeing elements.
 *
 * Frees the deque buckets.
 **/
void trb_deque_destroy(TrbDeque *self, TrbFreeFunc free_func);

/**
 * trb_deque_free:
 * @self: The deque to be freed.
 * @free_func: (scope call) (nullable): The function for freeing elements.
 *
 * Frees the deque completely.
 **/
void trb_deque_free(TrbDeque *self, TrbFreeFunc free_func);

/**
 * trb_deque_shrink:
 * @self: The deque which buffer is to shrunk.
 *
 * Frees unused memory of the deque buckets.
 *
 * Returns: %TRUE on success.
 **/
bool trb_deque_shrink(TrbDeque *self);

/**
 * trb_deque_ptr:
 * @self: The deque where to get.
 * @type: The type of the element.
 * @index: The position of the entry.
 *
 * Gets the pointer to the entry in the deque at the given index.
 **/
#define trb_deque_ptr(self, type, index) ({                      \
	usize __ei = ((index) + (self)->offset) % (self)->bucketcap; \
	usize __bi = ((index) + (self)->offset) / (self)->bucketcap; \
	void *__b = trb_vector_get(&(self)->buckets, void *, __bi);  \
	(type *) trb_array_cell(__b, __ei, (self)->elemsize);        \
})

/**
 * trb_deque_get:
 * @self: The deque where to get.
 * @type: The type of the element.
 * @index: The position of the entry.
 *
 * Gets the value of the entry in the deque at the given index.
 **/
#define trb_deque_get(self, type, index) (*trb_deque_ptr(self, type, index))

#endif /* end of include guard: DEQUE_H_RVOLUALQ */
