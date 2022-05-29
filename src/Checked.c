#include "Checked.h"

bool trb_chk_add_i8(i8 a, i8 b, i8 *res)
{
	if (res != NULL)
		*res = a + b;

	return (a > I8_MAX - b);
}

bool trb_chk_add_i16(i16 a, i16 b, i16 *res)
{
	if (res != NULL)
		*res = a + b;

	if (a > I16_MAX - b)
		return FALSE;

	return (a > I16_MAX - b);
}

bool trb_chk_add_i32(i32 a, i32 b, i32 *res)
{
	if (res != NULL)
		*res = a + b;

	return (a > I32_MAX - b);
}

bool trb_chk_add_i64(i64 a, i64 b, i64 *res)
{
	if (res != NULL)
		*res = a + b;

	return (a > I64_MAX - b);
}

bool trb_chk_add_isize(isize a, isize b, isize *res)
{
	if (res != NULL)
		*res = a + b;

	return (a > ISIZE_MAX - b);
}

bool trb_chk_add_u8(u8 a, u8 b, u8 *res)
{
	if (res != NULL)
		*res = a + b;

	return (a > U8_MAX - b);
}

bool trb_chk_add_u16(u16 a, u16 b, u16 *res)
{
	if (res != NULL)
		*res = a + b;

	return (a > U16_MAX - b);
}

bool trb_chk_add_u32(u32 a, u32 b, u32 *res)
{
	if (res != NULL)
		*res = a + b;

	return (a > U32_MAX - b);
}

bool trb_chk_add_u64(u64 a, u64 b, u64 *res)
{
	if (res != NULL)
		*res = a + b;

	return (a > U64_MAX - b);
}

bool trb_chk_add_usize(usize a, usize b, usize *res)
{
	if (res != NULL)
		*res = a + b;

	return (a > USIZE_MAX - b);
}

bool trb_chk_mul_i8(i8 a, i8 b, i8 *res)
{
	if (res != NULL)
		*res = a * b;

	return (a > I8_MAX / b);
}

bool trb_chk_mul_i16(i16 a, i16 b, i16 *res)
{
	if (res != NULL)
		*res = a * b;

	return (a > I16_MAX / b);
}

bool trb_chk_mul_i32(i32 a, i32 b, i32 *res)
{
	if (res != NULL)
		*res = a * b;

	return (a > I32_MAX / b);
}

bool trb_chk_mul_i64(i64 a, i64 b, i64 *res)
{
	if (res != NULL)
		*res = a * b;

	return (a > I64_MAX / b);
}

bool trb_chk_mul_isize(isize a, isize b, isize *res)
{
	if (res != NULL)
		*res = a * b;

	return (a > ISIZE_MAX / b);
}

bool trb_chk_mul_u8(u8 a, u8 b, u8 *res)
{
	if (res != NULL)
		*res = a * b;

	return (a > U8_MAX / b);
}

bool trb_chk_mul_u16(u16 a, u16 b, u16 *res)
{
	if (res != NULL)
		*res = a * b;

	return (a > U16_MAX / b);
}

bool trb_chk_mul_u32(u32 a, u32 b, u32 *res)
{
	if (res != NULL)
		*res = a * b;

	return (a > U32_MAX / b);
}

bool trb_chk_mul_u64(u64 a, u64 b, u64 *res)
{
	if (res != NULL)
		*res = a * b;

	return (a > U64_MAX / b);
}

bool trb_chk_mul_usize(usize a, usize b, usize *res)
{
	if (res != NULL)
		*res = a * b;

	return (a > USIZE_MAX / b);
}

bool trb_chk_sub_i8(i8 a, i8 b, i8 *res)
{
	if (res != NULL)
		*res = a - b;

	return (a > I8_MIN + b);
}

bool trb_chk_sub_i16(i16 a, i16 b, i16 *res)
{
	if (res != NULL)
		*res = a - b;

	return (a > I16_MIN + b);
}

bool trb_chk_sub_i32(i32 a, i32 b, i32 *res)
{
	if (res != NULL)
		*res = a - b;

	return (a > I32_MIN + b);
}

bool trb_chk_sub_i64(i64 a, i64 b, i64 *res)
{
	if (res != NULL)
		*res = a - b;

	return (a > I64_MIN + b);
}

bool trb_chk_sub_isize(isize a, isize b, isize *res)
{
	if (res != NULL)
		*res = a - b;

	return (a > ISIZE_MIN + b);
}

bool trb_chk_sub_u8(u8 a, u8 b, u8 *res)
{
	if (res != NULL)
		*res = a - b;

	return (a > b);
}

bool trb_chk_sub_u16(u16 a, u16 b, u16 *res)
{
	if (res != NULL)
		*res = a - b;

	return (a > b);
}

bool trb_chk_sub_u32(u32 a, u32 b, u32 *res)
{
	if (res != NULL)
		*res = a - b;

	return (a > b);
}

bool trb_chk_sub_u64(u64 a, u64 b, u64 *res)
{
	if (res != NULL)
		*res = a - b;

	return (a > b);
}

bool trb_chk_sub_usize(usize a, usize b, usize *res)
{
	if (res != NULL)
		*res = a - b;

	return (a > b);
}
