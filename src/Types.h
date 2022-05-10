#ifndef DEFINITIONS_H_N6TPGG3D
#define DEFINITIONS_H_N6TPGG3D

#include <limits.h>
#include <stdint.h>

/* Integers */
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef intptr_t isize;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintptr_t usize;

#define I8_MAX INT8_MAX
#define I8_MIN INT8_MIN
#define I16_MAX INT16_MAX
#define I16_MIN INT16_MIN
#define I32_MAX INT32_MAX
#define I32_MIN INT32_MIN
#define I64_MAX INT64_MAX
#define I64_MIN INT64_MIN
#define ISIZE_MIN INTPTR_MIN
#define ISIZE_MAX INTPTR_MAX

#define I8_C(c) INT8_C(c)
#define I16_C(C) INT16_C(c)
#define I32_C(C) INT32_C(c)
#define I64_C(c) INT64_C(c)

#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX
#define USIZE_MAX UINTPTR_MAX

#define U8_C(c) UINT8_C(c)
#define U16_C(C) UINT16_C(c)
#define U32_C(C) UINT32_C(c)
#define U64_C(c) UINT64_C(c)

/* Floating point numbers */
typedef float f32;
typedef double f64;
typedef long double f128;

/* Boolean */
#ifndef bool
typedef _Bool bool;
#endif

#define TRUE 1
#define FALSE 0

/* Functions */

/**
 * CmpFunc:
 * @a: The first value to be compared (can be `NULL`).
 * @b: The second value to be compared (can be `NULL`).
 *
 * The function for comparing two values.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
typedef i32 (*CmpFunc)(const void *a, const void *b);

typedef void *(*CopyFunc)(const void *src);
typedef void (*FreeFunc)(void *ptr);
typedef void (*UserFunc)(void *data, void *userdata);
typedef u32 (*HashFunc)(const void *key, usize keysize, usize seed);

#endif /* end of include guard: DEFINITIONS_H_N6TPGG3D */
