#ifndef HASH_H_RKAMEI83
#define HASH_H_RKAMEI83

#include "Types.h"

/**
 * murmurhash3_32:
 * @key: The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.
 *
 * MurmurHash3 x86 32.
 * [Reference](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp#L94).
 *
 * Returns: The hash of the key.
 **/
u32 murmurhash3_32(const void *key, u32 keysize, u32 seed);

/**
 * murmurhash3_64:
 * @key: The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.
 *
 * MurmurHash3 x64 128, but we discard the upper 64 bits.
 * [Reference](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp#L94).
 *
 * Returns: The hash of the key.
 **/
u64 murmurhash3_64(const void *key, u64 keysize, u64 seed);

/**
 * murmurhash3:
 * @key: The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.
 *
 * Uses murmurhash3_32() on 32-bit platform and murmurhash3_64() on 64-bit platform.
 *
 * Returns: The hash of the key.
 **/
usize murmurhash3(const void *key, usize keysize, usize seed);

/**
 * jhash32:
 * @key: The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.

 * Jenkins Hash 32-bit.
 * [Reference](http://burtleburtle.net/bob/c/lookup2.c).
 *
 * Returns: The hash of the key.
 **/
u32 jhash32(const void *key, u32 keysize, u32 seed);

/**
 * jhash64:
 * @key: The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.

 * Jenkins Hash 64-bit.
 * [Reference](http://burtleburtle.net/bob/c/lookup8.c).
 *
 * Returns: The hash of the key.
 **/
u64 jhash64(const void *key, u64 keysize, u64 seed);

/**
 * jhash:
 * @key: The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.
 *
 * Uses jhash32() on 32-bit platform and jhash64() on 64-bit platform.
 *
 * Returns: The hash of the key.
 **/
usize jhash(const void *key, usize keysize, usize seed);

/**
 * siphash32:
 * @key: The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.

 * Half SipHash 1-3.
 * [Reference](https://github.com/veorq/SipHash/blob/master/halfsiphash.c).
 *
 * Returns: The hash of the key.
 **/
u32 siphash32(const void *key, u32 keysize, u32 seed);

/**
 * siphash64:
 * @key: The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.

 * SipHash 1-3.
 * [Reference](https://github.com/veorq/SipHash/blob/master/siphash.c).
 *
 * Returns: The hash of the key.
 **/
u64 siphash64(const void *key, u64 keysize, u64 seed);

/**
 * siphash:
 * @key: The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.
 *
 * Uses siphash32() on 32-bit platform and siphash64() on 64-bit platform.
 *
 * Returns: The hash of the key.
 **/
usize siphash(const void *key, usize keysize, usize seed);

#endif /* end of include guard: HASH_H_RKAMEI83 */
