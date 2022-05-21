#ifndef MACROS_H_4JJFR6VE
#define MACROS_H_4JJFR6VE

#include "Types.h"

#include <malloc.h>

#define trb_talloc(struct_type, n_structs) ((struct_type *) ((n_structs > 0) ? (malloc(sizeof(struct_type) * n_structs)) : (NULL)))
#define trb_talloc0(struct_type, n_structs) ((struct_type *) ((n_structs > 0) ? (calloc(n_structs, sizeof(struct_type))) : (NULL)))

#define trb_get_ptr(type, ...) ((type *) &((type){ __VA_ARGS__ }))
#define trb_get_arr(type, len, ...) ((type[len]){ __VA_ARGS__ })

#define trb_array_cell(m, e, i) ((void *) &((char *) (m))[(i) * (e)])
#define trb_array_get(m, t, i) ((t *) (array_cell((m), sizeof(t), (i))))

#define trb_array_swap(a, b, elemsize) \
	{                                  \
		usize __size = (elemsize);     \
		char *__a = (a);               \
		char *__b = (b);               \
		do {                           \
			char __tmp = *__a;         \
			*__a++ = *__b;             \
			*__b++ = __tmp;            \
		} while (--__size > 0);        \
	}

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

/* Linux kernel vibes */

#define trb_container_of(ptr, type, member) ({             \
	const typeof(((type *) 0)->member) *__mptr = (ptr);    \
	(type *) (((char *) __mptr) - offsetof(type, member)); \
})

#endif /* end of include guard: MACROS_H_4JJFR6VE */
