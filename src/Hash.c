#include "Hash.h"

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
		u32 k1 = blocks[i];

		k1 *= c1;
		k1 = ROTL32(k1, 15);
		k1 *= c2;

		h ^= k1;
		h = ROTL32(h, 13);
		h = h * 5 + 0xe6546b64;
	}

	const u8 *tail = (const u8 *) (data + nblocks * 4);

	u32 k1 = 0;

	switch (keysize & 3) {
	case 3: k1 ^= tail[2] << 16;
	case 2: k1 ^= tail[1] << 8;
	case 1:
		k1 ^= tail[0];
		k1 *= c1;
		k1 = ROTL32(k1, 15);
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
	u32 a, b, c;
	const u8 *k = key;

	a = b = 0x9e3779b9;
	c = seed;

	while (len > 12) {
		a += k[0] + ((u32) k[1] << 8) + ((u32) k[2] << 16) + ((u32) k[3] << 24);
		b += k[4] + ((u32) k[5] << 8) + ((u32) k[6] << 16) + ((u32) k[7] << 24);
		c += k[8] + ((u32) k[9] << 8) + ((u32) k[10] << 16) + ((u32) k[11] << 24);
		jhash_mix(a, b, c);
		k += 12;
		len -= 12;
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
