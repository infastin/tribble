#ifndef RAND_H_B1PCU4KT
#define RAND_H_B1PCU4KT

#include "Types.h"

/**
 * TrbSM64:
 * @s: The state of the generator.
 *
 * SplitMix64 - is a fast, high-quality seudorandom number generator.
 * Recommended for initializing generators of the xoshiro familiy from a 64-bit seed.
 *
 * [Reference](https://prng.di.unimi.it/).
 **/
typedef struct _TrbSM64 TrbSM64;

struct _TrbSM64 {
	u64 s;
};

/**
 * trb_sm64_init:
 * @self: The pointer to the state to be initialized.
 * @seed: The seed to initialize with.
 *
 * Initializes the state of #TrbSM64 with the given seed.
 *
 * Returns: A new state. Can return %NULL if an allocation error occurs.
 **/
TrbSM64 *trb_sm64_init(TrbSM64 *self, u64 seed);

/**
 * trb_sm64_next_u32:
 * @self: The pointer to the state.
 *
 * Generates a random number with the given state.
 *
 * Returns: A random number in the range [0, U32_MAX].
 **/
u32 trb_sm64_next_u32(TrbSM64 *self);

/**
 * trb_sm64_next_u64:
 * @self: The pointer to the state.
 *
 * Generates a random number with the given state.
 *
 * Returns: A random number in the range [0, U64_MAX].
 **/
u64 trb_sm64_next_u64(TrbSM64 *self);

/**
 * TrbXs128ss:
 * @s: The state of the generator.
 *
 * Xoshiro128** - is all-purpose, rock-solid generator.
 * It is extremely (sub-ns) fast and it passes all tests.
 *
 * [Reference](https://prng.di.unimi.it/).
 **/
typedef struct _TrbXs128ss TrbXs128ss;

struct _TrbXs128ss {
	u32 s[4];
};

/**
 * trb_xs128ss_init:
 * @self: The pointer to the state to be initialized.
 * @seed: The seed to initialize with.
 *
 * Initializes the state of #TrbXs128ss with the given seed.
 *
 * Returns: A new state. Can return %NULL if an allocation error occurs.
 **/
TrbXs128ss *trb_xs128ss_init(TrbXs128ss *self, u64 seed);

/**
 * xs128ss_next:
 * @self: The pointer to the state.
 *
 * Generates a random number with the given state.
 *
 * Returns: A random number in the range [0, U32_MAX].
 **/
u32 trb_xs128ss_next(TrbXs128ss *self);

/**
 * TrbXs128p:
 * @s: The state of the generator;
 *
 * Xoshiro128+ - is the best and fastest 32-bit generator from Xorshift family for 32-bit floating-point numbers.
 * It passes all tests we are aware of except for
 * linearity tests, as the lowest four bits have low linear complexity, so
 * if low linear complexity is not considered an issue (as it is usually the case)
 * it can be used to generate 32-bit outputs, too.
 *
 * [Reference](https://prng.di.unimi.it/).
 **/
typedef struct _TrbXs128p TrbXs128p;

struct _TrbXs128p {
	u32 s[4];
};

/**
 * trb_xs128p_init:
 * @self: The pointer to the state to be initialized.
 * @seed: The seed to initialize with.
 *
 * Initializes the state of #TrbXs128p with the given seed.
 *
 * Returns: A new state. Can return %NULL if an allocation error occurs.
 **/
TrbXs128p *trb_xs128p_init(TrbXs128p *self, u64 seed);

/**
 * trb_xs128p_next_u32:
 * @self: The pointer to the state.
 *
 * Generates a random number with the given state.
 *
 * Returns: A random number in the range [0, U32_MAX].
 **/
u32 trb_xs128p_next_u32(TrbXs128p *self);

/**
 * trb_xs128p_next_f32:
 * @self: The pointer to the state.
 *
 * Generates a random floating-point number with the given state.
 *
 * Returns: A random number in the range [0, 1].
 **/
f32 trb_xs128p_next_f32(TrbXs128p *self);

/**
 * TrbXs256ss:
 * @s: The state of the generator.
 *
 * Xoshiro256** - is all-purpose, rock-solid generator.
 * It has excellent (sub-ns) speed and it passes all tests.
 *
 * [Reference](https://prng.di.unimi.it/).
 **/
typedef struct _TrbXs256ss TrbXs256ss;

struct _TrbXs256ss {
	u64 s[4];
};

/**
 * trb_xs256ss_init:
 * @self: The pointer to the state to be initialized.
 * @seed: The seed to initialize with.
 *
 * Initializes the state of #TrbXs256ss with the given seed.
 *
 * Returns: A new state. Can return %NULL if an allocation error occurs.
 **/
TrbXs256ss *trb_xs256ss_init(TrbXs256ss *self, u64 seed);

/**
 * xs256ss_next:
 * @self: The pointer to the state.
 *
 * Generates a random number with the given state.
 *
 * Returns: A random number in the range [0, U64_MAX].
 **/
u64 trb_xs256ss_next(TrbXs256ss *self);

/**
 * TrbXs256p:
 * @s: The state of the generator;
 *
 * Xoshiro256+ - is the best and fastest generator from Xorshift family for floating-point numbers.
 * It passes all tests except for the lowest three bits,
 * which might fail linearity tests (and just those), so if low linear
 * complexity is not considered an issue (as it is usually the case) it
 * can be used to generate 64-bit outputs, too.
 *
 * [Reference](https://prng.di.unimi.it/).
 **/
typedef struct _TrbXs256p TrbXs256p;

struct _TrbXs256p {
	u64 s[4];
};

/**
 * trb_xs256p_init:
 * @self: The pointer to the state to be initialized.
 * @seed: The seed to initialize with.
 *
 * Initializes the state of #TrbXs256p with the given seed.
 *
 * Returns: A new state. Can return %NULL if an allocation error occurs.
 **/
TrbXs256p *trb_xs256p_init(TrbXs256p *self, u64 seed);

/**
 * trb_xs256p_next_u64:
 * @self: The pointer to the state.
 *
 * Generates a random number with the given state.
 *
 * Returns: A random number in the range [0, U64_MAX].
 **/
u64 trb_xs256p_next_u64(TrbXs256p *self);

/**
 * trb_xs256p_next_f64:
 * @self: The pointer to the state.
 *
 * Generates a random floating-point number with the given state.
 *
 * Returns: A random number in the range [0, 1].
 **/
f64 trb_xs256p_next_f64(TrbXs256p *self);

#endif /* end of include guard: RAND_H_B1PCU4KT */
