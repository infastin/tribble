#include "Hash.h"

static inline uint32_t rotl32(uint32_t x, int8_t r)
{
	return (x << r) | (x >> (32 - r));
}

uint32_t MurmurHash3(const void *key, uint32_t keysize, uint32_t seed)
{
	const uint8_t *data = (const uint8_t *) key;
	const int32_t nblocks = keysize / 4;

	uint32_t h = seed;

	const uint32_t c1 = 0xcc9e2d51;
	const uint32_t c2 = 0x1b873593;

	const uint32_t *blocks = (const uint32_t *) (data + nblocks * 4);

	for (int32_t i = -nblocks; i; i++) {
		uint32_t k1 = blocks[i];

		k1 *= c1;
		k1 = rotl32(k1, 15);
		k1 *= c2;

		h ^= k1;
		h = rotl32(h, 13);
		h = h * 5 + 0xe6546b64;
	}

	const uint8_t *tail = (const uint8_t *) (data + nblocks * 4);

	uint32_t k1 = 0;

	switch (keysize & 3) {
	case 3: k1 ^= tail[2] << 16;
	case 2: k1 ^= tail[1] << 8;
	case 1:
		k1 ^= tail[0];
		k1 *= c1;
		k1 = rotl32(k1, 15);
		k1 *= c2;
		h ^= k1;
	};

	h ^= keysize;

	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;

	return h;
}
