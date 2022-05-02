#ifndef DEFINITIONS_H_N6TPGG3D
#define DEFINITIONS_H_N6TPGG3D

#include <limits.h>
#include <stdint.h>

/* Integers */

typedef uintptr_t size_t;
typedef uintptr_t word_t;

/* Boolean */

#ifdef bool
	#undef bool
#endif

typedef _Bool bool;

#define TRUE 1
#define FALSE 0

/* Functions */

typedef int32_t (*CmpFunc)(const void *a, const void *b);
typedef void *(*CopyFunc)(const void *src);
typedef void (*FreeFunc)(void *ptr);
typedef void (*UserFunc)(void *data, void *userdata);
typedef uint32_t (*HashFunc)(const void *key, uint32_t keysize, uint32_t seed);

#endif /* end of include guard: DEFINITIONS_H_N6TPGG3D */
