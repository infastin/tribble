#ifndef DEQUE_H_RVOLUALQ
#define DEQUE_H_RVOLUALQ

#include "Types.h"
#include "Vector.h"

typedef struct _TrbDequeBucket TrbDequeBucket;

struct _TrbDequeBucket {
	usize offset;
	usize len;
	char data[];
};

typedef struct _TrbDeque TrbDeque;

struct _TrbDeque {
	TrbVector buckets;
	TrbVector unused;
	usize elemsize;
	usize bucketsize;
	usize bucketcap;
	usize len;
};

TrbDeque *trb_deque_init(TrbDeque *self, usize elemsize);

bool trb_deque_push_back(TrbDeque *self, const void *data);

bool trb_deque_push_front(TrbDeque *self, const void *data);

bool trb_deque_insert(TrbDeque *self, usize index, const void *data);

bool trb_deque_push_back_many(TrbDeque *self, const void *data, usize len);

bool trb_deque_push_front_many(TrbDeque *self, const void *data, usize len);

bool trb_deque_insert_many(TrbDeque *self, usize index, const void *data, usize len);

bool trb_deque_pop_back(TrbDeque *self, void *ret);

bool trb_deque_pop_front(TrbDeque *self, void *ret);

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
#define trb_deque_ptr(self, type, index) ({                                         \
	TrbDequeBucket *__f = trb_vector_get(&(self)->buckets, TrbDequeBucket *, 0);    \
	usize __ei = ((index) + __f->offset) % (self)->bucketcap;                       \
	usize __bi = ((index) + __f->offset) / (self)->bucketcap;                       \
	TrbDequeBucket *__b = trb_vector_get(&(self)->buckets, TrbDequeBucket *, __bi); \
	((type *) &(__b->data[__ei * (self)->elemsize]));                               \
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
