#ifndef HASH_H_RKAMEI83
#define HASH_H_RKAMEI83

#include "Types.h"

/**
 * Reference: https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp#L94
 */
uint32_t MurmurHash3(const void *key, uint32_t keysize, uint32_t seed);

#endif /* end of include guard: HASH_H_RKAMEI83 */
