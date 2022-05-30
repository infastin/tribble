#ifndef HEAP_H_MGLIJUQF
#define HEAP_H_MGLIJUQF

#include "Types.h"
#include "Vector.h"

typedef struct _TrbHeap TrbHeap;

/**
 * TrbHeap:
 * @vector: A #Vector representation of the heap.
 * @cmp_func: The function for comparing elements.
 * @cmpd_func: The function for comparing elements using user data.
 * @data: User data.
 * @with_data: Indicates whether #TrbHeap has been initialized with data or not.
 *
 * A heap data structure represented as an array.
 **/
struct _TrbHeap {
	TrbVector vector;

	union {
		TrbCmpFunc cmp_func;
		TrbCmpDataFunc cmpd_func;
	};

	void *data;
	bool with_data;
};

/**
 * trb_heap_init:
 * @self: (nullable): The pointer to the `TrbHeap` to be initialized.
 * @elemsize: The size of each element in bytes.
 * @cmp_func: The function for comparing elements.
 *
 * Creates a new #TrbHeap.
 *
 * Returns: A new #TrbHeap. Can return `NULL` if an error occurs.
 **/
TrbHeap *trb_heap_init(TrbHeap *self, usize elemsize, TrbCmpFunc cmp_func);

/**
 * trb_heap_init_data:
 * @self: (nullable): The pointer to the `TrbHeap` to be initialized.
 * @elemsize: The size of each element in bytes.
 * @cmpd_func: The function for comparing elements.
 * @data: User data.
 *
 * Creates a new #TrbHeap with the comparison function that accepts user data.
 *
 * Returns: A new #TrbHeap. Can return `NULL` if an error occurs.
 **/
TrbHeap *trb_heap_init_data(TrbHeap *self, usize elemsize, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_heap_insert:
 * @self: The heap where to insert the element.
 * @data: The element to be inserted.
 *
 * Inserts the element in the heap.
 *
 * Returns: `TRUE` on success.
 **/
bool trb_heap_insert(TrbHeap *self, const void *data);

/**
 * trb_heap_remove:
 * @self: The heap where to remove the element.
 * @index: The index of the element to be removed.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the element from the heap.
 *
 * Returns: `TRUE` on success.
 **/
bool trb_heap_remove(TrbHeap *self, usize index, void *ret);

/**
 * trb_heap_pop_back:
 * @self: The heap where to remove the element.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the last element from the heap.
 *
 * Returns: `TRUE` on success.
 **/
bool trb_heap_pop_back(TrbHeap *self, void *ret);

/**
 * trb_heap_pop_front:
 * @self: The heap where to remove the element.
 * @ret: (optional) (out): The pointer to retrieve removed data.
 *
 * Removes the first element from the heap.
 *
 * Returns: `TRUE` on success.
 **/
bool trb_heap_pop_front(TrbHeap *self, void *ret);

/**
 * trb_heap_search:
 * @self: The heap where to search.
 * @target: The pointer to the data to be found.
 * @index: (optional) (out): The pointer to retrieve the index of found value.
 *
 * Searches for the entry in the heap.
 *
 * Returns: `TRUE` if found, `FALSE` if not.
 **/
bool trb_heap_search(const TrbHeap *self, const void *target, usize *index);

/**
 * trb_heap_destroy:
 * @self: The heap which buffer is to be freed.
 * @free_func: (scope call) (nullable): The function for freeing elements.
 *
 * Frees the heap buffer.
 **/
void trb_heap_destroy(TrbHeap *self, TrbFreeFunc free_func);

/**
 * trb_heap_free:
 * @self: The heap to be freed.
 * @free_func: (scope call) (nullable): The function for freeing elements.
 *
 * Frees the heap completely.
 **/
void trb_heap_free(TrbHeap *self, TrbFreeFunc free_func);

/**
 * trb_heap_ptr:
 * @self: The heap where to get.
 * @type: The type of the element.
 * @index: The position of the entry.
 *
 * Gets the pointer to the entry in the heap at the given index.
 **/
#define trb_heap_ptr(self, type, index) (trb_vector_ptr(&(self)->vector, type, index))

/**
 * trb_heap_get:
 * @self: The heap where to get.
 * @type: The type of the element.
 * @index: The position of the entry.
 *
 * Gets the value of the entry in the heap at the given index.
 **/
#define trb_heap_get(self, type, index) (trb_vector_get(&(self)->vector, type, index))

#endif /* end of include guard: HEAP_H_MGLIJUQF */
