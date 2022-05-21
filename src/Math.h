#ifndef MATH_H_5PNMJXKW
#define MATH_H_5PNMJXKW

#include "Types.h"

#ifdef __SIZEOF_INT128__
	#define HAS_INT128
#endif

#ifdef HAS_INT128
typedef __uint128_t u128;
typedef __int128_t i128;
#else
typedef struct {
	u64 hi;
	u64 lo;
} u128;

typedef struct {
	i64 hi;
	u64 lo;
} i128;
#endif

typedef struct {
	u128 quot;
	u128 rem;
} ud128;

u128 u128_add(u128 a, u128 b);

u128 u128_sub(u128 a, u128 b);

u128 u128_mul(u128 a, u128 b);

u128 u128_shl(u128 num, usize shift);

u128 u128_shr(u128 num, usize shift);

u128 u128_xor(u128 a, u128 b);

u128 u128_and(u128 a, u128 b);

u128 u128_or(u128 a, u128 b);

i32 u128_cmp(const u128 *a, const u128 *b);

i32 u128_clz(u128 num);

ud128 u128_div(u128 num, u128 denom);

/* I128 */

typedef struct {
	i128 quot;
	i128 rem;
} d128;

i128 i128_add(i128 a, i128 b);

i128 i128_sub(i128 a, i128 b);

i128 i128_mul(i128 a, i128 b);

i128 i128_shl(i128 num, usize shift);

i128 i128_shr(i128 num, usize shift);

i128 i128_xor(i128 a, i128 b);

i128 i128_and(i128 a, i128 b);

i128 i128_or(i128 a, i128 b);

i32 i128_cmp(const i128 *a, const i128 *b);

i128 i128_minus(i128 num);

i128 i128_neg(i128 num);

d128 i128_div(i128 num, i128 denom);

#define U128_WIDTH 128
#define I128_WIDTH 128

#ifdef HAS_INT128
	#define U128_MAX (((u128) U64_MAX << 64) | ((u128) U64_MAX))
	#define U128_C(hi, lo) (((u128) U64_C(hi) << 64) | ((u128) U64_C(lo)))

	#define U128_HI(num) ((u64) ((num) >> 64))
	#define U128_LO(num) ((u64) ((num) & (U64_MAX)))

	#define I128_MAX (((i128) I64_MAX << 64) | ((i128) U64_MAX))
	#define I128_MIN ((i128) I64_MIN << 64)
	#define I128_C(hi, lo) (((i128) I64_C(hi) << 64) | ((i128) U64_C(lo)))

	#define I128_HI(num) ((u64) ((num) >> 64))
	#define I128_LO(num) ((u64) ((num) & (U64_MAX)))
#else
	#define U128_MAX ((u128){ U64_MAX, U64_MAX })
	#define U128_C(hi, lo) ((u128){ hi, lo })

	#define U128_HI(num) ((num).hi)
	#define U128_LO(num) ((num).lo)

	#define I128_MAX ((i128){ I64_MAX, U64_MAX })
	#define I128_MIN ((i128){ I64_MIN, 0 })
	#define I128_C(hi, lo) ((i128){ hi, lo })

	#define I128_HI(num) ((num).hi)
	#define I128_LO(num) ((num).lo)
#endif

/* Other stuff */
u32 trb_abs32(i32 num);

u64 trb_abs64(i64 num);

usize trb_abs(isize num);

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
 * trb_pow2:
 * @value: The 32/64-bit integer.
 *
 * Rounds up to the next power of 2.
 *
 * Returns: The next power of 2.
 **/
usize trb_pow2(usize value);

/**
 * trb_mul_pow2_32:
 * @num: The 32-bit number.
 * @denom: The 32-bit multiplier which is a power of two.
 *
 * Multiplies the number by a power of two.
 *
 * Returns: A product.
 **/
u32 trb_mul_pow2_32(u32 num, u32 factor);

/**
 * trb_mul_pow2_64:
 * @num: The 64-bit number.
 * @denom: The 64-bit multiplier which is a power of two.
 *
 * Multiplies the number by a power of two.
 *
 * Returns: A product.
 **/
u64 trb_mul_pow2_64(u64 num, u64 denom);

/**
 * trb_mul_pow2:
 * @num: The 32/64-bit number.
 * @denom: The 32/64-bit multiplier which is a power of two.
 *
 * Multiplies the number by a power of two.
 *
 * Returns: A product.
 **/
usize trb_mul_pow2(usize num, usize denom);

/**
 * trb_div_pow2_32:
 * @num: The 32-bit dividend.
 * @denom: The 32-bit divisor which is a power of two.
 *
 * Divides the number by a power of two.
 *
 * Returns: Quotient and reminder.
 **/
ud32 trb_div_pow2_32(u32 num, u32 denom);

/**
 * trb_div_pow2_64:
 * @num: The 64-bit dividend.
 * @denom: The 64-bit divisor which is a power of two.
 *
 * Divides the number by a power of two.
 *
 * Returns: Quotient and reminder.
 **/
ud64 trb_div_pow2_64(u64 num, u64 denom);

/**
 * trb_div_pow2:
 * @num: The 32/64-bit dividend.
 * @denom: The 32/64-bit divisor which is a power of two.
 *
 * Divides the number by a power of two.
 *
 * Returns: Quotient and reminder.
 **/
udsize trb_div_pow2(usize num, usize denom);

#define trb_clz32(num) ((num) ? __builtin_clz(num) : 32)
#define trb_clz64(num) ((num) ? __builtin_clzll(num) : 64)

#if USIZE_WIDTH == 64
	#define trb_clz(num) trb_clz64(num)
#else
	#define trb_clz(num) trb_clz32(num)
#endif

#define trb_rotl32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))
#define trb_rotr32(x, r) (((x) >> (r)) | ((x) << (32 - (r))))

#define trb_rotl64(x, r) (((x) << (r)) | ((x) >> (64 - (r))))
#define trb_rotr64(x, r) (((x) >> (r)) | ((x) << (64 - (r))))

#define trb_max(a, b) (((a) > (b)) ? (a) : (b))
#define trb_min(a, b) (((a) < (b)) ? (a) : (b))

#endif /* end of include guard: MATH_H_5PNMJXKW */
