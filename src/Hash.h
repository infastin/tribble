#ifndef HASH_H_RKAMEI83
#define HASH_H_RKAMEI83

#include "Types.h"

/**
 * trb_murmurhash3_32:
 * @key: (not nullable): The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.
 *
 * MurmurHash3 x86 32.
 * [Reference](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp#L94).
 *
 * Returns: The hash of the key.
 **/
u32 trb_murmurhash3_32(const void *key, u32 keysize, u32 seed);

/**
 * trb_murmurhash3_64:
 * @key: (not nullable): The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.
 *
 * MurmurHash3 x64 128, but we discard the upper 64 bits.
 * [Reference](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp#L94).
 *
 * Returns: The hash of the key.
 **/
u64 trb_murmurhash3_64(const void *key, u64 keysize, u64 seed);

/**
 * trb_murmurhash3:
 * @key: (not nullable): The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.
 *
 * Uses trb_murmurhash3_32() on 32-bit platform and trb_murmurhash3_64() on 64-bit platform.
 *
 * Returns: The hash of the key.
 **/
usize trb_murmurhash3(const void *key, usize keysize, usize seed);

/**
 * trb_jhash32:
 * @key: (not nullable): The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.

 * Jenkins Hash 32-bit.
 * [Reference](http://burtleburtle.net/bob/c/lookup2.c).
 *
 * Returns: The hash of the key.
 **/
u32 trb_jhash32(const void *key, u32 keysize, u32 seed);

/**
 * trb_jhash64:
 * @key: (not nullable): The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.

 * Jenkins Hash 64-bit.
 * [Reference](http://burtleburtle.net/bob/c/lookup8.c).
 *
 * Returns: The hash of the key.
 **/
u64 trb_jhash64(const void *key, u64 keysize, u64 seed);

/**
 * trb_jhash:
 * @key: (not nullable): The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.
 *
 * Uses trb_jhash32() on 32-bit platform and trb_jhash64() on 64-bit platform.
 *
 * Returns: The hash of the key.
 **/
usize trb_jhash(const void *key, usize keysize, usize seed);

/**
 * trb_siphash32:
 * @key: (not nullable): The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.

 * Half SipHash 1-3.
 * [Reference](https://github.com/veorq/SipHash/blob/master/halftrb_siphash.c).
 *
 * Returns: The hash of the key.
 **/
u32 trb_siphash32(const void *key, u32 keysize, u32 seed);

/**
 * trb_siphash64:
 * @key: (not nullable): The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.

 * SipHash 1-3.
 * [Reference](https://github.com/veorq/SipHash/blob/master/trb_siphash.c).
 *
 * Returns: The hash of the key.
 **/
u64 trb_siphash64(const void *key, u64 keysize, u64 seed);

/**
 * trb_siphash:
 * @key: (not nullable): The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.
 *
 * Uses trb_siphash32() on 32-bit platform and trb_siphash64() on 64-bit platform.
 *
 * Returns: The hash of the key.
 **/
usize trb_siphash(const void *key, usize keysize, usize seed);

#endif /* end of include guard: HASH_H_RKAMEI83 */
