#ifndef DEQUE_H_RVOLUALQ
#define DEQUE_H_RVOLUALQ

#include "Types.h"
#include "Vector.h"

typedef struct _TrbDeque TrbDeque;

struct _TrbDeque {
	TrbVector buckets;
	TrbVector unused;
	usize elemsize;
	usize bucketsize;
	usize bucketcap;
	usize offset;
	usize len;
	bool clear;
	bool sorted;
};

TrbDeque *trb_deque_init(TrbDeque *self, bool clear, usize elemsize);

bool trb_deque_push_back(TrbDeque *self, const void *data);

bool trb_deque_push_front(TrbDeque *self, const void *data);

bool trb_deque_insert(TrbDeque *self, usize index, const void *data);

bool trb_deque_push_back_many(TrbDeque *self, const void *data, usize len);

bool trb_deque_push_front_many(TrbDeque *self, const void *data, usize len);

bool trb_deque_insert_many(TrbDeque *self, usize index, const void *data, usize len);

bool trb_deque_remove(TrbDeque *self, usize index, void *ret);

bool trb_deque_pop_back(TrbDeque *self, void *ret);

bool trb_deque_pop_front(TrbDeque *self, void *ret);

bool trb_deque_pop_back_many(TrbDeque *self, usize len, void *ret);

bool trb_deque_pop_front_many(TrbDeque *self, usize len, void *ret);

bool trb_deque_remove_range(TrbDeque *self, usize index, usize len, void *ret);

bool trb_deque_remove_all(TrbDeque *self, void *ret);

bool trb_deque_search(const TrbDeque *self, const void *target, TrbCmpFunc cmp_func, usize *index);

bool trb_deque_search_data(const TrbDeque *self, const void *target, TrbCmpDataFunc cmpd_func, void *data, usize *index);

TrbSlice *trb_deque_slice(TrbSlice *dst, TrbDeque *src, usize start, usize end);

void trb_deque_destroy(TrbDeque *self, TrbFreeFunc free_func);

void trb_deque_free(TrbDeque *self, TrbFreeFunc free_func);

void trb_deque_shrink(TrbDeque *self);

/**
 * trb_deque_ptr:
 * @self: The deque where to get.
 * @type: The type of the element.
 * @index: The position of the entry.
 *
 * Gets the pointer to the entry in the deque at the given index.
 *
 * Returns: The pointer to the entry.
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
 *
 * Returns: The value of the entry.
 **/
#define trb_deque_get(self, type, index) (*trb_deque_ptr(self, type, index))

#endif /* end of include guard: DEQUE_H_RVOLUALQ */
