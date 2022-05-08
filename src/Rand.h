#ifndef RAND_H_B1PCU4KT
#define RAND_H_B1PCU4KT

#include "Types.h"

/* SplitMix64 - a fast, high-quality seudorandom number generator.
 * Recommended for initializing generators of the xoshiro familiy from a 64-bit seed.
 */
typedef struct _SplitMix64 SplitMix64;

struct _SplitMix64 {
	u64 s;
};

/**
 * @brief Initializes the state of SplitMix64 with the given seed.
 *
 * @param state The pointer to the state to be initialized (can be NULL).
 * @param seed The seed to initialize with.
 *
 * @return A new state.
 */
SplitMix64 *sm64_init(SplitMix64 *state, u64 seed);

/**
 * @brief Generates a random number with the given state.
 *
 * @param state The pointer to the state.
 *
 * @return A random number in the range [0, U32_MAX].
 */
u32 sm64_next_u32(SplitMix64 *state);

/**
 * @brief Generates a random number with the given state.
 *
 * @param state The pointer to the state.
 *
 * @return A random number in the range [0, U64_MAX].
 */
u64 sm64_next_u64(SplitMix64 *state);

/**
 * xoshiro128** - all-purpose, rock-solid generator.
 * It is extremely (sub-ns) fast and it passes all tests.
 */
typedef struct _Xoshiro128StarStar Xoshiro128StarStar;

struct _Xoshiro128StarStar {
	u32 s[4];
};

/**
 * @brief Initializes the state of xoshiro128** with the given seed.
 *
 * @param state The pointer to the state to be initialized (can be NULL).
 * @param seed The seed to initialize with.
 *
 * @return A new state.
 */
Xoshiro128StarStar *xs128ss_init(Xoshiro128StarStar *state, u64 seed);

/**
 * @brief Generates a random number with the given state.
 *
 * @param state The pointer to the state.
 *
 * @return A random number in the range [0, U32_MAX].
 */
u32 xs128ss_next(Xoshiro128StarStar *state);

/**
 * xoshiro128+ - best and fastest 32-bit generator for 32-bit floating-point numbers.
 * It passes all tests we are aware of except for
 * linearity tests, as the lowest four bits have low linear complexity, so
 * if low linear complexity is not considered an issue (as it is usually the case)
 * it can be used to generate 32-bit outputs, too.
 */
typedef struct _Xoshiro128Plus Xoshiro128Plus;

struct _Xoshiro128Plus {
	u32 s[4];
};

/**
 * @brief Initializes the state of xoshiro128+ with the given seed.
 *
 * @param state The pointer to the state to be initialized (can be NULL).
 * @param seed The seed to initialize with.
 *
 * @return A new state.
 */
Xoshiro128Plus *xs128p_init(Xoshiro128Plus *state, u64 seed);

/**
 * @brief Generates a random number with the given state.
 *
 * @param state The pointer to the state.
 *
 * @return A random number in the range [0, U32_MAX].
 */
u32 xs128p_next_u32(Xoshiro128Plus *state);

/**
 * @brief Generates a random floating-point number with the given state.
 *
 * @param state The pointer to the state.
 *
 * @return A random number in the range [0, 1].
 */
f32 xs128p_next_f32(Xoshiro128Plus *state);

/**
 * xoshiro256** - all-purpose, rock-solid generator.
 * It has excellent (sub-ns) speed and it passes all tests.
 */
typedef struct _Xoshiro256StarStar Xoshiro256StarStar;

struct _Xoshiro256StarStar {
	u64 s[4];
};

/**
 * @brief Initializes the state of xoshiro256** with the given seed.
 *
 * @param state The pointer to the state to be initialized (can be NULL).
 * @param seed The seed to initialize with.
 *
 * @return A new state.
 */
Xoshiro256StarStar *xs256ss_init(Xoshiro256StarStar *state, u64 seed);

/**
 * @brief Generates a random number with the given state.
 *
 * @param state The pointer to the state.
 *
 * @return A random number in the range [0, U64_MAX].
 */
u64 xs256ss_next(Xoshiro256StarStar *state);

/**
 * xoshiro256+ - best and fastest generator for floating-point numbers.
 * It passes all tests except for the lowest three bits,
 * which might fail linearity tests (and just those), so if low linear
 * complexity is not considered an issue (as it is usually the case) it
 * can be used to generate 64-bit outputs, too.
 */
typedef struct _Xoshiro256Plus Xoshiro256Plus;

struct _Xoshiro256Plus {
	u64 s[4];
};

/**
 * @brief Initializes the state of xoshiro256+ with the given seed.
 *
 * @param state The pointer to the state to be initialized (can be NULL).
 * @param seed The seed to initialize with.
 *
 * @return A new state.
 */
Xoshiro256Plus *xs256p_init(Xoshiro256Plus *state, u64 seed);

/**
 * @brief Generates a random number with the given state.
 *
 * @param state The pointer to the state.
 *
 * @return A random number in the range [0, U64_MAX].
 */
u64 xs256p_next_u64(Xoshiro256Plus *state);

/**
 * @brief Generates a random floating-point number with the given state.
 *
 * @param state The pointer to the state.
 *
 * @return A random number in the range [0, 1].
 */
f64 xs256p_next_f64(Xoshiro256Plus *state);

#endif /* end of include guard: RAND_H_B1PCU4KT */
