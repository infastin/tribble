#ifndef HASH_H_RKAMEI83
#define HASH_H_RKAMEI83

#include "Types.h"

/**
 * @brief MurmurHash3 x86 32.
 * @details https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp#L94
 */
u32 murmurhash3_32(const void *key, u32 keysize, u32 seed);

/**
 * @brief MurmurHash3 x64 128, but we discard the upper 64 bits.
 * @details https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp#L94
 */
u64 murmurhash3_64(const void *key, u64 keysize, u64 seed);

usize murmurhash3(const void *key, usize keysize, usize seed);

/**
 * @brief Jenkins Hash 32-bit.
 * @details http://burtleburtle.net/bob/c/lookup2.c
 */
u32 jhash32(const void *key, u32 keysize, u32 seed);

/**
 * @brief Jenkins Hash 64-bit.
 * @details http://burtleburtle.net/bob/c/lookup8.c
 */
u64 jhash64(const void *key, u64 keysize, u64 seed);

usize jhash(const void *key, usize keysize, usize seed);

/**
 * @brief Half SipHash 1-3.
 * @details https://github.com/veorq/SipHash/blob/master/halfsiphash.c
 */
u32 siphash32(const void *key, u32 keysize, u32 seed);

/**
 * @brief SipHash 1-3.
 * @details https://github.com/veorq/SipHash/blob/master/siphash.c
 */
u64 siphash64(const void *key, u64 keysize, u64 seed);

usize siphash(const void *key, usize keysize, usize seed);

#endif /* end of include guard: HASH_H_RKAMEI83 */
