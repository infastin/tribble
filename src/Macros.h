#ifndef MACROS_H_4JJFR6VE
#define MACROS_H_4JJFR6VE

#include "Types.h"

#include <malloc.h>

#define STRFUNC ((const char *) (__PRETTY_FUNCTION__))
#define STRFILE ((const char *) (__FILE__))
#define USIZELINE ((usize) (__LINE__))

#define talloc(struct_type, n_structs) ((struct_type *) ((n_structs > 0) ? (malloc(sizeof(struct_type) * n_structs)) : (NULL)))
#define talloc0(struct_type, n_structs) ((struct_type *) ((n_structs > 0) ? (calloc(n_structs, sizeof(struct_type))) : (NULL)))

#define get_ptr(type, ...) ((type *) &((type){ __VA_ARGS__ }))
#define get_arr(type, len, ...) ((type[len]){ __VA_ARGS__ })

#define array_cell(m, e, i) ((void *) &((char *) (m))[(i) * (e)])
#define array_get(m, t, i) ((t *) (array_cell((m), sizeof(t), (i))))

#define array_swap(a, b, elemsize) \
	{                              \
		usize __size = (elemsize); \
		char *__a = (a);           \
		char *__b = (b);           \
		do {                       \
			char __tmp = *__a;     \
			*__a++ = *__b;         \
			*__b++ = __tmp;        \
		} while (--__size > 0);    \
	}

#define rotl32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))
#define rotr32(x, r) (((x) >> (r)) | ((x) << (32 - (r))))

#define rotl64(x, r) (((x) << (r)) | ((x) >> (64 - (r))))
#define rotr64(x, r) (((x) >> (r)) | ((x) << (64 - (r))))

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

#define FORMAT(archetype, index, first) __attribute__((format(archetype, index, first)))
#define UNUSED __attribute__((unused))
#define PACKED __attribute__((packed))

#define distance_of(type, m1, m2) ({                  \
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

#ifndef offsetof
	#define offsetof(type, member) ((usize) & ((type *) 0)->member)
#endif

#define container_of(ptr, type, member) ({                 \
	const typeof(((type *) 0)->member) *__mptr = (ptr);    \
	(type *) (((char *) __mptr) - offsetof(type, member)); \
})

#endif /* end of include guard: MACROS_H_4JJFR6VE */
