#ifndef CHECKED_H_NOFXS3P6
#define CHECKED_H_NOFXS3P6

#include "Types.h"

/**
 * trb_chk_add_i8:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Adds up @a and @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_add_i8(i8 a, i8 b, i8 *res);

/**
 * trb_chk_add_i16:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Adds up @a and @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_add_i16(i16 a, i16 b, i16 *res);

/**
 * trb_chk_add_i32:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Adds up @a and @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_add_i32(i32 a, i32 b, i32 *res);

/**
 * trb_chk_add_i64:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Adds up @a and @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_add_i64(i64 a, i64 b, i64 *res);

/**
 * trb_chk_add_isize:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Adds up @a and @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_add_isize(isize a, isize b, isize *res);

/**
 * trb_chk_add_u8:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Adds up @a and @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_add_u8(u8 a, u8 b, u8 *res);

/**
 * trb_chk_add_u16:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Adds up @a and @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_add_u16(u16 a, u16 b, u16 *res);

/**
 * trb_chk_add_u32:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Adds up @a and @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_add_u32(u32 a, u32 b, u32 *res);

/**
 * trb_chk_add_u64:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Adds up @a and @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_add_u64(u64 a, u64 b, u64 *res);

/**
 * trb_chk_add_usize:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Adds up @a and @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_add_usize(usize a, usize b, usize *res);

// clang-format off
/**
 * trb_chk_add:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Adds up @a and @b and puts the result to @res.
 * Returns %TRUE if the operation overflowed.
 **/
#define trb_chk_add(a, b, res) _Generic((a),\
	i8: trb_chk_add_i8, \
	i16: trb_chk_add_i16, \
	i32: trb_chk_add_i32, \
	i64: trb_chk_add_i64, \
	u8: trb_chk_add_u8, \
	u16: trb_chk_add_u16, \
	u32: trb_chk_add_u32, \
	u64: trb_chk_add_u64 \
)(a, b, res)
// clang-format on

/**
 * trb_chk_mul_i8:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Multiplies @a by @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_mul_i8(i8 a, i8 b, i8 *res);

/**
 * trb_chk_mul_i16:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Multiplies @a by @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_mul_i16(i16 a, i16 b, i16 *res);

/**
 * trb_chk_mul_i32:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Multiplies @a by @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_mul_i32(i32 a, i32 b, i32 *res);

/**
 * trb_chk_mul_i64:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Multiplies @a by @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_mul_i64(i64 a, i64 b, i64 *res);

/**
 * trb_chk_mul_isize:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Multiplies @a by @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_mul_isize(isize a, isize b, isize *res);

/**
 * trb_chk_mul_u8:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Multiplies @a by @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_mul_u8(u8 a, u8 b, u8 *res);

/**
 * trb_chk_mul_u16:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Multiplies @a by @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_mul_u16(u16 a, u16 b, u16 *res);

/**
 * trb_chk_mul_u32:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Multiplies @a by @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_mul_u32(u32 a, u32 b, u32 *res);

/**
 * trb_chk_mul_u64:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Multiplies @a by @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_mul_u64(u64 a, u64 b, u64 *res);

/**
 * trb_chk_mul_usize:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Multiplies @a by @b and puts the result to @res.
 *
 * Returns: %TRUE if the operation overflowed.
 **/
bool trb_chk_mul_usize(usize a, usize b, usize *res);

// clang-format off
/**
 * trb_chk_mul:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Multiplies @a by @b and puts the result to @res.
 * Returns %TRUE if the operation overflowed.
 **/
#define trb_chk_mul(a, b, res) _Generic((a),\
	i8: trb_chk_mul_i8, \
	i16: trb_chk_mul_i16, \
	i32: trb_chk_mul_i32, \
	i64: trb_chk_mul_i64, \
	u8: trb_chk_mul_u8, \
	u16: trb_chk_mul_u16, \
	u32: trb_chk_mul_u32, \
	u64: trb_chk_mul_u64 \
)(a, b, res)
// clang-format on

/**
 * trb_chk_sub_i8:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Subtracts @b from @a and puts the result to @res.
 *
 * Returns: %TRUE if the operation underflowed.
 **/
bool trb_chk_sub_i8(i8 a, i8 b, i8 *res);

/**
 * trb_chk_sub_i16:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Subtracts @b from @a and puts the result to @res.
 *
 * Returns: %TRUE if the operation underflowed.
 **/
bool trb_chk_sub_i16(i16 a, i16 b, i16 *res);

/**
 * trb_chk_sub_i32:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Subtracts @b from @a and puts the result to @res.
 *
 * Returns: %TRUE if the operation underflowed.
 **/
bool trb_chk_sub_i32(i32 a, i32 b, i32 *res);

/**
 * trb_chk_sub_i64:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Subtracts @b from @a and puts the result to @res.
 *
 * Returns: %TRUE if the operation underflowed.
 **/
bool trb_chk_sub_i64(i64 a, i64 b, i64 *res);

/**
 * trb_chk_sub_isize:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Subtracts @b from @a and puts the result to @res.
 *
 * Returns: %TRUE if the operation underflowed.
 **/
bool trb_chk_sub_isize(isize a, isize b, isize *res);

/**
 * trb_chk_sub_u8:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Subtracts @b from @a and puts the result to @res.
 *
 * Returns: %TRUE if the operation underflowed.
 **/
bool trb_chk_sub_u8(u8 a, u8 b, u8 *res);

/**
 * trb_chk_sub_u16:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Subtracts @b from @a and puts the result to @res.
 *
 * Returns: %TRUE if the operation underflowed.
 **/
bool trb_chk_sub_u16(u16 a, u16 b, u16 *res);

/**
 * trb_chk_sub_u32:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Subtracts @b from @a and puts the result to @res.
 *
 * Returns: %TRUE if the operation underflowed.
 **/
bool trb_chk_sub_u32(u32 a, u32 b, u32 *res);

/**
 * trb_chk_sub_u64:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Subtracts @b from @a and puts the result to @res.
 *
 * Returns: %TRUE if the operation underflowed.
 **/
bool trb_chk_sub_u64(u64 a, u64 b, u64 *res);

/**
 * trb_chk_sub_usize:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Subtracts @b from @a and puts the result to @res.
 *
 * Returns: %TRUE if the operation underflowed.
 **/
bool trb_chk_sub_usize(usize a, usize b, usize *res);

// clang-format off
/**
 * trb_chk_sub:
 * @a: The first operand.
 * @b: The second operand.
 * @res: (optional) (out): The result of the operation.
 *
 * Subtracts @b from @a and puts the result to @res.
 * Returns %TRUE if the operation underflowed.
 **/
#define trb_chk_sub(a, b, res) _Generic((a),\
	i8: trb_chk_sub_i8, \
	i16: trb_chk_sub_i16, \
	i32: trb_chk_sub_i32, \
	i64: trb_chk_sub_i64, \
	u8: trb_chk_sub_u8, \
	u16: trb_chk_sub_u16, \
	u32: trb_chk_sub_u32, \
	u64: trb_chk_sub_u64 \
)(a, b, res)
// clang-format on

#endif /* end of include guard: CHECKED_H_NOFXS3P6 */
