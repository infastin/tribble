#ifndef UTILS_H_ATMQZMHO
#define UTILS_H_ATMQZMHO

#include <stdarg.h>

#include "Types.h"

char* strdup_printf(const char *fmt, ...);
char* strdup_vprintf(const char *fmt, va_list *ap);

uint pow2(uint value);
ulong pow2l(ulong value);
ullong pow2ll(ullong value);

void inssort(void *mass, uint len, uint elemsize, CmpFunc cmp_func);
void heapsort(void *mass, uint len, uint elemsize, CmpFunc cmp_func);
void quicksort(void *mass, uint len, uint elemsize, CmpFunc cmp_func);

bool linear_search(void *mass, const void *target, uint len, uint elemsize, CmpFunc cmp_func, uint *index);
bool binary_search(void *mass, const void *target, uint len, uint elemsize, CmpFunc cmp_func, uint *index);

#endif /* end of include guard: UTILS_H_ATMQZMHO */
