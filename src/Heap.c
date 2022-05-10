#include "Heap.h"

#include "Messages.h"
#include "Utils.h"

Heap *heap_init(Heap *heap, usize elemsize, CmpFunc cmp_func)
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

	if (vector_init(&heap->vector, FALSE, FALSE, elemsize) == NULL && was_allocated) {
		free(heap);
		return NULL;
	}

	heap->cmp_func = cmp_func;

	return heap;
}

bool heap_insert(Heap *heap, const void *data)
{
	return_val_if_fail(heap != NULL, FALSE);

	if (vector_push_back(&heap->vector, data)) {
		heapify(heap->vector.data, heap->vector.len, heap->vector.elemsize, heap->cmp_func);
		return TRUE;
	}

	return FALSE;
}

static bool __heap_remove(Heap *heap, usize index, void *ret)
{
	if (index >= heap->vector.len) {
		msg_warn("element at [%zu] is out of bounds!", index);
		return FALSE;
	}

	array_swap(
		array_cell(heap->vector.data, heap->vector.elemsize, index),
		array_cell(heap->vector.data, heap->vector.elemsize, heap->vector.len - 1),
		heap->vector.elemsize
	);

	if (vector_pop_back(&heap->vector, ret)) {
		heapify(heap->vector.data, heap->vector.len, heap->vector.elemsize, heap->cmp_func);
		return TRUE;
	} else {
		array_swap(
			array_cell(heap->vector.data, heap->vector.elemsize, index),
			array_cell(heap->vector.data, heap->vector.elemsize, heap->vector.len - 1),
			heap->vector.elemsize
		);
	}

	return FALSE;
}

bool heap_remove(Heap *heap, usize index, void *ret)
{
	return_val_if_fail(heap != NULL, FALSE);
	return __heap_remove(heap, index, ret);
}

bool heap_pop_back(Heap *heap, void *ret)
{
	return_val_if_fail(heap != NULL, FALSE);

	if (heap->vector.len == 0) {
		msg_warn("heap is empty!");
		return FALSE;
	}

	return __heap_remove(heap, heap->vector.len - 1, ret);
}

bool heap_pop_front(Heap *heap, void *ret)
{
	return_val_if_fail(heap != NULL, FALSE);
	return __heap_remove(heap, 0, ret);
}

bool heap_get(const Heap *heap, usize index, void *ret)
{
	return_val_if_fail(heap != NULL, FALSE);
	return vector_get(&heap->vector, index, ret);
}

bool heap_search(const Heap *heap, const void *target, usize *index)
{
	return_val_if_fail(heap != NULL, FALSE);

	usize i = 0;

	while (i < heap->vector.len) {
		void *current = array_cell(heap->vector.data, heap->vector.elemsize, 0);

		i32 cmp = heap->cmp_func(current, target);

		if (cmp > 0) {
			i = (i << 1) + 1;
		} else if (cmp < 0) {
			i = (i << 1) + 2;
		} else {
			if (index != NULL)
				*index = i;
			return TRUE;
		}
	}

	return FALSE;
}

void heap_purge(Heap *heap, FreeFunc free_func)
{
	return_if_fail(heap != NULL);
	vector_purge(&heap->vector, free_func);
}

void heap_free(Heap *heap, FreeFunc free_func)
{
	return_if_fail(heap != NULL);
	heap_purge(heap, free_func);
	free(heap);
}
