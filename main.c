#include "tribble.h"

#include <stdio.h>

#define convert(ptr) ((u8(*)[])(ptr))

i32 my_cmp(const u32 *a, const u32 *b)
{
	i32 res = trb_u32cmp(a, b);
	return -res;
}

int main()
{
	/* TrbHeap heap; */
	/* trb_heap_init(&heap, 4, (TrbCmpFunc) my_cmp); */
	/*  */
	TrbPcg64 rng;
	trb_pcg64_init(&rng, 0xdeadbfef);
	/*  */
	/* for (int i = 0; i < 10; ++i) { */
	/* 	u32 val = trb_pcg64_next_u32(&rng) % 100; */
	/* 	trb_heap_insert(&heap, &val); */
	/* } */
	/*  */
	/* for (int i = 0; i < 10; ++i) { */
	/* 	u32 val = trb_heap_get(&heap, u32, i); */
	/* 	printf("%u ", val); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* *trb_heap_ptr(&heap, u32, 0) = 99; */
	/*  */
	/* TrbSlice slice; */
	/* trb_vector_slice(&heap.vector, &slice, 0, heap.vector.len); */
	/*  */
	/* trb_heapify(&slice, (TrbCmpFunc) my_cmp); */
	/*  */
	/* for (int i = 0; i < 10; ++i) { */
	/* 	u32 val = trb_heap_get(&heap, u32, i); */
	/* 	printf("%u ", val); */
	/* } */

	TrbVector vector;
	trb_vector_init(&vector, TRUE, 8);

	trb_vector_push_back(&vector, trb_get_ptr(u64, 78));

	for (usize i = 0; i < 20; ++i) {
		trb_vector_pop_front(&vector, NULL);

		for (usize j = 0; j < 4; ++j) {
			u64 val = trb_pcg64_next_u64(&rng) % 100;
			trb_vector_push_back(&vector, &val);
		}
	}

	TrbSlice slice;
	trb_vector_slice(&vector, &slice, 0, vector.len);

	for (usize i = 0; i < vector.len; ++i) {
		u64 val = *(u64 *) slice.at(&slice, i);
		printf("%lu ", val);
	}

	trb_vector_pop_front_many(&vector, 30, NULL);

	printf("\n");

	trb_vector_slice(&vector, &slice, 0, vector.len);

	for (usize i = 0; i < vector.len; ++i) {
		u64 val = *(u64 *) slice.at(&slice, i);
		printf("%lu ", val);
	}

	printf("\n%zu\n", vector.offset);

	trb_vector_shrink(&vector);

	printf("%zu\n", vector.offset);

	trb_vector_slice(&vector, &slice, 0, vector.len);

	for (usize i = 0; i < vector.len; ++i) {
		u64 val = *(u64 *) slice.at(&slice, i);
		printf("%lu ", val);
	}

	return 0;
}
