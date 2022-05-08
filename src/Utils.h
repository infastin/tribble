#ifndef UTILS_H_ATMQZMHO
#define UTILS_H_ATMQZMHO

#include "Types.h"

#include <stdarg.h>

char *strdup_printf(const char *fmt, ...);
char *strdup_vprintf(const char *fmt, va_list *ap);

u32 pow2_32(u32 value);
u64 pow2_64(u64 value);

void heapify(void *array, usize len, usize elemsize, CmpFunc cmp_func);

void inssort(void *array, usize len, usize elemsize, CmpFunc cmp_func);
void heapsort(void *array, usize len, usize elemsize, CmpFunc cmp_func);
void quicksort(void *array, usize len, usize elemsize, CmpFunc cmp_func);

bool linear_search(const void *array, const void *target, usize len, usize elemsize, CmpFunc cmp_func, usize *index);
bool binary_search(const void *array, const void *target, usize len, usize elemsize, CmpFunc cmp_func, usize *index);

i32 u8cmp(const void *a, const void *b);
i32 u16cmp(const void *a, const void *b);
i32 u32cmp(const void *a, const void *b);
i32 u64cmp(const void *a, const void *b);
i32 usizecmp(const void *a, const void *b);

i32 i8cmp(const void *a, const void *b);
i32 i16cmp(const void *a, const void *b);
i32 i32cmp(const void *a, const void *b);
i32 i64cmp(const void *a, const void *b);
i32 isizecmp(const void *a, const void *b);

i32 f32cmp(const void *a, const void *b);
i32 f64cmp(const void *a, const void *b);
i32 f128cmp(const void *a, const void *b);

#endif /* end of include guard: UTILS_H_ATMQZMHO */
