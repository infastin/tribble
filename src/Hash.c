#include "Hash.h"

#include "Macros.h"

#define ROTL32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))
#define ROTR32(x, r) (((x) >> (r)) | ((x) << (32 - (r))))

u32 murmur_hash3(const void *key, u32 keysize, u32 seed)
{
	const u8 *data = (const u8 *) key;
	const i32 nblocks = keysize / 4;

	u32 h = seed;

	const u32 c1 = 0xcc9e2d51;
	const u32 c2 = 0x1b873593;

	const u32 *blocks = (const u32 *) (data + nblocks * 4);

	for (i32 i = -nblocks; i; i++) {
		u32 k = blocks[i];

		k *= c1;
		k = ROTL32(k, 15);
		k *= c2;

		h ^= k;
		h = ROTL32(h, 13);
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
		k = ROTL32(k, 15);
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

#define jhash_mix(a, b, c) \
	{                      \
		a -= b;            \
		a -= c;            \
		a ^= c >> 13;      \
		b -= c;            \
		b -= a;            \
		b ^= a << 8;       \
		c -= a;            \
		c -= b;            \
		c ^= b >> 13;      \
		a -= b;            \
		a -= c;            \
		a ^= c >> 12;      \
		b -= c;            \
		b -= a;            \
		b ^= a << 16;      \
		c -= a;            \
		c -= b;            \
		c ^= b >> 5;       \
		a -= b;            \
		a -= c;            \
		a ^= c >> 3;       \
		b -= c;            \
		b -= a;            \
		b ^= a << 10;      \
		c -= a;            \
		c -= b;            \
		c ^= b >> 15;      \
	}

u32 jhash(const void *key, u32 len, u32 seed)
{
	const u8 *k = key;
	u32 a, b, c;
	u32 l = len;

	a = b = 0x9e3779b9;
	c = seed;

	while (l >= 12) {
		a += k[0] + ((u32) k[1] << 8) + ((u32) k[2] << 16) + ((u32) k[3] << 24);
		b += k[4] + ((u32) k[5] << 8) + ((u32) k[6] << 16) + ((u32) k[7] << 24);
		c += k[8] + ((u32) k[9] << 8) + ((u32) k[10] << 16) + ((u32) k[11] << 24);
		jhash_mix(a, b, c);
		k += 12;
		l -= 12;
	}

	c += len;
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

	jhash_mix(a, b, c);

	return c;
}

#define HALF_SIPHASH_CROUNDS 1
#define HALF_SIPHASH_DROUNDS 3

#define HALF_SIPROUND        \
	do {                     \
		v0 += v1;            \
		v1 = ROTL32(v1, 5);  \
		v1 ^= v0;            \
		v0 = ROTL32(v0, 16); \
		v2 += v3;            \
		v3 = ROTL32(v3, 8);  \
		v3 ^= v2;            \
		v0 += v3;            \
		v3 = ROTL32(v3, 7);  \
		v3 ^= v0;            \
		v2 += v1;            \
		v1 = ROTL32(v1, 13); \
		v1 ^= v2;            \
		v2 = ROTL32(v2, 16); \
	} while (0)

u32 half_siphash(const void *key, u32 len, u32 seed)
{
	const u8 *d = (const u8 *) key;

	u32 v0 = 0;
	u32 v1 = 0;
	u32 v2 = 0x6c796765;
	u32 v3 = 0x74656462;

	u32 k = seed;
	u32 m;

	i32 i;
	const u8 *end = d + len - (len & 3);
	const i32 left = len & 3;
	u32 b = len << 24;

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
