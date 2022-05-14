#ifndef HEAP_H_MGLIJUQF
#define HEAP_H_MGLIJUQF

#include "Types.h"
#include "Vector.h"

typedef struct _Heap Heap;

struct _Heap {
	Vector vector;
	CmpFunc cmp_func;
};

/**
 * heap_init:
 * @heap: The pointer to the `Heap` to be initialized (can be `NULL`).
 * @elemsize: The size of each element in bytes.
 * @cmp_func: The compare function for comparing elements.
 *
 * Creates a new #Heap.
 *
 * Returns: A new #Heap. Can return `NULL` if an error occurs.
 **/
Heap *heap_init(Heap *heap, usize elemsize, CmpFunc cmp_func);

/**
 * heap_insert:
 * @heap: The heap where to insert the element.
 * @data: The element to be inserted (can be `NULL`).
 *
 * Inserts the element in the heap.
 *
 * Returns: `TRUE` on success.
 **/
bool heap_insert(Heap *heap, const void *data);

/**
 * heap_remove:
 * @heap: The heap where to remove the element.
 * @index: The index of the element to be removed.
 * @ret: The pointer to retrieve removed data (can be `NULL`).
 *
 * Removes the element from the heap.
 *
 * Returns: `TRUE` on success.
 **/
bool heap_remove(Heap *heap, usize index, void *ret);

/**
 * heap_pop_back:
 * @heap: The heap where to remove the element.
 * @ret: The pointer to retrieve removed data (can be `NULL`).
 *
 * Removes the last element from the heap.
 *
 * Returns: `TRUE` on success.
 **/
bool heap_pop_back(Heap *heap, void *ret);

/**
 * heap_pop_front:
 * @heap: The heap where to remove the element.
 * @ret: The pointer to retrieve removed data (can be `NULL`).
 *
 * Removes the first element from the heap.
 *
 * Returns: `TRUE` on success.
 **/
bool heap_pop_front(Heap *heap, void *ret);

/**
 * heap_get:
 * @heap: The heap where to get.
 * @index: The position of the entry which value is to be got.
 * @ret: The pointer to retrieve value of the entry.
 *
 * Gets the value of the entry in the heap at the given index.
 *
 * Returns: `TRUE` on success.
 **/
bool heap_get(const Heap *heap, usize index, void *ret);

/**
 * heap_search:
 * @heap: The heap where to search.
 * @target: The pointer to the data to be found (can be `NULL`).
 * @index: The pointer to retrieve the index of found value (can be `NULL`).
 *
 * Searches for the entry in the heap.
 *
 * Returns: `TRUE` if found, `FALSE` if not.
 **/
bool heap_search(const Heap *heap, const void *target, usize *index);

/**
 * heap_destroy:
 * @heap: The heap which buffer is to be freed.
 * @free_func: The function for freeing elements (can be `NULL`).
 *
 * Frees the heap buffer.
 **/
void heap_destroy(Heap *heap, FreeFunc free_func);

/**
 * heap_free:
 * @heap: The heap to be freed.
 * @free_func: The function for freeing elements (can be `NULL`).
 *
 * Frees the heap completely.
 **/
void heap_free(Heap *heap, FreeFunc free_func);

/**
 * heap_get_unsafe:
 * @heap: The heap where to get.
 * @type: The type to be got.
 * @index: The position of the entry which value is to be got.
 *
 * Unsafe version of vector_heap.
 *
 * Returns: The value of the entry.
 **/
#define heap_get_unsafe(heap, type, index) (vector_get_unsafe(&(heap)->vector, type, index))

#endif /* end of include guard: HEAP_H_MGLIJUQF */
