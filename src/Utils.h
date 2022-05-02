#ifndef UTILS_H_ATMQZMHO
#define UTILS_H_ATMQZMHO

#include "Types.h"

#include <stdarg.h>

char *strdup_printf(const char *fmt, ...);
char *strdup_vprintf(const char *fmt, va_list *ap);

u32 pow2_32(u32 value);
u64 pow2_64(u64 value);

void inssort(void *mass, u32 len, u32 elemsize, CmpFunc cmp_func);
void heapsort(void *mass, u32 len, u32 elemsize, CmpFunc cmp_func);
void quicksort(void *mass, u32 len, u32 elemsize, CmpFunc cmp_func);

bool linear_search(void *mass, const void *target, u32 len, u32 elemsize, CmpFunc cmp_func, u32 *index);
bool binary_search(void *mass, const void *target, u32 len, u32 elemsize, CmpFunc cmp_func, u32 *index);

#endif /* end of include guard: UTILS_H_ATMQZMHO */
