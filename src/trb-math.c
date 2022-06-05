#include "trb-math.h"

#include <stdlib.h>

u8 trb_abs_8(i8 num)
{
	return (num < 0) ? -num : num;
}

u16 trb_abs_16(i16 num)
{
	return (num < 0) ? -num : num;
}

u32 trb_abs_32(i32 num)
{
	return (num < 0) ? -num : num;
}

u64 trb_abs_64(i64 num)
{
	return (num < 0) ? -num : num;
}

usize trb_abs_size(isize num)
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

usize trb_pow2_size(usize value)
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

usize trb_mul_pow2_size(usize num, usize factor)
{
	if (factor == 0)
		return 0;

	return num << (USIZE_WIDTH - trb_clz(factor) - 1);
}

udiv32 trb_div_pow2_32(u32 num, u32 denom)
{
	if (denom == 0) {
		return (udiv32){
			.quot = 0,
			.rem = 0,
		};
	}

	return (udiv32){
		.quot = num >> (31 - trb_clz32(denom)),
		.rem = num & (denom - 1),
	};
}

udiv64 trb_div_pow2_64(u64 num, u64 denom)
{
	if (denom == 0) {
		return (udiv64){
			.quot = 0,
			.rem = 0,
		};
	}

	return (udiv64){
		.quot = num >> (63 - trb_clz64(denom)),
		.rem = num & (denom - 1),
	};
}

udivsize trb_div_pow2_size(usize num, usize denom)
{
	if (denom == 0) {
		return (udivsize){
			.quot = 0,
			.rem = 0,
		};
	}

	return (udivsize){
		.rem = num >> (USIZE_WIDTH - trb_clz(denom) - 1),
		.quot = num & (denom - 1),
	};
}
