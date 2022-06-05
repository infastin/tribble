#ifndef MACROS_H_4JJFR6VE
#define MACROS_H_4JJFR6VE

#include "trb-types.h"

#include <malloc.h>

/**
 * trb_talloc:
 * @struct_type: The structure type.
 * @n_structs: The number of structures to be allocated.
 *
 * Allocates @n_structs of the @struct_type using `malloc()`.
 **/
#define trb_talloc(struct_type, n_structs) ((struct_type *) ((n_structs > 0) ? (malloc(sizeof(struct_type) * n_structs)) : (NULL)))

/**
 * trb_talloc0:
 * @struct_type: The structure type.
 * @n_structs: The number of structures to be allocated.
 *
 * Allocates @n_structs of the @struct_type using `calloc()`.
 **/
#define trb_talloc0(struct_type, n_structs) ((struct_type *) ((n_structs > 0) ? (calloc(n_structs, sizeof(struct_type))) : (NULL)))

#define trb_get_ptr(type, ...) ((type *) &((type){ __VA_ARGS__ }))
#define trb_get_arr(type, len, ...) ((type[len]){ __VA_ARGS__ })

#define trb_array_cell(m, e, i) ((void *) &((char *) (m))[(i) * (e)])
#define trb_array_get(m, t, i) ((t *) (array_cell((m), sizeof(t), (i))))

/**
 * trb_memswap:
 * @a: The first pointer.
 * @b: The second pointer.
 * @size: The amount of bytes to be swapped.
 *
 * Swaps @size bytes in @a and @b.
 **/
#define trb_memswap(a, b, size) \
	{                           \
		usize __size = (size);  \
		char *__a = (a);        \
		char *__b = (b);        \
		do {                    \
			char __tmp = *__a;  \
			*__a++ = *__b;      \
			*__b++ = __tmp;     \
		} while (--__size > 0); \
	}

#ifndef offsetof
	#define offsetof(type, member) ((usize) & ((type *) 0)->member)
#endif

/**
 * trb_distance_of:
 * @type: The structure type.
 * @m1: Some member of the @type.
 * @m2: Another member of the @type.
 *
 * Returns the distance between @m1 and @m2 in the @type.
 **/
#define trb_distance_of(type, m1, m2) ({              \
	const usize __m1_offset = offsetof(type, m1);     \
	const usize __m1_size = sizeof(((type *) 0)->m1); \
	const usize __m2_offset = offsetof(type, m2);     \
	const usize __m2_size = sizeof(((type *) 0)->m2); \
	(__m1_offset > __m2_offset)                       \
		? (__m1_offset - __m2_size)                   \
	: (__m1_offset < __m2_offset)                     \
		? (__m2_offset - __m1_size)                   \
		: 0;                                          \
})

#define trb_container_of(ptr, type, member) ({             \
	const typeof(((type *) 0)->member) *__mptr = (ptr);    \
	(type *) (((char *) __mptr) - offsetof(type, member)); \
})

#endif /* end of include guard: MACROS_H_4JJFR6VE */
