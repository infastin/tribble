#include "tribble.h"

#define convert(ptr) ((u8(*)[])(ptr))

i32 my_cmp(const u32 *a, const u32 *b)
{
	i32 res = trb_u32cmp(a, b);
	return -res;
}

int main()
{
	TrbHeap heap;
	trb_heap_init(&heap, 4, (TrbCmpFunc) my_cmp);

	TrbPcg64 rng;
	trb_pcg64_init(&rng, 0xdeadbfef);

	for (int i = 0; i < 10; ++i) {
		u32 val = trb_pcg64_next_u32(&rng) % 100;
		trb_heap_insert(&heap, &val);
	}

	for (int i = 0; i < 10; ++i) {
		u32 val = trb_heap_get(&heap, u32, i);
		printf("%u ", val);
	}

	printf("\n");

	*trb_heap_ptr(&heap, u32, 0) = 99;

	TrbSlice slice;
	trb_deque_slice(&heap.deque, &slice, 0, heap.deque.len);

	trb_heapify(&slice, (TrbCmpFunc) my_cmp);

	for (int i = 0; i < 10; ++i) {
		u32 val = trb_heap_get(&heap, u32, i);
		printf("%u ", val);
	}

	return 0;
}
