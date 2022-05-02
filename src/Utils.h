#ifndef UTILS_H_ATMQZMHO
#define UTILS_H_ATMQZMHO

#include "Types.h"

#include <stdarg.h>

char *strdup_printf(const char *fmt, ...);
char *strdup_vprintf(const char *fmt, va_list *ap);

uint32_t pow2_32(uint32_t value);
uint64_t pow2_64(uint64_t value);

void inssort(void *mass, uint32_t len, uint32_t elemsize, CmpFunc cmp_func);
void heapsort(void *mass, uint32_t len, uint32_t elemsize, CmpFunc cmp_func);
void quicksort(void *mass, uint32_t len, uint32_t elemsize, CmpFunc cmp_func);

bool linear_search(void *mass, const void *target, uint32_t len, uint32_t elemsize, CmpFunc cmp_func, uint32_t *index);
bool binary_search(void *mass, const void *target, uint32_t len, uint32_t elemsize, CmpFunc cmp_func, uint32_t *index);

#endif /* end of include guard: UTILS_H_ATMQZMHO */
