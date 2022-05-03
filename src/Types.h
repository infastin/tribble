#ifndef DEFINITIONS_H_N6TPGG3D
#define DEFINITIONS_H_N6TPGG3D

#include <limits.h>
#include <stdint.h>

/* Integers */
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef intptr_t isize;
typedef uintptr_t usize;

/* Floating point numbers */
typedef float f32;
typedef double f64;
typedef long double f128;

/* Boolean */
#ifdef bool
	#undef bool
#endif

typedef _Bool bool;

#define TRUE 1
#define FALSE 0

/* Functions */
typedef i32 (*CmpFunc)(const void *a, const void *b);
typedef void *(*CopyFunc)(const void *src);
typedef void (*FreeFunc)(void *ptr);
typedef void (*UserFunc)(void *data, void *userdata);
typedef u32 (*HashFunc)(const void *key, u32 keysize, u32 seed);

#endif /* end of include guard: DEFINITIONS_H_N6TPGG3D */
