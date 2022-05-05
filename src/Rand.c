#include "Rand.h"

#include "Messages.h"

SplitMix64 *sm64_init(SplitMix64 *state, u64 seed)
{
	if (state == NULL) {
		state = talloc(SplitMix64, 1);

		if (state == NULL) {
			msg_error("couldn't allocate memory for the SplitMix64 state");
			return NULL;
		}
	}

	state->s = seed;

	return state;
}

u32 sm64_next_u32(SplitMix64 *state)
{
	return_val_if_fail(state != NULL, 0);

	u64 result = (state->s += 0x9e3779b97f4a7c15);
	result = (result ^ (result >> 33)) * 0x62a9d9ed799705f5;
	result = (result ^ (result >> 28)) * 0xcb24d0a5c88c35b3;

	return (u32) (result >> 32);
}

u64 sm64_next_u64(SplitMix64 *state)
{
	return_val_if_fail(state != NULL, 0);

	u64 result = (state->s += 0x9e3779b97f4a7c15);
	result = (result ^ (result >> 30)) * 0xbf58476d1ce4e5b9;
	result = (result ^ (result >> 27)) * 0x94d049bb133111eb;

	return result ^ (result >> 31);
}

Xoshiro128StarStar *xs128ss_init(Xoshiro128StarStar *state, u64 seed)
{
	if (state == NULL) {
		state = talloc(Xoshiro128StarStar, 1);

		if (state == NULL) {
			msg_error("couldn't allocate memory for the Xoshiro128StarStar state");
			return NULL;
		}
	}

	SplitMix64 sm;
	sm64_init(&sm, seed);

	state->s[0] = sm64_next_u32(&sm);
	state->s[1] = sm64_next_u32(&sm);
	state->s[2] = sm64_next_u32(&sm);
	state->s[3] = sm64_next_u32(&sm);

	return state;
}

u32 xs128ss_next(Xoshiro128StarStar *state)
{
	return_val_if_fail(state != NULL, 0);

	u32 result = ROTL32(state->s[1] * 5, 7) * 9;
	u32 t = state->s[1] << 9;

	state->s[2] ^= state->s[0];
	state->s[3] ^= state->s[1];
	state->s[1] ^= state->s[2];
	state->s[0] ^= state->s[3];

	state->s[2] ^= t;

	state->s[3] = ROTL32(state->s[3], 11);

	return result;
}

Xoshiro128Plus *xs128p_init(Xoshiro128Plus *state, u64 seed)
{
	if (state == NULL) {
		state = talloc(Xoshiro128Plus, 1);

		if (state == NULL) {
			msg_error("couldn't allocate memory for the Xoshiro128Plus state");
			return NULL;
		}
	}

	SplitMix64 sm;
	sm64_init(&sm, seed);

	state->s[0] = sm64_next_u32(&sm);
	state->s[1] = sm64_next_u32(&sm);
	state->s[2] = sm64_next_u32(&sm);
	state->s[3] = sm64_next_u32(&sm);

	return state;
}

u32 xs128p_next_u32(Xoshiro128Plus *state)
{
	return_val_if_fail(state != NULL, 0);

	u32 result = state->s[0] + state->s[3];
	u32 t = state->s[1] << 9;

	state->s[2] ^= state->s[0];
	state->s[3] ^= state->s[1];
	state->s[1] ^= state->s[2];
	state->s[0] ^= state->s[3];

	state->s[2] ^= t;

	state->s[3] = ROTL32(state->s[3], 11);

	return result;
}

f32 xs128p_next_f32(Xoshiro128Plus *state)
{
	return_val_if_fail(state != NULL, 0);

	u32 result = xs128p_next_u32(state);
	result &= 0xffffff00;
	f32 fr = (f32) result;

	return fr / (f32) U32_MAX;
}

Xoshiro256StarStar *xs256ss_init(Xoshiro256StarStar *state, u64 seed)
{
	if (state == NULL) {
		state = talloc(Xoshiro256StarStar, 1);

		if (state == NULL) {
			msg_error("couldn't allocate memory for the Xoshiro256StarStar state");
			return NULL;
		}
	}

	SplitMix64 sm;
	sm64_init(&sm, seed);

	state->s[0] = sm64_next_u64(&sm);
	state->s[1] = sm64_next_u64(&sm);
	state->s[2] = sm64_next_u64(&sm);
	state->s[3] = sm64_next_u64(&sm);

	return state;
}

u64 xs256ss_next(Xoshiro256StarStar *state)
{
	return_val_if_fail(state != NULL, 0);

	u64 result = ROTL64(state->s[1] * 5, 7) * 9;
	u64 t = state->s[1] << 17;

	state->s[2] ^= state->s[0];
	state->s[3] ^= state->s[1];
	state->s[1] ^= state->s[2];
	state->s[0] ^= state->s[3];

	state->s[2] ^= t;

	state->s[3] = ROTL64(state->s[3], 45);

	return result;
}

Xoshiro256Plus *xs256p_init(Xoshiro256Plus *state, u64 seed)
{
	if (state == NULL) {
		state = talloc(Xoshiro256Plus, 1);

		if (state == NULL) {
			msg_error("couldn't allocate memory for the Xoshiro256Plus state");
			return NULL;
		}
	}

	SplitMix64 sm;
	sm64_init(&sm, seed);

	state->s[0] = sm64_next_u64(&sm);
	state->s[1] = sm64_next_u64(&sm);
	state->s[2] = sm64_next_u64(&sm);
	state->s[3] = sm64_next_u64(&sm);

	return state;
}

u64 xs256p_next_u64(Xoshiro256Plus *state)
{
	return_val_if_fail(state != NULL, 0);

	u64 result = state->s[0] + state->s[3];
	u64 t = state->s[1] << 17;

	state->s[2] ^= state->s[0];
	state->s[3] ^= state->s[1];
	state->s[1] ^= state->s[2];
	state->s[0] ^= state->s[3];

	state->s[2] ^= t;

	state->s[3] = ROTL64(state->s[3], 45);

	return result;
}

f64 xs256p_next_f64(Xoshiro256Plus *state)
{
	return_val_if_fail(state != NULL, 0);

	u64 result = xs256p_next_u64(state);
	result &= U64_C(0xfffffffffffff800);
	f64 fr = (f64) result;

	return fr / (f64) U64_MAX;
}
