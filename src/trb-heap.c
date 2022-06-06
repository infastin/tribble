#include "trb-heap.h"

#include "trb-messages.h"
#include "trb-utils.h"

#include <string.h>

TrbHeap *trb_heap_init(TrbHeap *self, usize elemsize, TrbCmpFunc cmp_func)
{
	trb_return_val_if_fail(cmp_func != NULL, NULL);
	trb_return_val_if_fail(elemsize != 0, NULL);

	bool was_allocated = FALSE;

	if (self == NULL) {
		self = trb_talloc(TrbHeap, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the heap!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	if (trb_vector_init(&self->vector, FALSE, elemsize) == NULL && was_allocated) {
		free(self);
		return NULL;
	}

	self->cmp_func = cmp_func;
	self->data = NULL;
	self->with_data = FALSE;

	return self;
}

TrbHeap *trb_heap_init_data(TrbHeap *self, usize elemsize, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_val_if_fail(cmpd_func != NULL, NULL);
	trb_return_val_if_fail(elemsize != 0, NULL);

	bool was_allocated = FALSE;

	if (self == NULL) {
		self = trb_talloc(TrbHeap, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the heap!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	if (trb_vector_init(&self->vector, FALSE, elemsize) == NULL && was_allocated) {
		free(self);
		return NULL;
	}

	self->cmpd_func = cmpd_func;
	self->data = data;
	self->with_data = TRUE;

	return self;
}

bool trb_heap_insert(TrbHeap *self, const void *data)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (trb_vector_push_back(&self->vector, data)) {
		if (self->vector.len == 1)
			return TRUE;

		TrbSlice heap_slice;
		trb_vector_slice(&self->vector, &heap_slice, 0, self->vector.len);

		if (self->with_data)
			trb_heapify_data(&heap_slice, self->cmpd_func, self->data);
		else
			trb_heapify(&heap_slice, self->cmp_func);

		return TRUE;
	}

	return FALSE;
}

void trb_heap_fix(TrbHeap *self)
{
	trb_return_if_fail(self != NULL);

	TrbSlice heap_slice;
	trb_vector_slice(&self->vector, &heap_slice, 0, self->vector.len);

	if (self->with_data)
		trb_heapify_data(&heap_slice, self->cmpd_func, self->data);
	else
		trb_heapify(&heap_slice, self->cmp_func);
}

static bool __trb_heap_remove(TrbHeap *self, usize index, void *ret)
{
	if (index >= self->vector.len) {
		trb_msg_warn("element at [%zu] is out of bounds!", index);
		return FALSE;
	}

	void *at_index = trb_vector_ptr(&self->vector, void, index);
	void *last = trb_vector_ptr(&self->vector, void, self->vector.len - 1);

	trb_memswap(at_index, last, self->vector.elemsize);

	if (trb_vector_pop_back(&self->vector, ret)) {
		TrbSlice heap_slice;
		trb_vector_slice(&self->vector, &heap_slice, 0, self->vector.len);

		if (self->with_data)
			trb_heapify_data(&heap_slice, self->cmpd_func, self->data);
		else
			trb_heapify(&heap_slice, self->cmp_func);

		return TRUE;
	} else {
		trb_memswap(at_index, last, self->vector.elemsize);
	}

	return FALSE;
}

bool trb_heap_remove(TrbHeap *self, usize index, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_heap_remove(self, index, ret);
}

bool trb_heap_pop_back(TrbHeap *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->vector.len == 0) {
		trb_msg_warn("heap is empty!");
		return FALSE;
	}

	return __trb_heap_remove(self, self->vector.len - 1, ret);
}

bool trb_heap_pop_front(TrbHeap *self, void *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_heap_remove(self, 0, ret);
}

bool trb_heap_search(const TrbHeap *self, const void *target, TrbCmpFunc cmp_func, usize *index)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	cmp_func = cmp_func ?: self->cmp_func;
	return trb_vector_search(&self->vector, target, cmp_func, index);
}

bool trb_heap_search_data(const TrbHeap *self, const void *target, TrbCmpDataFunc cmpd_func, void *data, usize *index)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	cmpd_func = cmpd_func ?: self->cmpd_func;
	data = data ?: self->data;
	return trb_vector_search_data(&self->vector, target, cmpd_func, data, index);
}

void trb_heap_destroy(TrbHeap *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);
	trb_vector_destroy(&self->vector, free_func);
}

void trb_heap_free(TrbHeap *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);
	trb_heap_destroy(self, free_func);
	free(self);
}
