#include "Hash.h"

#include "Macros.h"

u32 murmurhash3_32(const void *key, u32 keysize, u32 seed)
{
	const u8 *data = key;
	const i32 nblocks = keysize / 4;

	u32 h = seed;

	const u32 c1 = 0xcc9e2d51;
	const u32 c2 = 0x1b873593;

	const u32 *blocks = (const u32 *) (data + nblocks * 4);

	for (i32 i = -nblocks; i; i++) {
		u32 k = blocks[i];

		k *= c1;
		k = rotl32(k, 15);
		k *= c2;

		h ^= k;
		h = rotl32(h, 13);
		h = h * 5 + 0xe6546b64;
	}

	const u8 *tail = (const u8 *) (data + nblocks * 4);

	u32 k = 0;

	switch (keysize & 3) {
	case 3: k ^= tail[2] << 16;
	case 2: k ^= tail[1] << 8;
	case 1:
		k ^= tail[0];
		k *= c1;
		k = rotl32(k, 15);
		k *= c2;
		h ^= k;
	};

	h ^= keysize;

	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;

	return h;
}

u64 murmurhash3_64(const void *key, u64 keysize, u64 seed)
{
	const u8 *data = key;
	const int nblocks = keysize / 16;

	u64 h1 = seed;
	u64 h2 = seed;

	const u64 c1 = U64_C(0x87c37b91114253d5);
	const u64 c2 = U64_C(0x4cf5ad432745937f);

	const u64 *blocks = (const u64 *) (data);

	for (int i = 0; i < nblocks; i++) {
		u64 k1 = blocks[i * 2];
		u64 k2 = blocks[i * 2 + 1];

		k1 *= c1;
		k1 = rotl64(k1, 31);
		k1 *= c2;
		h1 ^= k1;

		h1 = rotl64(h1, 27);
		h1 += h2;
		h1 = h1 * 5 + 0x52dce729;

		k2 *= c2;
		k2 = rotl64(k2, 33);
		k2 *= c1;
		h2 ^= k2;

		h2 = rotl64(h2, 31);
		h2 += h1;
		h2 = h2 * 5 + 0x38495ab5;
	}

	const u8 *tail = (const u8 *) (data + nblocks * 16);

	u64 k1 = 0;
	u64 k2 = 0;

	switch (keysize & 15) {
	case 15: k2 ^= ((u64) tail[14]) << 48;
	case 14: k2 ^= ((u64) tail[13]) << 40;
	case 13: k2 ^= ((u64) tail[12]) << 32;
	case 12: k2 ^= ((u64) tail[11]) << 24;
	case 11: k2 ^= ((u64) tail[10]) << 16;
	case 10: k2 ^= ((u64) tail[9]) << 8;
	case 9:
		k2 ^= ((u64) tail[8]) << 0;
		k2 *= c2;
		k2 = rotl64(k2, 33);
		k2 *= c1;
		h2 ^= k2;
	case 8: k1 ^= ((u64) tail[7]) << 56;
	case 7: k1 ^= ((u64) tail[6]) << 48;
	case 6: k1 ^= ((u64) tail[5]) << 40;
	case 5: k1 ^= ((u64) tail[4]) << 32;
	case 4: k1 ^= ((u64) tail[3]) << 24;
	case 3: k1 ^= ((u64) tail[2]) << 16;
	case 2: k1 ^= ((u64) tail[1]) << 8;
	case 1:
		k1 ^= ((u64) tail[0]) << 0;
		k1 *= c1;
		k1 = rotl64(k1, 31);
		k1 *= c2;
		h1 ^= k1;
	};

	h1 ^= keysize;
	h2 ^= keysize;

	h1 += h2;
	h2 += h1;

	h1 ^= h1 >> 33;
	h1 *= U64_C(0xff51afd7ed558ccd);
	h1 ^= h1 >> 33;
	h1 *= U64_C(0xc4ceb9fe1a85ec53);
	h1 ^= h1 >> 33;

	h2 ^= h2 >> 33;
	h2 *= U64_C(0xff51afd7ed558ccd);
	h2 ^= h2 >> 33;
	h2 *= U64_C(0xc4ceb9fe1a85ec53);
	h2 ^= h2 >> 33;

	h1 += h2;
	h2 += h1;

	return h1;
}

usize murmurhash3(const void *key, usize keysize, usize seed)
{
#if __WORDSIZE == 64
	return murmurhash3_64(key, keysize, seed);
#else
	return murmurhash3_32(key, keysize, seed);
#endif
}

#define jhash32_mix(a, b, c) \
	{                        \
		a -= b;              \
		a -= c;              \
		a ^= c >> 13;        \
		b -= c;              \
		b -= a;              \
		b ^= a << 8;         \
		c -= a;              \
		c -= b;              \
		c ^= b >> 13;        \
		a -= b;              \
		a -= c;              \
		a ^= c >> 12;        \
		b -= c;              \
		b -= a;              \
		b ^= a << 16;        \
		c -= a;              \
		c -= b;              \
		c ^= b >> 5;         \
		a -= b;              \
		a -= c;              \
		a ^= c >> 3;         \
		b -= c;              \
		b -= a;              \
		b ^= a << 10;        \
		c -= a;              \
		c -= b;              \
		c ^= b >> 15;        \
	}

u32 jhash32(const void *key, u32 keysize, u32 seed)
{
	const u8 *k = key;
	u32 a, b, c;
	u32 len = keysize;

	a = b = 0x9e3779b9;
	c = seed;

	while (len >= 12) {
		a += k[0] + ((u32) k[1] << 8) + ((u32) k[2] << 16) + ((u32) k[3] << 24);
		b += k[4] + ((u32) k[5] << 8) + ((u32) k[6] << 16) + ((u32) k[7] << 24);
		c += k[8] + ((u32) k[9] << 8) + ((u32) k[10] << 16) + ((u32) k[11] << 24);
		jhash32_mix(a, b, c);
		k += 12;
		len -= 12;
	}

	c += keysize;
	switch (len) {
	case 11: c += (u32) k[10] << 24;
	case 10: c += (u32) k[9] << 16;
	case 9: c += (u32) k[8] << 8;
	case 8: b += (u32) k[7] << 24;
	case 7: b += (u32) k[6] << 16;
	case 6: b += (u32) k[5] << 8;
	case 5: b += k[4];
	case 4: a += (u32) k[3] << 24;
	case 3: a += (u32) k[2] << 16;
	case 2: a += (u32) k[1] << 8;
	case 1: a += k[0];
	case 0: break;
	}

	jhash32_mix(a, b, c);

	return c;
}

#define jhash64_mix(a, b, c) \
	{                        \
		a -= b;              \
		a -= c;              \
		a ^= c >> 43;        \
		b -= c;              \
		b -= a;              \
		b ^= a << 9;         \
		c -= a;              \
		c -= b;              \
		c ^= b >> 8;         \
		a -= b;              \
		a -= c;              \
		a ^= c >> 38;        \
		b -= c;              \
		b -= a;              \
		b ^= a << 23;        \
		c -= a;              \
		c -= b;              \
		c ^= b >> 5;         \
		a -= b;              \
		a -= c;              \
		a ^= c >> 35;        \
		b -= c;              \
		b -= a;              \
		b ^= a << 49;        \
		c -= a;              \
		c -= b;              \
		c ^= b >> 11;        \
		a -= b;              \
		a -= c;              \
		a ^= c >> 12;        \
		b -= c;              \
		b -= a;              \
		b ^= a << 18;        \
		c -= a;              \
		c -= b;              \
		c ^= b >> 22;        \
	}

u64 jhash64(const void *key, u64 keysize, u64 seed)
{
	const u8 *k = key;
	u64 a, b, c;
	u64 len = keysize;

	len = keysize;
	a = b = seed;
	c = 0x9e3779b97f4a7c13LL;

	while (keysize >= 24) {
		a += k[0] + ((u64) k[1] << 8) + ((u64) k[2] << 16) + ((u64) k[3] << 24) + ((u64) k[4] << 32) + ((u64) k[5] << 40) + ((u64) k[6] << 48) + ((u64) k[7] << 56);
		b += k[8] + ((u64) k[9] << 8) + ((u64) k[10] << 16) + ((u64) k[11] << 24) + ((u64) k[12] << 32) + ((u64) k[13] << 40) + ((u64) k[14] << 48) + ((u64) k[15] << 56);
		c += k[16] + ((u64) k[17] << 8) + ((u64) k[18] << 16) + ((u64) k[19] << 24) + ((u64) k[20] << 32) + ((u64) k[21] << 40) + ((u64) k[22] << 48) + ((u64) k[23] << 56);
		jhash64_mix(a, b, c);
		k += 24;
		len -= 24;
	}

	c += keysize;
	switch (len) {
	case 23: c += (u64) k[22] << 56;
	case 22: c += (u64) k[21] << 48;
	case 21: c += (u64) k[20] << 40;
	case 20: c += (u64) k[19] << 32;
	case 19: c += (u64) k[18] << 24;
	case 18: c += (u64) k[17] << 16;
	case 17: c += (u64) k[16] << 8;
	case 16: b += (u64) k[15] << 56;
	case 15: b += (u64) k[14] << 48;
	case 14: b += (u64) k[13] << 40;
	case 13: b += (u64) k[12] << 32;
	case 12: b += (u64) k[11] << 24;
	case 11: b += (u64) k[10] << 16;
	case 10: b += (u64) k[9] << 8;
	case 9: b += (u64) k[8];
	case 8: a += (u64) k[7] << 56;
	case 7: a += (u64) k[6] << 48;
	case 6: a += (u64) k[5] << 40;
	case 5: a += (u64) k[4] << 32;
	case 4: a += (u64) k[3] << 24;
	case 3: a += (u64) k[2] << 16;
	case 2: a += (u64) k[1] << 8;
	case 1: a += (u64) k[0];
	case 0: break;
	}

	jhash64_mix(a, b, c);

	return c;
}

usize jhash(const void *key, usize keysize, usize seed)
{
#if __WORDSIZE == 64
	return jhash64(key, keysize, seed);
#else
	return jhash32(key, keysize, seed);
#endif
}

#define HALF_SIPHASH_CROUNDS 1
#define HALF_SIPHASH_DROUNDS 3

#define HALF_SIPROUND        \
	do {                     \
		v0 += v1;            \
		v1 = rotl32(v1, 5);  \
		v1 ^= v0;            \
		v0 = rotl32(v0, 16); \
		v2 += v3;            \
		v3 = rotl32(v3, 8);  \
		v3 ^= v2;            \
		v0 += v3;            \
		v3 = rotl32(v3, 7);  \
		v3 ^= v0;            \
		v2 += v1;            \
		v1 = rotl32(v1, 13); \
		v1 ^= v2;            \
		v2 = rotl32(v2, 16); \
	} while (0)

u32 siphash32(const void *key, u32 keysize, u32 seed)
{
	const u8 *d = key;

	u32 v0 = 0;
	u32 v1 = 0;
	u32 v2 = 0x6c796765;
	u32 v3 = 0x74656462;

	u32 k = seed;
	u32 m;

	i32 i;
	const u8 *end = d + keysize - (keysize & 3);
	const i32 left = keysize & 3;
	u32 b = keysize << 24;

	v2 ^= k;
	v0 ^= k;

	for (; d != end; d += 4) {
		m = *((u32 *) d);
		v3 ^= m;

		for (i = 0; i < HALF_SIPHASH_CROUNDS; ++i)
			HALF_SIPROUND;

		v0 ^= m;
	}

	switch (left) {
	case 3:
		b |= (u32) d[2] << 16;
	case 2:
		b |= (u32) d[1] << 8;
	case 1:
		b |= (u32) d[0];
		break;
	case 0:
		break;
	}

	v3 ^= b;

	for (i = 0; i < HALF_SIPHASH_CROUNDS; ++i)
		HALF_SIPROUND;

	v0 ^= b;
	v2 ^= 0xff;

	for (i = 0; i < HALF_SIPHASH_DROUNDS; ++i)
		HALF_SIPROUND;

	b = v1 ^ v3;

	return b;
}

#define SIPHASH_CROUNDS 1
#define SIPHASH_DROUNDS 3

#define SIPROUND             \
	do {                     \
		v0 += v1;            \
		v1 = rotl64(v1, 13); \
		v1 ^= v0;            \
		v0 = rotl64(v0, 32); \
		v2 += v3;            \
		v3 = rotl64(v3, 16); \
		v3 ^= v2;            \
		v0 += v3;            \
		v3 = rotl64(v3, 21); \
		v3 ^= v0;            \
		v2 += v1;            \
		v1 = rotl64(v1, 17); \
		v1 ^= v2;            \
		v2 = rotl64(v2, 32); \
	} while (0)

u64 siphash64(const void *key, u64 keysize, u64 seed)
{
	const u8 *d = key;

	u64 v0 = U64_C(0x736f6d6570736575);
	u64 v1 = U64_C(0x646f72616e646f6d);
	u64 v2 = U64_C(0x6c7967656e657261);
	u64 v3 = U64_C(0x7465646279746573);

	u64 k = seed;
	u64 m;

	u32 i;
	const unsigned char *end = d + keysize - (keysize & 7);
	const int left = keysize & 7;
	u64 b = keysize << 56;

	v2 ^= k;
	v0 ^= k;

	for (; d != end; d += 8) {
		m = *((u64 *) d);
		v3 ^= m;

		for (i = 0; i < SIPHASH_CROUNDS; ++i)
			SIPROUND;

		v0 ^= m;
	}

	switch (left) {
	case 7: b |= ((u64) d[6]) << 48;
	case 6: b |= ((u64) d[5]) << 40;
	case 5: b |= ((u64) d[4]) << 32;
	case 4: b |= ((u64) d[3]) << 24;
	case 3: b |= ((u64) d[2]) << 16;
	case 2: b |= ((u64) d[1]) << 8;
	case 1: b |= ((u64) d[0]);
	case 0: break;
	}

	v3 ^= b;

	for (i = 0; i < SIPHASH_CROUNDS; ++i)
		SIPROUND;

	v0 ^= b;
	v2 ^= 0xff;

	for (i = 0; i < SIPHASH_DROUNDS; ++i)
		SIPROUND;

	b = v0 ^ v1 ^ v2 ^ v3;

	return b;
}

usize siphash(const void *key, usize keysize, usize seed)
{
#if __WORDSIZE == 64
	return siphash64(key, keysize, seed);
#else
	return siphash32(key, keysize, seed);
#endif
}
