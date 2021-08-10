#ifndef DEFINITIONS_H_N6TPGG3D
#define DEFINITIONS_H_N6TPGG3D

#include <limits.h>
#include <stdint.h>

/* Integers */

typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef unsigned long long ullong;

typedef uintptr_t          size_t;
typedef uintptr_t          word_t;

#define UINT_BIT (sizeof(uint) * 8)
#define ULONG_BIT (sizeof(ulong) * 8)
#define ULLONG_BIT (sizeof(ullong) * 8)

/* Boolean */

#ifdef bool
#undef bool
#endif

typedef _Bool bool;

#define TRUE 1
#define FALSE 0

/* Functions */

typedef int   (*CmpFunc)(const void *a, const void *b);
typedef void* (*CopyFunc)(const void *src);
typedef void  (*FreeFunc)(void *ptr);
typedef void  (*UserFunc)(void *data, void *userdata);

#endif /* end of include guard: DEFINITIONS_H_N6TPGG3D */
