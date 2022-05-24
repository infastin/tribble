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
	usize n_buckets;
	usize n_unused;
	usize len;
};

TrbDeque *trb_deque_init(TrbDeque *self, usize elemsize);

bool trb_deque_push_back(TrbDeque *self, const void *data);

bool trb_deque_push_front(TrbDeque *self, const void *data);

bool trb_deque_pop_back(TrbDeque *self, void *ret);

bool trb_deque_pop_front(TrbDeque *self, void *ret);

bool trb_deque_get(TrbDeque *self, usize index, void *ret);

void trb_deque_destroy(TrbDeque *self, TrbFreeFunc free_func);

void trb_deque_free(TrbDeque *self, TrbFreeFunc free_func);

void trb_deque_shrink(TrbDeque *self);

#define trb_deque_get_unsafe(deque, type, index) ({                                         \
	TrbDequeBucket *__f = trb_vector_get_unsafe(&(deque)->buckets, TrbDequeBucket *, 0);    \
	usize __ei = ((index) + __f->offset) % (deque)->bucketcap;                              \
	usize __bi = ((index) + __f->offset) / (deque)->bucketcap;                              \
	TrbDequeBucket *__b = trb_vector_get_unsafe(&(deque)->buckets, TrbDequeBucket *, __bi); \
	(*(type *) &(__b->data[__ei * (deque)->elemsize]));                                     \
})

#endif /* end of include guard: DEQUE_H_RVOLUALQ */
