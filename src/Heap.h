#ifndef HEAP_H_MGLIJUQF
#define HEAP_H_MGLIJUQF

#include "Array.h"
#include "Types.h"

typedef struct _Heap Heap;

struct _Heap {
	Array array;
	CmpFunc cmp_func;
};

Heap *heap_init(Heap *heap, u32 elemsize, CmpFunc cmp_func);

bool heap_insert(Heap *heap, const void *data);

bool heap_remove(Heap *heap, u32 index, void *ret);

bool heap_get(const Heap *heap, u32 index, void *ret);

bool heap_search(Heap *heap, const void *target, u32 *index);

void heap_purge(Heap *heap);

void heap_free(Heap *heap);

#define heap_get_unsafe(heap, type, index) (array_get_unsafe(&(heap)->array, type, index))

#endif /* end of include guard: HEAP_H_MGLIJUQF */
