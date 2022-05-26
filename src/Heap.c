#include "Heap.h"

#include "Messages.h"
#include "Utils.h"

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

	if (trb_vector_init(&self->vector, FALSE, FALSE, elemsize) == NULL && was_allocated) {
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

	if (trb_vector_init(&self->vector, FALSE, FALSE, elemsize) == NULL && was_allocated) {
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
		if (self->with_data)
			trb_heapify_data(self->vector.data, self->vector.len, self->vector.elemsize, self->cmpd_func, self->data);
		else
			trb_heapify(self->vector.data, self->vector.len, self->vector.elemsize, self->cmp_func);
		return TRUE;
	}

	return FALSE;
}

static bool __trb_heap_remove(TrbHeap *self, usize index, void *ret)
{
	if (index >= self->vector.len) {
		trb_msg_warn("element at [%zu] is out of bounds!", index);
		return FALSE;
	}

	trb_array_swap(
		trb_array_cell(self->vector.data, self->vector.elemsize, index),
		trb_array_cell(self->vector.data, self->vector.elemsize, self->vector.len - 1),
		self->vector.elemsize
	);

	if (trb_vector_pop_back(&self->vector, ret)) {
		if (self->with_data)
			trb_heapify_data(self->vector.data, self->vector.len, self->vector.elemsize, self->cmpd_func, self->data);
		else
			trb_heapify(self->vector.data, self->vector.len, self->vector.elemsize, self->cmp_func);
		return TRUE;
	} else {
		trb_array_swap(
			trb_array_cell(self->vector.data, self->vector.elemsize, index),
			trb_array_cell(self->vector.data, self->vector.elemsize, self->vector.len - 1),
			self->vector.elemsize
		);
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

bool trb_heap_search(const TrbHeap *self, const void *target, usize *index)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	usize i = 0;

	while (i < self->vector.len) {
		void *current = trb_array_cell(self->vector.data, self->vector.elemsize, i);

		i32 cmp;

		if (self->with_data)
			cmp = self->cmpd_func(current, target, self->data);
		else
			cmp = self->cmp_func(current, target);

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
