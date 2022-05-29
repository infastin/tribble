#ifndef MATH_H_5PNMJXKW
#define MATH_H_5PNMJXKW

#include "Types.h"

#ifdef __SIZEOF_INT128__
	#define HAS_INT128
#endif

#undef HAS_INT128

#ifdef HAS_INT128
typedef __uint128_t u128;
typedef __int128_t i128;
#else
/**
 * u128:
 * @hi: An upper 64 bits of an interger.
 * @lo: A lower 64 bits of an interger.
 *
 * An unsigned 128-bit integer type.
 **/
typedef struct {
	u64 hi;
	u64 lo;
} u128;

/**
 * i128:
 * @hi: An upper 64 bits of an interger.
 * @lo: A lower 64 bits of an interger.
 *
 * A signed 128-bit integer type.
 **/
typedef struct {
	i64 hi;
	u64 lo;
} i128;
#endif

/**
 * ud128:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `u128`.
 **/
typedef struct {
	u128 quot;
	u128 rem;
} ud128;

/**
 * u128_add:
 * @a: The first operand.
 * @b: The second operand.
 *
 * Adds up @a and @b.
 *
 * Returns: A result of the operation @a + @b.
 **/
u128 u128_add(u128 a, u128 b);

/**
 * u128_sub:
 * @a: The first operand.
 * @b: The second operand.
 *
 * Subtracts @b from @a.
 *
 * Returns: A result of the operation @a - @b.
 **/
u128 u128_sub(u128 a, u128 b);

/**
 * u128_mul:
 * @a: The first operand.
 * @b: The second operand.
 *
 * Multiplies @a by @b.
 *
 * Returns: A result of the operation @a * @b.
 **/
u128 u128_mul(u128 a, u128 b);

/**
 * u128_shl:
 * @num: The first operand.
 * @shift: The second operand.
 *
 * Shifts @num by @shift bits to the left.
 *
 * Returns: A result of the operation @num << @shift.
 **/
u128 u128_shl(u128 num, usize shift);

/**
 * u128_shr:
 * @num: The first operand.
 * @shift: The second operand.
 *
 * Shifts @num by @shift bits to the right.
 *
 * Returns: A result of the operation @num >> @shift.
 **/
u128 u128_shr(u128 num, usize shift);

/**
 * u128_xor:
 * @a: The first operand.
 * @b: The second operand.
 *
 * Performs the logical XOR operation for @a and @b.
 *
 * Returns: A result of the operation @a ^ @b.
 **/
u128 u128_xor(u128 a, u128 b);

/**
 * u128_and:
 * @a: The first operand.
 * @b: The second operand.
 *
 * Performs the logical AND operation for @a and @b.
 *
 * Returns: A result of the operation @a & @b.
 **/
u128 u128_and(u128 a, u128 b);

/**
 * u128_or:
 * @a: The first operand.
 * @b: The second operand.
 *
 * Performs the logical OR operation for @a and @b.
 *
 * Returns: A result of the operation @a | @b.
 **/
u128 u128_or(u128 a, u128 b);

/**
 * u128_cmp:
 * @a: The first number to be compared.
 * @b: The second number to be compared.
 *
 * Compares two `u128`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 u128_cmp(const u128 *a, const u128 *b);

/**
 * u128_clz:
 * @num: An `u128` integer.
 *
 * Counts the number of leading 0-bits in @num,
 * starting at the most significant bit position
 *
 * Returns: The number of leading 0-bits.
 **/
i32 u128_clz(u128 num);

/**
 * u128_div:
 * @num: The dividend.
 * @denom: The divisor.
 *
 * Divides @num by @denom.
 *
 * Returns: A result of operations @num / @denom and @num % @denom.
 **/
ud128 u128_div(u128 num, u128 denom);

/**
 * d128:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `i128`.
 **/
typedef struct {
	i128 quot;
	i128 rem;
} d128;

/**
 * i128_add:
 * @a: The first operand.
 * @b: The second operand.
 *
 * Adds up @a and @b.
 *
 * Returns: A result of the operation @a + @b.
 **/
i128 i128_add(i128 a, i128 b);

/**
 * i128_sub:
 * @a: The first operand.
 * @b: The second operand.
 *
 * Subtracts @b from @a.
 *
 * Returns: A result of the operation @a - @b.
 **/
i128 i128_sub(i128 a, i128 b);

/**
 * i128_mul:
 * @a: The first operand.
 * @b: The second operand.
 *
 * Multiplies @a by @b.
 *
 * Returns: A result of the operation @a * @b.
 **/
i128 i128_mul(i128 a, i128 b);

/**
 * i128_shl:
 * @num: The first operand.
 * @shift: The second operand.
 *
 * Shifts @num by @shift bits to the left.
 *
 * Returns: A result of the operation @num << @shift.
 **/
i128 i128_shl(i128 num, usize shift);

/**
 * i128_shr:
 * @num: The first operand.
 * @shift: The second operand.
 *
 * Shifts @num by @shift bits to the right.
 *
 * Returns: A result of the operation @num >> @shift.
 **/
i128 i128_shr(i128 num, usize shift);

/**
 * i128_xor:
 * @a: The first operand.
 * @b: The second operand.
 *
 * Performs the logical XOR operation for @a and @b.
 *
 * Returns: A result of the operation @a ^ @b.
 **/
i128 i128_xor(i128 a, i128 b);

/**
 * i128_and:
 * @a: The first operand.
 * @b: The second operand.
 *
 * Performs the logical AND operation for @a and @b.
 *
 * Returns: A result of the operation @a & @b.
 **/
i128 i128_and(i128 a, i128 b);

/**
 * i128_or:
 * @a: The first operand.
 * @b: The second operand.
 *
 * Performs the logical OR operation for @a and @b.
 *
 * Returns: A result of the operation @a | @b.
 **/
i128 i128_or(i128 a, i128 b);

/**
 * i128_cmp:
 * @a: The first number to be compared.
 * @b: The second number to be compared.
 *
 * Compares two `i128`.
 *
 * Returns: -1 if @a < @b; 0 if @a == @b; 1 if @a > @b.
 **/
i32 i128_cmp(const i128 *a, const i128 *b);

/**
 * i128_neg:
 * @num: An `i128` integer.
 *
 * Performs minus unary operation for @num.
 *
 * Returns: A result of the operation -@num.
 **/
i128 i128_minus(i128 num);

/**
 * i128_not:
 * @num: An `i128` integer.
 *
 * Performs one's complement negation for @num.
 *
 * Returns: A result of the operation ~@num.
 **/
i128 i128_not(i128 num);

/**
 * i128_div:
 * @num: The dividend.
 * @denom: The divisor.
 *
 * Divides @num by @denom.
 *
 * Returns: A result of operations @num / @denom and @num % @denom.
 **/
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
	#define U128_C(hi, lo) ((u128){ U64_C(hi), U64_C(lo) })

	#define U128_HI(num) ((num).hi)
	#define U128_LO(num) ((num).lo)

	#define I128_MAX ((i128){ I64_MAX, U64_MAX })
	#define I128_MIN ((i128){ I64_MIN, 0 })
	#define I128_C(hi, lo) ((i128){ U64_C(hi), U64_C(lo) })

	#define I128_HI(num) ((num).hi)
	#define I128_LO(num) ((num).lo)
#endif

/**
 * d32:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `i32`.
 **/
typedef struct {
	i32 quot;
	i32 rem;
} d32;

/**
 * d64:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `i64`.
 **/
typedef struct {
	i64 quot;
	i64 rem;
} d64;

/**
 * dsize:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `isize`.
 **/
typedef struct {
	isize quot;
	isize rem;
} dsize;

/**
 * ud32:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `u32`.
 **/
typedef struct {
	u32 quot;
	u32 rem;
} ud32;

/**
 * ud64:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `u64`.
 **/
typedef struct {
	u64 quot;
	u64 rem;
} ud64;

/**
 * udsize:
 * @quot: A quotient.
 * @rem: A remainder.
 *
 * Stores the result of division of two `usize`.
 **/
typedef struct {
	usize quot;
	usize rem;
} udsize;

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
ud32 trb_div_pow2_32(u32 num, u32 denom);

/**
 * trb_div_pow2_64:
 * @num: The 64-bit dividend.
 * @denom: The 64-bit divisor which is a power of two.
 *
 * Divides the number by a power of two.
 *
 * Returns: A quotient and a remainder.
 **/
ud64 trb_div_pow2_64(u64 num, u64 denom);

/**
 * trb_div_pow2_size:
 * @num: The 32/64-bit dividend.
 * @denom: The 32/64-bit divisor which is a power of two.
 *
 * Divides the number by a power of two.
 *
 * Returns: A quotient and remainder.
 **/
udsize trb_div_pow2_size(usize num, usize denom);

// clang-format off
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
 * starting at the most significant bit position
 *
 * Returns: The number of leading 0-bits.
 **/
#define trb_clz32(num) ((num) ? __builtin_clz(num) : 32)

/**
 * trb_clz64:
 * @num: An `u64` integer.
 *
 * Counts the number of leading 0-bits in @num,
 * starting at the most significant bit position
 *
 * Returns: The number of leading 0-bits.
 **/
#define trb_clz64(num) ((num) ? __builtin_clzll(num) : 64)

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
 *
 * Returns: A rotated 32-bit integer.
 **/
#define trb_rotl32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))

/**
 * trb_rotr32:
 * @x: The 32-bit integer to be rotated.
 * @r: The rotation count.
 *
 * Rotates @x by @r bits to the right.
 *
 * Returns: A rotated 32-bit integer.
 **/
#define trb_rotr32(x, r) (((x) >> (r)) | ((x) << (32 - (r))))

/**
 * trb_rotl64:
 * @x: The 64-bit integer to be rotated.
 * @r: The rotation count.
 *
 * Rotates @x by @r bits to the left.
 *
 * Returns: A rotated 64-bit integer.
 **/
#define trb_rotl64(x, r) (((x) << (r)) | ((x) >> (64 - (r))))

/**
 * trb_rotr64:
 * @x: The 64-bit integer to be rotated.
 * @r: The rotation count.
 *
 * Rotates @x by @r bits to the right.
 *
 * Returns: A rotated 64-bit integer.
 **/
#define trb_rotr64(x, r) (((x) >> (r)) | ((x) << (64 - (r))))

#if USIZE_WIDTH == 64
	#define trb_rotr(num) trb_rotr64(num)
	#define trb_rotl(num) trb_rotl64(num)
#else
	#define trb_rotr(num) trb_rotr32(num)
	#define trb_rotl(num) trb_rotl32(num)
#endif

/**
 * trb_max:
 * @a: The first value.
 * @b: The second value.
 *
 * Chooses the largest value between @a and @b.
 *
 * Returns: the largest value between @a and @b.
 **/
#define trb_max(a, b) (((a) > (b)) ? (a) : (b))

/**
 * trb_min:
 * @a: The first value.
 * @b: The second value.
 *
 * Chooses the lowest value between @a and @b.
 *
 * Returns: the lowest value between @a and @b.
 **/
#define trb_min(a, b) (((a) < (b)) ? (a) : (b))

#endif /* end of include guard: MATH_H_5PNMJXKW */
