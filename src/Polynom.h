#ifndef POLYNOM_H_JR2FKQMF
#define POLYNOM_H_JR2FKQMF

#include "List.h"

typedef struct _Polynom Polynom;

struct _Polynom {
	int32_t degree;
	uint32_t count;
	List monoms;
};

Polynom *poly_init(Polynom *poly);
bool poly_set(Polynom *poly, int32_t coef, int32_t degree);
Polynom *poly_copy(Polynom *dst, const Polynom *src);
Polynom *poly_sum(Polynom *ret, const Polynom *a, const Polynom *b);
void poly_add(Polynom *poly, const Polynom *add);
Polynom *poly_subtract(Polynom *ret, const Polynom *a, const Polynom *b);
Polynom *poly_product(Polynom *ret, const Polynom *a, const Polynom *b);
void poly_sub(Polynom *poly, const Polynom *sub);
void poly_mul(Polynom *poly, const Polynom *mul);
void poly_print(Polynom *poly);

#endif /* end of include guard: POLYNOM_H_JR2FKQMF */
