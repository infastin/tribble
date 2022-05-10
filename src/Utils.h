#ifndef UTILS_H_ATMQZMHO
#define UTILS_H_ATMQZMHO

#include "Types.h"

#include <stdarg.h>

usize strfmt(char **buf, const char *fmt, ...);
usize vstrfmt(char **buf, const char *fmt, va_list args);

char *strdup_fmt(const char *fmt, ...);
char *strdup_vfmt(const char *fmt, va_list args);

u32 pow2_32(u32 value);
u64 pow2_64(u64 value);

void heapify(void *array, usize len, usize elemsize, CmpFunc cmp_func);

void inssort(void *array, usize len, usize elemsize, CmpFunc cmp_func);
void heapsort(void *array, usize len, usize elemsize, CmpFunc cmp_func);
void quicksort(void *array, usize len, usize elemsize, CmpFunc cmp_func);

bool linear_search(const void *array, const void *target, usize len, usize elemsize, CmpFunc cmp_func, usize *index);
bool binary_search(const void *array, const void *target, usize len, usize elemsize, CmpFunc cmp_func, usize *index);

i32 u8cmp(const u8 *a, const u8 *b);
i32 u16cmp(const u16 *a, const u16 *b);
i32 u32cmp(const u32 *a, const u32 *b);
i32 u64cmp(const u64 *a, const u64 *b);
i32 usizecmp(const usize *a, const usize *b);

i32 i8cmp(const i8 *a, const i8 *b);
i32 i16cmp(const i16 *a, const i16 *b);
i32 i32cmp(const i32 *a, const i32 *b);
i32 i64cmp(const i64 *a, const i64 *b);
i32 isizecmp(const isize *a, const isize *b);

i32 f32cmp(const f32 *a, const f32 *b);
i32 f64cmp(const f64 *a, const f64 *b);
i32 f128cmp(const f128 *a, const f128 *b);

#endif /* end of include guard: UTILS_H_ATMQZMHO */
