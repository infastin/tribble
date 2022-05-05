#ifndef MACROS_H_4JJFR6VE
#define MACROS_H_4JJFR6VE

#include "Types.h"

#include <malloc.h>

#define STRFUNC ((const char *) (__PRETTY_FUNCTION__))

#define talloc(struct_type, n_structs) ((struct_type *) ((n_structs > 0) ? (malloc(sizeof(struct_type) * n_structs)) : (NULL)))
#define talloc0(struct_type, n_structs) ((struct_type *) ((n_structs > 0) ? (calloc(n_structs, sizeof(struct_type))) : (NULL)))

#define GET_PTR(type, ...) ((type *) &((type){ __VA_ARGS__ }))
#define GET_ARR(type, len, ...) ((type[len]){ __VA_ARGS__ })

#define mass_cell(m, e, i) ((void *) &((char *) (m))[(i) * (e)])
#define mass_get(m, t, i) ((t *) (mass_cell((m), sizeof(t), (i))))

#define mass_swap(a, b, elemsize)  \
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

#define ROTL32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))
#define ROTR32(x, r) (((x) >> (r)) | ((x) << (32 - (r))))

#define ROTL64(x, r) (((x) << (r)) | ((x) >> (64 - (r))))
#define ROTR64(x, r) (((x) >> (r)) | ((x) << (64 - (r))))

/* Linux kernel vibes */

#ifndef offsetof
	#define offsetof(type, member) ((usize) & ((type *) 0)->member)
#endif

#define container_of(ptr, type, member) ({                 \
	const typeof(((type *) 0)->member) *__mptr = (ptr);    \
	(type *) (((char *) __mptr) - offsetof(type, member)); \
})

#endif /* end of include guard: MACROS_H_4JJFR6VE */
