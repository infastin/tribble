#ifndef DEFINITIONS_H_N6TPGG3D
#define DEFINITIONS_H_N6TPGG3D

#include <limits.h>
#include <stdint.h>

/**
 * i8:
 * A signed 8-bit integer type.
 **/
typedef int8_t i8;

/**
 * i16:
 * A signed 16-bit integer type.
 **/
typedef int16_t i16;

/**
 * i32:
 * A signed 32-bit integer type.
 **/
typedef int32_t i32;

/**
 * i64:
 * A signed 64-bit integer type.
 **/
typedef int64_t i64;

/**
 * isize:
 * A signed pointer size integer type.
 **/
typedef intptr_t isize;

/**
 * u8:
 * An unsigned 8-bit integer type.
 **/
typedef uint8_t u8;

/**
 * u16:
 * An unsigned 16-bit integer type.
 **/
typedef uint16_t u16;

/**
 * u32:
 * An unsigned 32-bit integer type.
 **/
typedef uint32_t u32;

/**
 * u64:
 * An unsigned 64-bit integer type.
 **/
typedef uint64_t u64;

/**
 * usize:
 * An unsigned pointer size integer type.
 **/
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

#define I8_WIDTH INT8_WIDTH
#define I16_WIDTH INT16_WIDTH
#define I32_WIDTH INT32_WIDTH
#define I64_WIDTH INT64_WIDTH
#define ISIZE_WIDTH INTPTR_WIDTH

#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX
#define USIZE_MAX UINTPTR_MAX

#define U8_C(c) UINT8_C(c)
#define U16_C(C) UINT16_C(c)
#define U32_C(C) UINT32_C(c)
#define U64_C(c) UINT64_C(c)

#define U8_WIDTH UINT8_WIDTH
#define U16_WIDTH UINT16_WIDTH
#define U32_WIDTH UINT32_WIDTH
#define U64_WIDTH UINT64_WIDTH
#define USIZE_WIDTH UINTPTR_WIDTH

#define TRB_STRFUNC ((const char *) (__PRETTY_FUNCTION__))
#define TRB_STRFILE ((const char *) (__FILE__))
#define TRB_USIZELINE ((usize) (__LINE__))

#define TRB_FORMAT(archetype, index, first) __attribute__((format(archetype, index, first)))
#define TRB_UNUSED __attribute__((unused))
#define TRB_PACKED __attribute__((packed))

/**
 * f32:
 * A single precision floating-point number type.
 **/
typedef float f32;

/**
 * f64:
 * A double precision floating-point number type.
 **/
typedef double f64;

/**
 * real:
 * A quadruple precision floating-point number type.
 **/
typedef long double real;

#ifndef bool
typedef _Bool bool;
#endif

#define TRUE 1
#define FALSE 0

#ifndef NULL
	#define NULL ((void *) 0)
#endif

#ifndef fallthrough
	#define fallthrough [[fallthrough]]
#endif

/**
 * TrbCmpFunc:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 *
 * The function for comparing two values.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
typedef i32 (*TrbCmpFunc)(const void *a, const void *b);

/**
 * TrbCmpDataFunc:
 * @a: The first value to be compared.
 * @b: The second value to be compared.
 * @data: User data.
 *
 * The function for comparing two values using user data.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
typedef i32 (*TrbCmpDataFunc)(const void *a, const void *b, void *data);

typedef void *(*TrbCopyFunc)(const void *src);

typedef void (*TrbFreeFunc)(void *ptr);

typedef void (*TrbUserFunc)(void *data, void *userdata);

/**
 * TrbHashFunc:
 * @key: (not nullable): The key to be hashed.
 * @keysize: The size of the key.
 * @seed: The seed for hashing.
 *
 * The function for hashing keys.
 *
 * Returns: A hash value of the @key.
 **/
typedef usize (*TrbHashFunc)(const void *key, usize keysize, usize seed);

#endif /* end of include guard: DEFINITIONS_H_N6TPGG3D */
