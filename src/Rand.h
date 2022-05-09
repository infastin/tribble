#ifndef RAND_H_B1PCU4KT
#define RAND_H_B1PCU4KT

#include "Types.h"

/**
 * SplitMix64:
 * @s: The state of the generator.
 *
 * It is a fast, high-quality seudorandom number generator.
 * Recommended for initializing generators of the xoshiro familiy from a 64-bit seed.
 *
 * [Reference](https://prng.di.unimi.it/).
 **/
typedef struct _SplitMix64 SplitMix64;

struct _SplitMix64 {
	u64 s;
};

/**
 * sm64_init:
 * @state: The pointer to the state to be initialized (can be `NULL`).
 * @seed: The seed to initialize with.
 *
 * Initializes the state of SplitMix64 with the given seed.
 *
 * Returns: A new state.
 **/
SplitMix64 *sm64_init(SplitMix64 *state, u64 seed);

/**
 * sm64_next_u32:
 * @state: The pointer to the state.
 *
 * Generates a random number with the given state.
 *
 * Returns: A random number in the range [0, U32_MAX].
 **/
u32 sm64_next_u32(SplitMix64 *state);

/**
 * sm64_next_u64:
 * @state: The pointer to the state.
 *
 * Generates a random number with the given state.
 *
 * Returns: A random number in the range [0, U64_MAX].
 **/
u64 sm64_next_u64(SplitMix64 *state);

/**
 * Xoshiro128StarStar:
 * @s: The state of the generator.
 *
 * It is all-purpose, rock-solid generator.
 * It is extremely (sub-ns) fast and it passes all tests.
 *
 * [Reference](https://prng.di.unimi.it/).
 **/
typedef struct _Xoshiro128StarStar Xoshiro128StarStar;

struct _Xoshiro128StarStar {
	u32 s[4];
};

/**
 * xs128ss_init:
 * @state: The pointer to the state to be initialized (can be `NULL`).
 * @seed: The seed to initialize with.
 *
 * Initializes the state of xoshiro128** with the given seed.
 *
 * Returns: A new state.
 **/
Xoshiro128StarStar *xs128ss_init(Xoshiro128StarStar *state, u64 seed);

/**
 * xs128ss_next:
 * @state: The pointer to the state.
 *
 * Generates a random number with the given state.
 *
 * Returns: A random number in the range [0, U32_MAX].
 **/
u32 xs128ss_next(Xoshiro128StarStar *state);

/**
 * Xoshiro128Plus:
 * @s: The state of the generator;
 *
 * It is the best and fastest 32-bit generator from Xorshift family for 32-bit floating-point numbers.
 * It passes all tests we are aware of except for
 * linearity tests, as the lowest four bits have low linear complexity, so
 * if low linear complexity is not considered an issue (as it is usually the case)
 * it can be used to generate 32-bit outputs, too.
 *
 * [Reference](https://prng.di.unimi.it/).
 **/
typedef struct _Xoshiro128Plus Xoshiro128Plus;

struct _Xoshiro128Plus {
	u32 s[4];
};

/**
 * xs128p_init:
 * @state: The pointer to the state to be initialized (can be `NULL`).
 * @seed: The seed to initialize with.
 *
 * Initializes the state of xoshiro128+ with the given seed.
 *
 * Returns: A new state.
 **/
Xoshiro128Plus *xs128p_init(Xoshiro128Plus *state, u64 seed);

/**
 * xs128p_next_u32:
 * @state: The pointer to the state.
 *
 * Generates a random number with the given state.
 *
 * Returns: A random number in the range [0, U32_MAX].
 **/
u32 xs128p_next_u32(Xoshiro128Plus *state);

/**
 * xs128p_next_f32:
 * @state: The pointer to the state.
 *
 * Generates a random floating-point number with the given state.
 *
 * Returns: A random number in the range [0, 1].
 **/
f32 xs128p_next_f32(Xoshiro128Plus *state);

/**
 * Xoshiro256StarStar:
 * @s: The state of the generator.
 *
 * It is all-purpose, rock-solid generator.
 * It has excellent (sub-ns) speed and it passes all tests.
 *
 * [Reference](https://prng.di.unimi.it/).
 **/
typedef struct _Xoshiro256StarStar Xoshiro256StarStar;

struct _Xoshiro256StarStar {
	u64 s[4];
};

/**
 * xs256ss_init:
 * @state: The pointer to the state to be initialized (can be `NULL`).
 * @seed: The seed to initialize with.
 *
 * Initializes the state of xoshiro256** with the given seed.
 *
 * Returns: A new state.
 **/
Xoshiro256StarStar *xs256ss_init(Xoshiro256StarStar *state, u64 seed);

/**
 * xs256ss_next:
 * @state: The pointer to the state.
 *
 * Generates a random number with the given state.
 *
 * Returns: A random number in the range [0, U64_MAX].
 **/
u64 xs256ss_next(Xoshiro256StarStar *state);

/**
 * Xoshiro256Plus:
 * @s: The state of the generator;
 *
 * It is the best and fastest generator from Xorshift family for floating-point numbers.
 * It passes all tests except for the lowest three bits,
 * which might fail linearity tests (and just those), so if low linear
 * complexity is not considered an issue (as it is usually the case) it
 * can be used to generate 64-bit outputs, too.
 *
 * [Reference](https://prng.di.unimi.it/).
 **/
typedef struct _Xoshiro256Plus Xoshiro256Plus;

struct _Xoshiro256Plus {
	u64 s[4];
};

/**
 * xs256p_init:
 * @state: The pointer to the state to be initialized (can be `NULL`).
 * @seed: The seed to initialize with.
 *
 * Initializes the state of xoshiro256+ with the given seed.
 *
 * Returns: A new state.
 **/
Xoshiro256Plus *xs256p_init(Xoshiro256Plus *state, u64 seed);

/**
 * xs256p_next_u64:
 * @state: The pointer to the state.
 *
 * Generates a random number with the given state.
 *
 * Returns: A random number in the range [0, U64_MAX].
 **/
u64 xs256p_next_u64(Xoshiro256Plus *state);

/**
 * xs256p_next_f64:
 * @state: The pointer to the state.
 *
 * Generates a random floating-point number with the given state.
 *
 * Returns: A random number in the range [0, 1].
 **/
f64 xs256p_next_f64(Xoshiro256Plus *state);

#endif /* end of include guard: RAND_H_B1PCU4KT */
