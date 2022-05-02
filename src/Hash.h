#ifndef HASH_H_RKAMEI83
#define HASH_H_RKAMEI83

#include "Types.h"

/**
 * MurmurHash3
 * Reference: https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp#L94
 */
u32 murmur_hash3(const void *key, u32 keysize, u32 seed);

/**
 * Jenkins Hash
 * Reference: http://burtleburtle.net/bob/hash/evahash.html
 */
u32 jhash(const void *key, u32 len, u32 seed);

/**
 * Half SipHash 1-3
 * Reference: https://github.com/veorq/SipHash/blob/master/halfsiphash.c
 */
u32 half_siphash(const void *key, u32 len, u32 seed);

#endif /* end of include guard: HASH_H_RKAMEI83 */
