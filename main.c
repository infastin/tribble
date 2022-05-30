#include "Tribble.h"

int main()
{
	TrbPcg128 *rng = trb_pcg128_init(NULL, U128_C(0xa0b, 0xdeadbeef));
	TrbVector *vector = trb_vector_init(NULL, TRUE, sizeof(u64));

	for (usize i = 0; i < 64; ++i) {
		u64 num = trb_pcg128_next_u64(rng) % 1000;
		trb_vector_push_back(vector, &num);
	}

	TrbSlice *vector_slice = trb_vector_slice(vector, NULL, 0, vector->len);

	trb_quicksort(vector_slice, (TrbCmpFunc) trb_u64cmp);

	for (usize i = 0; i < 64; ++i) {
		u64 num = *(u64 *) vector_slice->at(vector_slice, i);
		printf("%lu ", num);
	}

	free(vector_slice);
	trb_vector_free(vector, NULL);
	free(rng);

	return 0;
}
