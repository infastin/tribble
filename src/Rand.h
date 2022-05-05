#ifndef RAND_H_B1PCU4KT
#define RAND_H_B1PCU4KT

#include "Types.h"

typedef struct _SplitMix64 SplitMix64;

struct _SplitMix64 {
	u64 s;
};

SplitMix64 *splitmix64_init(SplitMix64 *state, u64 seed);
u64 splitmix64_next(SplitMix64 *state);

typedef struct _Xoshiro128 Xoshiro128;

struct _Xoshiro128 {
	u64 s[2];
};

Xoshiro128 *xoshiro128_init(Xoshiro128 *state, u64 seed);
u32 xoshiro128_next(Xoshiro128 *state);

typedef struct _Xoshiro256 Xoshiro256;

struct _Xoshiro256 {
	u64 s[4];
};

Xoshiro256 *xoshiro256_init(Xoshiro256 *state, u64 seed);
u64 xoshiro256_next(Xoshiro256 *state);

#endif /* end of include guard: RAND_H_B1PCU4KT */
