#include "Rand.h"
#include "Messages.h"

SplitMix64 *splitmix64_init(SplitMix64 *state, u64 seed)
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

u64 splitmix64_next(SplitMix64 *state)
{
	return_val_if_fail(state != NULL, 0);

	u64 result = (state->s += 0x9E3779B97f4A7C15);
	result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
	result = (result ^ (result >> 27)) * 0x94D049BB133111EB;

	return result ^ (result >> 31);
}

Xoshiro128 *xoshiro128_init(Xoshiro128 *state, u64 seed)
{
	if (state == NULL) {
		state = talloc(Xoshiro128, 1);

		if (state == NULL) {
			msg_error("couldn't allocate memory for the Xoshiro128 state");
			return NULL;
		}
	}

	SplitMix64 sm;
	splitmix64_init(&sm, seed);

	state->s[0] = splitmix64_next(&sm);
	state->s[1] = splitmix64_next(&sm);

	return state;
}
