#ifndef HEAP_H_MGLIJUQF
#define HEAP_H_MGLIJUQF

#include "Types.h"
#include "Vector.h"

typedef struct _Heap Heap;

struct _Heap {
	Vector vector;
	CmpFunc cmp_func;
};

Heap *heap_init(Heap *heap, usize elemsize, CmpFunc cmp_func);

bool heap_insert(Heap *heap, const void *data);

bool heap_remove(Heap *heap, usize index, void *ret);

bool heap_pop_back(Heap *heap, void *ret);

bool heap_pop_front(Heap *heap, void *ret);

bool heap_get(const Heap *heap, usize index, void *ret);

bool heap_search(Heap *heap, const void *target, usize *index);

void heap_purge(Heap *heap, FreeFunc free_func);

void heap_free(Heap *heap, FreeFunc free_func);

#define heap_get_unsafe(heap, type, index) (vector_get_unsafe(&(heap)->vector, type, index))

#endif /* end of include guard: HEAP_H_MGLIJUQF */
