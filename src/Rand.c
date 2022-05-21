#include "Rand.h"

#include "Messages.h"

TrbSM64 *sm64_init(TrbSM64 *state, u64 seed)
{
	if (state == NULL) {
		state = trb_talloc(TrbSM64, 1);

		if (state == NULL) {
			trb_msg_error("couldn't allocate memory for the TrbSM64 state");
			return NULL;
		}
	}

	state->s = seed;

	return state;
}

u32 sm64_next_u32(TrbSM64 *state)
{
	trb_return_val_if_fail(state != NULL, 0);

	u64 result = (state->s += 0x9e3779b97f4a7c15);
	result = (result ^ (result >> 33)) * 0x62a9d9ed799705f5;
	result = (result ^ (result >> 28)) * 0xcb24d0a5c88c35b3;

	return (u32) (result >> 32);
}

u64 sm64_next_u64(TrbSM64 *state)
{
	trb_return_val_if_fail(state != NULL, 0);

	u64 result = (state->s += 0x9e3779b97f4a7c15);
	result = (result ^ (result >> 30)) * 0xbf58476d1ce4e5b9;
	result = (result ^ (result >> 27)) * 0x94d049bb133111eb;

	return result ^ (result >> 31);
}

TrbXs128ss *xs128ss_init(TrbXs128ss *state, u64 seed)
{
	if (state == NULL) {
		state = trb_talloc(TrbXs128ss, 1);

		if (state == NULL) {
			trb_msg_error("couldn't allocate memory for the TrbXs128ss state");
			return NULL;
		}
	}

	TrbSM64 sm;
	sm64_init(&sm, seed);

	state->s[0] = sm64_next_u32(&sm);
	state->s[1] = sm64_next_u32(&sm);
	state->s[2] = sm64_next_u32(&sm);
	state->s[3] = sm64_next_u32(&sm);

	return state;
}

u32 xs128ss_next(TrbXs128ss *state)
{
	trb_return_val_if_fail(state != NULL, 0);

	u32 result = trb_rotl32(state->s[1] * 5, 7) * 9;
	u32 t = state->s[1] << 9;

	state->s[2] ^= state->s[0];
	state->s[3] ^= state->s[1];
	state->s[1] ^= state->s[2];
	state->s[0] ^= state->s[3];

	state->s[2] ^= t;

	state->s[3] = trb_rotl32(state->s[3], 11);

	return result;
}

TrbXs128p *xs128p_init(TrbXs128p *state, u64 seed)
{
	if (state == NULL) {
		state = trb_talloc(TrbXs128p, 1);

		if (state == NULL) {
			trb_msg_error("couldn't allocate memory for the TrbXs128p state");
			return NULL;
		}
	}

	TrbSM64 sm;
	sm64_init(&sm, seed);

	state->s[0] = sm64_next_u32(&sm);
	state->s[1] = sm64_next_u32(&sm);
	state->s[2] = sm64_next_u32(&sm);
	state->s[3] = sm64_next_u32(&sm);

	return state;
}

u32 xs128p_next_u32(TrbXs128p *state)
{
	trb_return_val_if_fail(state != NULL, 0);

	u32 result = state->s[0] + state->s[3];
	u32 t = state->s[1] << 9;

	state->s[2] ^= state->s[0];
	state->s[3] ^= state->s[1];
	state->s[1] ^= state->s[2];
	state->s[0] ^= state->s[3];

	state->s[2] ^= t;

	state->s[3] = trb_rotl32(state->s[3], 11);

	return result;
}

f32 xs128p_next_f32(TrbXs128p *state)
{
	trb_return_val_if_fail(state != NULL, 0);

	u32 result = xs128p_next_u32(state);
	result &= 0xffffff00;
	f32 fr = (f32) result;

	return fr / (f32) U32_MAX;
}

TrbXs256ss *xs256ss_init(TrbXs256ss *state, u64 seed)
{
	if (state == NULL) {
		state = trb_talloc(TrbXs256ss, 1);

		if (state == NULL) {
			trb_msg_error("couldn't allocate memory for the TrbXs256ss state");
			return NULL;
		}
	}

	TrbSM64 sm;
	sm64_init(&sm, seed);

	state->s[0] = sm64_next_u64(&sm);
	state->s[1] = sm64_next_u64(&sm);
	state->s[2] = sm64_next_u64(&sm);
	state->s[3] = sm64_next_u64(&sm);

	return state;
}

u64 xs256ss_next(TrbXs256ss *state)
{
	trb_return_val_if_fail(state != NULL, 0);

	u64 result = trb_rotl64(state->s[1] * 5, 7) * 9;
	u64 t = state->s[1] << 17;

	state->s[2] ^= state->s[0];
	state->s[3] ^= state->s[1];
	state->s[1] ^= state->s[2];
	state->s[0] ^= state->s[3];

	state->s[2] ^= t;

	state->s[3] = trb_rotl64(state->s[3], 45);

	return result;
}

TrbXs256p *xs256p_init(TrbXs256p *state, u64 seed)
{
	if (state == NULL) {
		state = trb_talloc(TrbXs256p, 1);

		if (state == NULL) {
			trb_msg_error("couldn't allocate memory for the TrbXs256p state");
			return NULL;
		}
	}

	TrbSM64 sm;
	sm64_init(&sm, seed);

	state->s[0] = sm64_next_u64(&sm);
	state->s[1] = sm64_next_u64(&sm);
	state->s[2] = sm64_next_u64(&sm);
	state->s[3] = sm64_next_u64(&sm);

	return state;
}

u64 xs256p_next_u64(TrbXs256p *state)
{
	trb_return_val_if_fail(state != NULL, 0);

	u64 result = state->s[0] + state->s[3];
	u64 t = state->s[1] << 17;

	state->s[2] ^= state->s[0];
	state->s[3] ^= state->s[1];
	state->s[1] ^= state->s[2];
	state->s[0] ^= state->s[3];

	state->s[2] ^= t;

	state->s[3] = trb_rotl64(state->s[3], 45);

	return result;
}

f64 xs256p_next_f64(TrbXs256p *state)
{
	trb_return_val_if_fail(state != NULL, 0);

	u64 result = xs256p_next_u64(state);
	result &= U64_C(0xfffffffffffff800);
	f64 fr = (f64) result;

	return fr / (f64) U64_MAX;
}
