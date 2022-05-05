#include "Heap.h"

#include "Messages.h"
#include "Utils.h"

Heap *heap_init(Heap *heap, u32 elemsize, CmpFunc cmp_func)
{
	return_val_if_fail(cmp_func != NULL, NULL);
	return_val_if_fail(elemsize != 0, NULL);

	bool was_allocated = FALSE;

	if (heap == NULL) {
		heap = talloc(Heap, 1);

		if (heap == NULL) {
			msg_error("couldn't allocate memory for the heap!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	if (array_init(&heap->array, FALSE, FALSE, elemsize) == NULL && was_allocated) {
		free(heap);
		return NULL;
	}

	heap->cmp_func = cmp_func;

	return heap;
}

bool heap_insert(Heap *heap, const void *data)
{
	return_val_if_fail(heap != NULL, FALSE);

	if (array_push_back(&heap->array, data)) {
		heapify(heap->array.mass, heap->array.len, heap->array.elemsize, heap->cmp_func);
		return TRUE;
	}

	return FALSE;
}

bool heap_remove(Heap *heap, u32 index, void *ret)
{
	return_val_if_fail(heap != NULL, FALSE);

	if (index >= heap->array.len) {
		msg_warn("element at [%u] is out of bounds!", index);
		return FALSE;
	}

	mass_swap(
		mass_cell(heap->array.mass, heap->array.elemsize, index),
		mass_cell(heap->array.mass, heap->array.elemsize, heap->array.len - 1),
		heap->array.elemsize
	);

	if (array_pop_back(&heap->array, ret)) {
		heapify(heap->array.mass, heap->array.len, heap->array.elemsize, heap->cmp_func);
		return TRUE;
	} else {
		mass_swap(
			mass_cell(heap->array.mass, heap->array.elemsize, index),
			mass_cell(heap->array.mass, heap->array.elemsize, heap->array.len - 1),
			heap->array.elemsize
		);
	}

	return FALSE;
}

bool heap_get(const Heap *heap, u32 index, void *ret)
{
	return_val_if_fail(heap != NULL, FALSE);
	return array_get(&heap->array, index, ret);
}

bool heap_search(Heap *heap, const void *target, u32 *index)
{
	return_val_if_fail(heap != NULL, FALSE);
	return array_search(&heap->array, target, heap->cmp_func, index);
}

void heap_purge(Heap *heap)
{
	return_if_fail(heap != NULL);
	array_purge(&heap->array);
}

void heap_free(Heap *heap)
{
	return_if_fail(heap != NULL);
	heap_purge(heap);
	free(heap);
}
