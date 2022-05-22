#include "Math.h"

#include <memory.h>
#include <stdlib.h>

u128 u128_add(u128 a, u128 b)
{
#ifdef HAS_INT128
	return a + b;
#else
	u64 hi = a.hi + b.hi;
	u64 lo = a.lo + b.lo;

	u128 result = (u128){ hi, lo };

	if (lo < a.lo) {
		result.hi += 1;
	}

	return result;
#endif
}

u128 u128_sub(u128 a, u128 b)
{
#ifdef HAS_INT128
	return a - b;
#else
	u64 lo = a.lo - b.lo;
	u64 hi = a.hi - b.hi;

	u128 result = (u128){ hi, lo };

	if (a.lo < b.lo) {
		result.hi -= 1;
	}

	return result;
#endif
}

u128 u128_shr(u128 num, usize shift)
{
#ifdef HAS_INT128
	return num >> shift;
#else
	if (shift == 0)
		return num;

	u128 result;

	if (shift >= 64) {
		result = (u128){ 0, num.hi >> (shift - 64) };
	} else {
		result = (u128){ num.hi >> shift, (num.lo >> shift) | (num.hi << (64 - shift)) };
	}

	return result;
#endif
}

u128 u128_shl(u128 num, usize shift)
{
#ifdef HAS_INT128
	return num << shift;
#else
	if (shift == 0)
		return num;

	u128 result;

	if (shift >= 64) {
		result = (u128){ num.lo << (shift - 64), 0 };
	} else {
		result = (u128){ (num.hi << shift) | (num.lo >> (64 - shift)), num.lo << shift };
	}

	return result;
#endif
}

u128 u128_mul(u128 a, u128 b)
{
#ifdef HAS_INT128
	return a * b;
#else
	u64 a32 = a.lo >> 32;
	u64 a00 = a.lo & 0xffffffff;
	u64 b32 = b.lo >> 32;
	u64 b00 = b.lo & 0xffffffff;

	u128 result = (u128){
		a.hi * b.lo + a.lo * b.hi + a32 * b32,
		a00 * b00
	};

	u128 a32b00 = (u128){ 0, a32 * b00 };
	a32b00 = u128_shl(a32b00, 32);

	u128 a00b32 = (u128){ 0, a00 * b32 };
	a00b32 = u128_shl(a00b32, 32);

	result = u128_add(result, a32b00);
	result = u128_add(result, a00b32);

	return result;
#endif
}

u128 u128_xor(u128 a, u128 b)
{
#ifdef HAS_INT128
	return a ^ b;
#else
	return (u128){ a.hi ^ b.hi, a.lo ^ b.lo };
#endif
}

u128 u128_and(u128 a, u128 b)
{
#ifdef HAS_INT128
	return a & b;
#else
	return (u128){ a.hi & b.hi, a.lo & b.lo };
#endif
}

u128 u128_or(u128 a, u128 b)
{
#ifdef HAS_INT128
	return a | b;
#else
	return (u128){ a.hi | b.hi, a.lo | b.lo };
#endif
}

i32 u128_cmp(const u128 *a, const u128 *b)
{
#ifdef HAS_INT128
	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
#else
	if (a->hi > b->hi)
		return 1;

	if (a->hi < b->hi)
		return -1;

	if (a->lo > b->lo)
		return 1;

	if (a->lo < b->lo)
		return -1;

	return 0;
#endif
}

i32 u128_clz(u128 num)
{
#ifdef HAS_INT128
	u64 hi = (u64) (num >> 64);
	u64 lo = num & U64_MAX;
	return hi ? trb_clz64(hi) : trb_clz64(hi) + trb_clz64(lo);
#else
	return num.hi ? trb_clz64(num.hi) : trb_clz64(num.hi) + trb_clz64(num.lo);
#endif
}

ud128 u128_div(u128 num, u128 denom)
{
#ifdef HAS_INT128
	return (ud128){
		.quot = num / denom,
		.rem = num % denom,
	};
#else
	if (u128_cmp(&denom, &num) > 0) {
		return (ud128){
			.quot = (u128){0, 0},
			.rem = num,
		};
	}

	if (u128_cmp(&num, &denom) == 0) {
		return (ud128){
			.quot = (u128){0, 1},
			.rem = (u128){0, 0},
		};
	}

	u128 quot;

	isize shift = trb_abs(u128_clz(num) - u128_clz(denom));
	denom = u128_shl(denom, shift);

	quot = (u128){ 0, 0 };

	for (; shift >= 0; --shift) {
		quot = u128_shl(quot, 1);

		if (u128_cmp(&num, &denom) >= 0) {
			num = u128_sub(num, denom);
			quot = u128_or(quot, (u128){ 0, 1 });
		}

		denom = u128_shr(denom, 1);
	}

	return (ud128){
		.quot = quot,
		.rem = num,
	};
#endif
}

i128 i128_add(i128 a, i128 b)
{
#ifdef HAS_INT128
	return a + b;
#else
	u64 lo = a.lo + b.lo;
	u64 hi = a.hi + b.hi;

	i128 result = (i128){ hi, lo };

	if (lo < a.lo) {
		result.hi += 1;
	}

	return result;
#endif
}

i128 i128_sub(i128 a, i128 b)
{
#ifdef HAS_INT128
	return a - b;
#else
	u64 lo = a.lo - b.lo;
	u64 hi = a.hi - b.hi;

	i128 result = (i128){ hi, lo };

	if (a.lo < b.lo) {
		result.hi -= 1;
	}

	return result;
#endif
}

i128 i128_mul(i128 a, i128 b)
{
#ifdef HAS_INT128
	return a * b;
#else
	u128 ua = { a.hi, a.lo };
	u128 ub = { b.hi, b.lo };

	u128 umul = u128_mul(ua, ub);

	return (i128){ umul.hi, umul.lo };
#endif
}

i128 i128_shr(i128 num, usize shift)
{
#ifdef HAS_INT128
	return num >> shift;
#else
	if (shift == 0)
		return num;

	i128 result;

	if (shift >= 64) {
		result = (i128){ (num.hi >> 32) >> 32, (u64) num.hi >> (shift - 64) };
	} else {
		result = (i128){ num.hi >> shift, (num.lo >> shift) | ((u64) num.hi << (64 - shift)) };
	}

	return result;
#endif
}

i128 i128_shl(i128 num, usize shift)
{
#ifdef HAS_INT128
	return num << shift;
#else
	if (shift == 0)
		return num;

	i128 result;

	if (shift >= 64) {
		result = (i128){ (i64) num.lo << (shift - 64), 0 };
	} else {
		result = (i128){ (num.hi << shift) | ((i64) num.lo >> (64 - shift)), num.lo << shift };
	}

	return result;
#endif
}

i128 i128_xor(i128 a, i128 b)
{
#ifdef HAS_INT128
	return a ^ b;
#else
	return (i128){ a.hi ^ b.hi, a.lo ^ b.lo };
#endif
}

i128 i128_and(i128 a, i128 b)
{
#ifdef HAS_INT128
	return a & b;
#else
	return (i128){ a.hi & b.hi, a.lo & b.lo };
#endif
}

i128 i128_or(i128 a, i128 b)
{
#ifdef HAS_INT128
	return a | b;
#else
	return (i128){ a.hi | b.hi, a.lo | b.lo };
#endif
}

i32 i128_cmp(const i128 *a, const i128 *b)
{
#ifdef HAS_INT128
	if (*a > *b)
		return 1;

	if (*a < *b)
		return -1;

	return 0;
#else
	if (a->hi > b->hi)
		return 1;

	if (a->hi < b->hi)
		return -1;

	if (a->lo > b->lo)
		return 1;

	if (a->lo < b->lo)
		return -1;

	return 0;
#endif
}

i128 i128_minus(i128 num)
{
#ifdef HAS_INT128
	return -num;
#else
	return (i128){
		~num.hi + (num.lo == 0),
		~num.lo + 1
	};
#endif
}

i128 i128_not(i128 num)
{
#ifdef HAS_INT128
	return ~num;
#else
	return (i128){ ~num.hi, ~num.lo };
#endif
}

d128 i128_div(i128 num, i128 denom)
{
#ifdef HAS_INT128
	return (d128){
		.quot = num / denom,
		.rem = num % denom,
	};
#else
	if (i128_cmp(&num, &I128_MIN) == 0) {
		return (d128){
			.quot = (i128){0, 0},
			.rem = num,
		};
	}

	if (i128_cmp(&num, &I128_C(-1, -1)) == 0) {
		return (d128){
			.quot = (i128){0, 0},
			.rem = num,
		};
	}

	u128 unum = { trb_abs64(num.hi), trb_abs64(num.lo) };
	u128 udenom = { trb_abs64(denom.hi), trb_abs64(denom.lo) };

	ud128 div = u128_div(unum, udenom);
	d128 result = {
		.quot = {div.quot.hi, div.quot.lo},
		.rem = { div.rem.hi, div.quot.lo}
	};

	if ((num.hi < 0) != (denom.hi < 0))
		result.quot = i128_minus(result.quot);

	return result;
#endif
}

u32 trb_abs32(i32 num)
{
	return (num < 0) ? -num : num;
}

u64 trb_abs64(i64 num)
{
	return (num < 0) ? -num : num;
}

usize trb_abs(isize num)
{
	return (num < 0) ? -num : num;
}

u32 trb_pow2_32(u32 value)
{
	if (value == 0 || value == 1)
		return 1;

	return (1 << (32 - trb_clz32(value - 1)));
}

u64 trb_pow2_64(u64 value)
{
	if (value == 0 || value == 1)
		return 1;

	return (1 << (64 - trb_clz64(value - 1)));
}

usize trb_pow2(usize value)
{
	if (value == 0 || value == 1)
		return 1;

	return (1 << (USIZE_WIDTH - trb_clz(value - 1)));
}

u32 trb_mul_pow2_32(u32 num, u32 factor)
{
	if (factor == 0)
		return 0;

	return num << (31 - trb_clz(factor));
}

u64 trb_mul_pow2_64(u64 num, u64 factor)
{
	if (factor == 0)
		return 0;

	return num << (63 - trb_clz(factor));
}

usize trb_mul_pow2(usize num, usize factor)
{
	if (factor == 0)
		return 0;

	return num << (USIZE_WIDTH - trb_clz(factor) - 1);
}

ud32 trb_div_pow2_32(u32 num, u32 denom)
{
	if (denom == 0) {
		return (ud32){
			.quot = 0,
			.rem = 0,
		};
	}

	return (ud32){
		.quot = num >> (31 - trb_clz32(denom)),
		.rem = num & (denom - 1),
	};
}

ud64 trb_div_pow2_64(u64 num, u64 denom)
{
	if (denom == 0) {
		return (ud64){
			.quot = 0,
			.rem = 0,
		};
	}

	return (ud64){
		.quot = num >> (63 - trb_clz64(denom)),
		.rem = num & (denom - 1),
	};
}

udsize trb_div_pow2(usize num, usize denom)
{
	if (denom == 0) {
		return (udsize){
			.quot = 0,
			.rem = 0,
		};
	}

	return (udsize){
		.rem = num >> (USIZE_WIDTH - trb_clz(denom) - 1),
		.quot = num & (denom - 1),
	};
}
