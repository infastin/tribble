#ifndef MATH_H_5PNMJXKW
#define MATH_H_5PNMJXKW

#include "Types.h"

/**
 * div32:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `i32`.
 **/
typedef struct {
	i32 quot;
	i32 rem;
} div32;

/**
 * div64:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `i64`.
 **/
typedef struct {
	i64 quot;
	i64 rem;
} div64;

/**
 * divsize:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `isize`.
 **/
typedef struct {
	isize quot;
	isize rem;
} divsize;

/**
 * udiv32:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `u32`.
 **/
typedef struct {
	u32 quot;
	u32 rem;
} udiv32;

/**
 * udiv64:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `u64`.
 **/
typedef struct {
	u64 quot;
	u64 rem;
} udiv64;

/**
 * udivsize:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `usize`.
 **/
typedef struct {
	usize quot;
	usize rem;
} udivsize;

/**
 * trb_abs_8:
 * @num: An `i8` integer.
 *
 * Gets the absolute value of @num.
 *
 * Returns: The absolute value of @num as an `u8`.
 **/
u8 trb_abs_8(i8 num);

/**
 * trb_abs_16:
 * @num: An `i16` integer.
 *
 * Gets the absolute value of @num.
 *
 * Returns: The absolute value of @num as an `u16`.
 **/
u16 trb_abs_16(i16 num);

/**
 * trb_abs_32:
 * @num: An `i32` integer.
 *
 * Gets the absolute value of @num.
 *
 * Returns: The absolute value of @num as an `u32`.
 **/
u32 trb_abs_32(i32 num);

/**
 * trb_abs_64:
 * @num: An `i64` integer.
 *
 * Gets the absolute value of @num.
 *
 * Returns: The absolute value of @num as an `u64`.
 **/
u64 trb_abs_64(i64 num);

/**
 * trb_abs_size:
 * @num: An `isize` integer.
 *
 * Gets the absolute value of @num.
 *
 * Returns: The absolute value of @num as an `usize`.
 **/
usize trb_abs_size(isize num);

// clang-format off
/**
 * trb_abs:
 * @num: An integer.
 *
 * Gets the absolute value of @num.
 **/
#define trb_abs(num) _Generic((num), \
	i8: trb_abs_8, \
	i16: trb_abs_16, \
	i32: trb_abs_32, \
	i64: trb_abs_64 \
)(num)
// clang-format on

/**
 * trb_pow2_32:
 * @value: The 32-bit integer.
 *
 * Rounds up to the next power of 2.
 *
 * Returns: The next power of 2.
 **/
u32 trb_pow2_32(u32 value);

/**
 * trb_pow2_64:
 * @value: The 64-bit integer.
 *
 * Rounds up to the next power of 2.
 *
 * Returns: The next power of 2.
 **/
u64 trb_pow2_64(u64 value);

/**
 * trb_pow2_size:
 * @value: The 32/64-bit integer.
 *
 * Rounds up to the next power of 2.
 *
 * Returns: The next power of 2.
 **/
usize trb_pow2_size(usize value);

// clang-format off
/**
 * trb_pow2:
 * @value: The 32/64-bit integer.
 *
 * Rounds up to the next power of 2.
 **/
#define trb_pow2(value) _Generic((value), \
	u32: trb_pow2_32, \
	u64: trb_pow2_64 \
)(num)
// clang-format on

/**
 * trb_mul_pow2_32:
 * @num: The 32-bit number.
 * @factor: The 32-bit multiplier which is a power of two.
 *
 * Multiplies the number by a power of two.
 *
 * Returns: A product.
 **/
u32 trb_mul_pow2_32(u32 num, u32 factor);

/**
 * trb_mul_pow2_64:
 * @num: The 64-bit number.
 * @factor: The 64-bit multiplier which is a power of two.
 *
 * Multiplies the number by a power of two.
 *
 * Returns: A product.
 **/
u64 trb_mul_pow2_64(u64 num, u64 factor);

/**
 * trb_mul_pow2_size:
 * @num: The 32/64-bit number.
 * @factor: The 32/64-bit multiplier which is a power of two.
 *
 * Multiplies the number by a power of two.
 *
 * Returns: A product.
 **/
usize trb_mul_pow2_size(usize num, usize factor);

// clang-format off
/**
 * trb_mul_pow2:
 * @num: The 32/64-bit number.
 * @factor: The 32/64-bit multiplier which is a power of two.
 *
 * Multiplies the number by a power of two.
 **/
#define trb_mul_pow2(num, factor) _Generic((num), \
	u32: trb_mul_pow2_32, \
	u64: trb_mul_pow2_64 \
)(num, factor)
// clang-format on

/**
 * trb_div_pow2_32:
 * @num: The 32-bit dividend.
 * @denom: The 32-bit divisor which is a power of two.
 *
 * Divides the number by a power of two.
 *
 * Returns: A quotient and remainder.
 **/
udiv32 trb_div_pow2_32(u32 num, u32 denom);

/**
 * trb_div_pow2_64:
 * @num: The 64-bit dividend.
 * @denom: The 64-bit divisor which is a power of two.
 *
 * Divides the number by a power of two.
 *
 * Returns: A quotient and a remainder.
 **/
udiv64 trb_div_pow2_64(u64 num, u64 denom);

/**
 * trb_div_pow2_size:
 * @num: The 32/64-bit dividend.
 * @denom: The 32/64-bit divisor which is a power of two.
 *
 * Divides the number by a power of two.
 *
 * Returns: A quotient and remainder.
 **/
udivsize trb_div_pow2_size(usize num, usize denom);

// clang-format off
/**
 * trb_div_pow2:
 * @num: The 32/64-bit dividend.
 * @denom: The 32/64-bit divisor which is a power of two.
 *
 * Divides the number by a power of two.
 * Returns a quotient and remainder.
 **/
#define trb_div_pow2(num, denom) _Generic((num), \
	u32: trb_div_pow2_32, \
	u64: trb_div_pow2_64 \
)(num, denom)
// clang-format on

/**
 * trb_clz32:
 * @num: An `u32` integer.
 *
 * Counts the number of leading 0-bits in @num,
 * starting at the most significant bit position.
 * Returns the number of leading 0-bits.
 **/
#define trb_clz32(num) ((num) ? __builtin_clz(num) : 32)

/**
 * trb_clz64:
 * @num: An `u64` integer.
 *
 * Counts the number of leading 0-bits in @num,
 * starting at the most significant bit position.
 * Returns the number of leading 0-bits.
 **/
#define trb_clz64(num) ((num) ? __builtin_clzll(num) : 64)

/**
 * trb_clz:
 * @num: An `usize` integer.
 *
 * Counts the number of leading 0-bits in @num,
 * starting at the most significant bit position.
 * Returns the number of leading 0-bits.
 **/
#if USIZE_WIDTH == 64
	#define trb_clz(num) trb_clz64(num)
#else
	#define trb_clz(num) trb_clz32(num)
#endif

/**
 * trb_rotl32:
 * @x: The 32-bit integer to be rotated.
 * @r: The rotation count.
 *
 * Rotates @x by @r bits to the left.
 * Returns a rotated 32-bit integer.
 **/
#define trb_rotl32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))

/**
 * trb_rotr32:
 * @x: The 32-bit integer to be rotated.
 * @r: The rotation count.
 *
 * Rotates @x by @r bits to the right.
 * Returnsa a rotated 32-bit integer.
 **/
#define trb_rotr32(x, r) (((x) >> (r)) | ((x) << (32 - (r))))

/**
 * trb_rotl64:
 * @x: The 64-bit integer to be rotated.
 * @r: The rotation count.
 *
 * Rotates @x by @r bits to the left.
 * Returns a rotated 64-bit integer.
 **/
#define trb_rotl64(x, r) (((x) << (r)) | ((x) >> (64 - (r))))

/**
 * trb_rotr64:
 * @x: The 64-bit integer to be rotated.
 * @r: The rotation count.
 *
 * Rotates @x by @r bits to the right.
 * Returns a rotated 64-bit integer.
 **/
#define trb_rotr64(x, r) (((x) >> (r)) | ((x) << (64 - (r))))

/**
 * trb_rotr:
 * @x: The 32/64-bit integer to be rotated.
 * @r: The rotation count.
 *
 * Rotates @x by @r bits to the right.
 * Returns a rotated 32/64-bit integer.
 **/
#if USIZE_WIDTH == 64
	#define trb_rotr(num) trb_rotr64(num)
#else
	#define trb_rotr(num) trb_rotr32(num)
#endif

/**
 * trb_rotl:
 * @x: The 32/64-bit integer to be rotated.
 * @r: The rotation count.
 *
 * Rotates @x by @r bits to the left.
 * Returns a rotated 32/64-bit integer.
 **/
#if USIZE_WIDTH == 64
	#define trb_rotl(num) trb_rotl64(num)
#else
	#define trb_rotl(num) trb_rotl32(num)
#endif

/**
 * trb_max:
 * @a: The first value.
 * @b: The second value.
 *
 * Chooses the largest value between @a and @b.
 **/
#define trb_max(a, b) (((a) > (b)) ? (a) : (b))

/**
 * trb_min:
 * @a: The first value.
 * @b: The second value.
 *
 * Chooses the lowest value between @a and @b.
 **/
#define trb_min(a, b) (((a) < (b)) ? (a) : (b))

#endif /* end of include guard: MATH_H_5PNMJXKW */
