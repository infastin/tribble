#include "trb-rand.h"

#include "trb-math.h"
#include "trb-messages.h"

#include <math.h>

TrbSM64 *trb_sm64_init(TrbSM64 *self, u64 seed)
{
	if (self == NULL) {
		self = trb_talloc(TrbSM64, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the TrbSM64 self");
			return NULL;
		}
	}

	self->s = seed;

	return self;
}

u32 trb_sm64_next_u32(TrbSM64 *self)
{
	trb_return_val_if_fail(self != NULL, 0);

	u64 result = (self->s += 0x9e3779b97f4a7c15);
	result = (result ^ (result >> 33)) * 0x62a9d9ed799705f5;
	result = (result ^ (result >> 28)) * 0xcb24d0a5c88c35b3;

	return (u32) (result >> 32);
}

u64 trb_sm64_next_u64(TrbSM64 *self)
{
	trb_return_val_if_fail(self != NULL, 0);

	u64 result = (self->s += 0x9e3779b97f4a7c15);
	result = (result ^ (result >> 30)) * 0xbf58476d1ce4e5b9;
	result = (result ^ (result >> 27)) * 0x94d049bb133111eb;

	return result ^ (result >> 31);
}

TrbXs128ss *trb_xs128ss_init(TrbXs128ss *self, u64 seed)
{
	if (self == NULL) {
		self = trb_talloc(TrbXs128ss, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the TrbXs128ss self");
			return NULL;
		}
	}

	TrbSM64 sm;
	trb_sm64_init(&sm, seed);

	self->s[0] = trb_sm64_next_u32(&sm);
	self->s[1] = trb_sm64_next_u32(&sm);
	self->s[2] = trb_sm64_next_u32(&sm);
	self->s[3] = trb_sm64_next_u32(&sm);

	return self;
}

u32 trb_xs128ss_next(TrbXs128ss *self)
{
	trb_return_val_if_fail(self != NULL, 0);

	u32 result = trb_rotl32(self->s[1] * 5, 7) * 9;
	u32 t = self->s[1] << 9;

	self->s[2] ^= self->s[0];
	self->s[3] ^= self->s[1];
	self->s[1] ^= self->s[2];
	self->s[0] ^= self->s[3];

	self->s[2] ^= t;

	self->s[3] = trb_rotl32(self->s[3], 11);

	return result;
}

TrbXs128p *trb_xs128p_init(TrbXs128p *self, u64 seed)
{
	if (self == NULL) {
		self = trb_talloc(TrbXs128p, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the TrbXs128p self");
			return NULL;
		}
	}

	TrbSM64 sm;
	trb_sm64_init(&sm, seed);

	self->s[0] = trb_sm64_next_u32(&sm);
	self->s[1] = trb_sm64_next_u32(&sm);
	self->s[2] = trb_sm64_next_u32(&sm);
	self->s[3] = trb_sm64_next_u32(&sm);

	return self;
}

u32 trb_xs128p_next_u32(TrbXs128p *self)
{
	trb_return_val_if_fail(self != NULL, 0);

	u32 result = self->s[0] + self->s[3];
	u32 t = self->s[1] << 9;

	self->s[2] ^= self->s[0];
	self->s[3] ^= self->s[1];
	self->s[1] ^= self->s[2];
	self->s[0] ^= self->s[3];

	self->s[2] ^= t;

	self->s[3] = trb_rotl32(self->s[3], 11);

	return result;
}

f32 trb_xs128p_next_f32(TrbXs128p *self)
{
	trb_return_val_if_fail(self != NULL, 0);

	u32 result = trb_xs128p_next_u32(self);
	result &= 0xffffff00;
	f32 fr = (f32) result;

	return ldexpf(fr, -32);
}

TrbXs256ss *trb_xs256ss_init(TrbXs256ss *self, u64 seed)
{
	if (self == NULL) {
		self = trb_talloc(TrbXs256ss, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the TrbXs256ss self");
			return NULL;
		}
	}

	TrbSM64 sm;
	trb_sm64_init(&sm, seed);

	self->s[0] = trb_sm64_next_u64(&sm);
	self->s[1] = trb_sm64_next_u64(&sm);
	self->s[2] = trb_sm64_next_u64(&sm);
	self->s[3] = trb_sm64_next_u64(&sm);

	return self;
}

u64 trb_xs256ss_next(TrbXs256ss *self)
{
	trb_return_val_if_fail(self != NULL, 0);

	u64 result = trb_rotl64(self->s[1] * 5, 7) * 9;
	u64 t = self->s[1] << 17;

	self->s[2] ^= self->s[0];
	self->s[3] ^= self->s[1];
	self->s[1] ^= self->s[2];
	self->s[0] ^= self->s[3];

	self->s[2] ^= t;

	self->s[3] = trb_rotl64(self->s[3], 45);

	return result;
}

TrbXs256p *trb_xs256p_init(TrbXs256p *self, u64 seed)
{
	if (self == NULL) {
		self = trb_talloc(TrbXs256p, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the TrbXs256p state");
			return NULL;
		}
	}

	TrbSM64 sm;
	trb_sm64_init(&sm, seed);

	self->s[0] = trb_sm64_next_u64(&sm);
	self->s[1] = trb_sm64_next_u64(&sm);
	self->s[2] = trb_sm64_next_u64(&sm);
	self->s[3] = trb_sm64_next_u64(&sm);

	return self;
}

u64 trb_xs256p_next_u64(TrbXs256p *self)
{
	trb_return_val_if_fail(self != NULL, 0);

	u64 result = self->s[0] + self->s[3];
	u64 t = self->s[1] << 17;

	self->s[2] ^= self->s[0];
	self->s[3] ^= self->s[1];
	self->s[1] ^= self->s[2];
	self->s[0] ^= self->s[3];

	self->s[2] ^= t;

	self->s[3] = trb_rotl64(self->s[3], 45);

	return result;
}

f64 trb_xs256p_next_f64(TrbXs256p *state)
{
	trb_return_val_if_fail(state != NULL, 0);

	u64 result = trb_xs256p_next_u64(state);
	result &= U64_C(0xfffffffffffff800);
	f64 fr = (f64) result;

	return ldexp(fr, -64);
}

TrbPcg64 *trb_pcg64_init(TrbPcg64 *self, u64 seed)
{
	if (self == NULL) {
		self = trb_talloc(TrbPcg64, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the Pcg64 state");
			return NULL;
		}
	}

	self->s = 0;
	self->s = self->s * U64_C(6364136223846793005) + U64_C(1442695040888963407);
	self->s += seed;
	self->s = self->s * U64_C(6364136223846793005) + U64_C(1442695040888963407);

	return self;
}

u32 trb_pcg64_next_u32(TrbPcg64 *self)
{
	u64 oldstate = self->s;
	self->s = self->s * U64_C(6364136223846793005) + U64_C(1442695040888963407);
	return (u32) (((oldstate >> 22) ^ oldstate) >> ((oldstate >> 61) + 22));
}

f32 trb_pcg64_next_f32(TrbPcg64 *self)
{
	u32 r = trb_pcg64_next_u32(self);
	f32 fr = (float) r;
	return ldexpf(fr, -32);
}

u64 trb_pcg64_next_u64(TrbPcg64 *self)
{
	u64 oldstate = self->s;
	self->s = self->s * U64_C(6364136223846793005) + U64_C(1442695040888963407);

	u64 word = ((oldstate >> ((oldstate >> 59u) + 5u)) ^ oldstate) * U64_C(12605985483714917081);
	return (word >> 43u) ^ word;
}

f64 trb_pcg64_next_f64(TrbPcg64 *self)
{
	u64 r = trb_pcg64_next_u64(self);
	f64 fr = (double) r;
	return ldexp(fr, -64);
}

typedef unsigned _BitInt(128) u128;

#define U128_C(hi, lo) (((u128) U64_C(hi) << 64) | ((u128) U64_C(lo)))
#define U128_TO(hi, lo) (((u128) (hi) << 64) | ((u128) (lo)))

TrbPcg128 *trb_pcg128_init(TrbPcg128 *self, u64 seed)
{
	if (self == NULL) {
		self = trb_talloc(TrbPcg128, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the Pcg128 state");
			return NULL;
		}
	}

	const u128 multi = U128_C(2549297995355413924, 4865540595714422341);
	const u128 inc = U128_C(6364136223846793005, 1442695040888963407);

	u128 state = (inc + seed) * multi + inc;
	self->s[0] = (u64) (state & U64_MAX);
	self->s[1] = (u64) (state >> 64);

	return self;
}

u64 trb_pcg128_next_u64(TrbPcg128 *self)
{
	const u128 multi = U128_C(2549297995355413924, 4865540595714422341);
	const u128 inc = U128_C(6364136223846793005, 1442695040888963407);

	u128 state = U128_TO(self->s[1], self->s[0]);

	state = (state * multi) + inc;
	self->s[0] = (u64) (state & U64_MAX);
	self->s[1] = (u64) (state >> 64);

	return (u64) (((state >> 43) ^ state) >> ((state >> 124) + 45));
}

f64 trb_pcg128_next_f64(TrbPcg128 *self)
{
	u64 r = trb_pcg128_next_u64(self);
	f64 fr = (double) r;
	return ldexp(fr, -64);
}
