#include "trb-heap.h"

#include "trb-messages.h"
#include "trb-utils.h"

#include <memory.h>

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

	if (trb_deque_init(&self->deque, FALSE, elemsize) == NULL && was_allocated) {
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

	if (trb_deque_init(&self->deque, FALSE, elemsize) == NULL && was_allocated) {
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

	if (trb_deque_push_back(&self->deque, data)) {
		if (self->deque.len == 1)
			return TRUE;

		TrbSlice heap_slice;
		trb_deque_slice(&self->deque, &heap_slice, 0, self->deque.len);

		if (self->with_data)
			trb_heapify_data(&heap_slice, self->cmpd_func, self->data);
		else
			trb_heapify(&heap_slice, self->cmp_func);

		return TRUE;
	}

	return FALSE;
}

bool trb_heap_set(TrbHeap *self, usize index, const void *data)
{
	trb_return_val_if_fail(self != NULL, NULL);

	if (index >= self->deque.len) {
		trb_msg_warn("element at [%zu] is out of bounds!", index);
		return FALSE;
	}

	void *at_index = trb_deque_ptr(&self->deque, void, index);

	if (data != NULL)
		memcpy(at_index, data, self->deque.elemsize);
	else
		memset(at_index, 0, self->deque.elemsize);

	TrbSlice heap_slice;
	trb_deque_slice(&self->deque, &heap_slice, 0, self->deque.len);

	if (self->with_data)
		trb_heapify_data(&heap_slice, self->cmpd_func, self->data);
	else
		trb_heapify(&heap_slice, self->cmp_func);

	return TRUE;
}

static bool __trb_heap_remove(TrbHeap *self, usize index, void *ret)
{
	if (index >= self->deque.len) {
		trb_msg_warn("element at [%zu] is out of bounds!", index);
		return FALSE;
	}

	void *at_index = trb_deque_ptr(&self->deque, void, index);
	void *last = trb_deque_ptr(&self->deque, void, self->deque.len - 1);

	trb_memswap(at_index, last, self->deque.elemsize);

	if (trb_deque_pop_back(&self->deque, ret)) {
		TrbSlice heap_slice;
		trb_deque_slice(&self->deque, &heap_slice, 0, self->deque.len);

		if (self->with_data)
			trb_heapify_data(&heap_slice, self->cmpd_func, self->data);
		else
			trb_heapify(&heap_slice, self->cmp_func);

		return TRUE;
	} else {
		trb_memswap(at_index, last, self->deque.elemsize);
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

	if (self->deque.len == 0) {
		trb_msg_warn("heap is empty!");
		return FALSE;
	}

	return __trb_heap_remove(self, self->deque.len - 1, ret);
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

	usize i = 0;

	while (i < self->deque.len) {
		void *current = trb_deque_ptr(&self->deque, void, i);

		i32 cmp = cmp_func(current, target);

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

bool trb_heap_search_data(const TrbHeap *self, const void *target, TrbCmpDataFunc cmpd_func, void *data, usize *index)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	cmpd_func = cmpd_func ?: self->cmpd_func;
	data = data ?: self->data;

	usize i = 0;

	while (i < self->deque.len) {
		void *current = trb_deque_ptr(&self->deque, void, i);

		i32 cmp = cmpd_func(current, target, data);

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
	trb_deque_destroy(&self->deque, free_func);
}

void trb_heap_free(TrbHeap *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);
	trb_heap_destroy(self, free_func);
	free(self);
}
