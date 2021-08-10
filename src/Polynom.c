#include <stdlib.h>

#include "List.h"
#include "Polynom.h"
#include "Messages.h"
#include "Types.h"

typedef struct _Monom Monom;

struct _Monom
{
	int coef;
	int degree;
	List entry;
};

Polynom* poly_init(Polynom *poly)
{
	if (poly == NULL)
	{
		poly = talloc(Polynom, 1);

		if (poly == NULL)
		{
			msg_error("couldn't allocate memory for the polynom!");
			return NULL;
		}
	}

	poly->count = 0;
	poly->degree = 0;
	list_head_init(&poly->monoms);

	return poly;
}

static Monom* __monom_alloc(int coef, int degree)
{
	Monom *monom = talloc(Monom, 1);

	if (monom == NULL)
	{
		msg_error("couldn't allocate memory for the monom!");
		return NULL;
	}

	monom->coef = coef;
	monom->degree = degree;
	list_node_init(&monom->entry);

	return monom;
}

static int __poly_add_empty(Polynom *poly, int coef, int degree)
{
	Monom *monom = __monom_alloc(coef, degree);
	return_val_if_fail(monom != NULL, -2);
	
	list_append(&poly->monoms, &monom->entry);
	
	poly->count++;
	poly->degree = degree;
	
	return 0;
}

static int __poly_add_reverse(Polynom *poly, int coef, int degree, bool replace)
{
	Monom *monom = __monom_alloc(coef, degree);
	return_val_if_fail(monom != NULL, -2);

	List *iter;

	list_foreach_reverse(iter, &poly->monoms)
	{
		Monom *im = list_entry(iter, Monom, entry);

		if (degree > im->degree)
		{
			list_insert_after(iter, &monom->entry);
			break;
		}
		
		if (degree == im->degree)
		{
			im->coef = (replace) ? coef : im->coef + coef;
			return 0;
		}
	}

	poly->count++;
	poly->degree = (poly->degree < degree) ? degree : poly->degree;

	return 0;
}

static int __poly_add(Polynom *poly, int coef, int degree, bool replace)
{
	Monom *monom = __monom_alloc(coef, degree);
	return_val_if_fail(monom != NULL, -2);

	List *iter;

	list_foreach(iter, &poly->monoms)
	{
		Monom *im = list_entry(iter, Monom, entry);

		if (degree < im->degree)
		{
			list_insert_before(iter, &monom->entry);
			break;
		}

		if (degree == im->degree)
		{
			im->coef = (replace) ? coef : im->coef + coef;
			return 0;
		}
	}

	poly->count++;
	poly->degree = (poly->degree < degree) ? degree : poly->degree;

	return 0;
}

static int __poly_add_monom(Polynom *poly, int coef, int degree, bool replace)
{
	if (coef == 0)
		return 0;

	if (list_empty(&poly->monoms))
		return __poly_add_empty(poly, coef, degree);

	if (degree >= poly->degree / 2)
		return __poly_add_reverse(poly, coef, degree, replace);

	return __poly_add(poly, coef, degree, replace);
}

int poly_set(Polynom *poly, int coef, int degree)
{
	return_val_if_fail(poly != NULL, -1);
	return __poly_add_monom(poly, coef, degree, TRUE);
}

int poly_add_monom(Polynom *poly, int coef, int degree)
{
	return_val_if_fail(poly != NULL, -1);
	return __poly_add_monom(poly, coef, degree, FALSE);
}

static void* __monom_copy(const void *_src)
{
	Monom *res = talloc(Monom, 1);

	if (res == NULL)
	{
		msg_error("couldn't allocate memory for the copy of the monom!");
		return NULL;
	}

	Monom *src = list_entry(_src, Monom, entry);

	res->degree = src->degree;
	res->coef = src->coef;
	list_node_init(&res->entry);

	return &res->entry;
}

Polynom* poly_copy(Polynom *dst, const Polynom *src)
{
	return_val_if_fail(src != NULL, NULL);

	if (dst == NULL)
	{
		dst = talloc(Polynom, 1);

		if (dst == NULL)
		{
			msg_error("couldn't allocate memory for the copy of the polynom!");
			return NULL;
		}
	}

	dst->degree = src->degree;
	dst->count = src->count;
	list_head_init(&dst->monoms);

	list_copy(&dst->monoms, &src->monoms, __monom_copy);

	return dst;
}

static int __poly_append(Polynom *poly, int coef, int degree)
{
	if (coef == 0)
		return 0;

	Monom *monom = __monom_alloc(coef, degree);
	return_val_if_fail(monom != NULL, -2);

	list_append(&poly->monoms, &monom->entry);

	poly->count++;
	poly->degree = (poly->degree < degree) ? degree : poly->degree;

	return 0;
}

Polynom* poly_sum(Polynom *ret, const Polynom *a, const Polynom *b)
{
	return_val_if_fail(a != NULL, NULL);
	return_val_if_fail(b != NULL, NULL);

	ret = poly_init(ret);

	if (ret == NULL)
	{
		msg_error("couldn't allocate memory for the result of the sum!");
		return NULL;
	}

	const List *am = &a->monoms;
	const List *bm = &b->monoms;

	List *a_iter = am->next;
	List *b_iter = bm->next;

	while (1) 
	{
		if (a_iter == am && b_iter == bm)
			break;

		if (a_iter != am && b_iter != bm)
		{
			Monom *a_monom = list_entry(a_iter, Monom, entry);
			Monom *b_monom = list_entry(b_iter, Monom, entry);
			
			if (a_monom->degree == b_monom->degree)
			{
				int coef_sum = a_monom->coef + b_monom->coef;

				int err = __poly_append(ret, coef_sum, a_monom->degree);
				if (err != 0)
					break;

				a_iter = a_iter->next;
				b_iter = b_iter->next;
			}
			else if (a_monom->degree < b_monom->degree)
			{
				int err = __poly_append(ret, a_monom->coef, a_monom->degree);
				if (err != 0)
					break;

				a_iter = a_iter->next;
			}
			else
			{
				int err = __poly_append(ret, b_monom->coef, b_monom->degree);
				if (err != 0) 
					break;

				b_iter = b_iter->next;
			}
		}
		else if (a_iter != am)
		{
			Monom *a_monom = list_entry(a_iter, Monom, entry);
			
			int err = __poly_append(ret, a_monom->coef, a_monom->degree);
			if (err != 0)
				break;

			a_iter = a_iter->next;
		}
		else
		{
			Monom *b_monom = list_entry(b_iter, Monom, entry);

			int err = __poly_append(ret, b_monom->coef, b_monom->degree);
			if (err != 0)
				break;

			b_iter = b_iter->next;
		}
	}

	return ret;
}

Polynom* poly_product(Polynom *ret, const Polynom *a, const Polynom *b)
{
	return_val_if_fail(a != NULL, NULL);
	return_val_if_fail(b != NULL, NULL);

	ret = poly_init(ret);

	if (ret == NULL)
	{
		msg_error("couldn't allocate memory for the result of the sum!");
		return NULL;
	}

	const List *am = &a->monoms;
	const List *bm = &b->monoms;

	List *a_iter;
	List *b_iter;

	list_foreach(a_iter, am)
	{
		list_foreach(b_iter, bm)
		{
			Monom *a_monom = list_entry(a_iter, Monom, entry);
			Monom *b_monom = list_entry(b_iter, Monom, entry);
			
			int coef = a_monom->coef * b_monom->coef;
			int degree = a_monom->degree + b_monom->degree;

			int err = __poly_add_monom(ret, coef, degree, FALSE);
			if (err != 0)
				return ret;
		}
	}

	return ret;
}

Polynom* poly_subtract(Polynom *ret, const Polynom *a, const Polynom *b)
{
	return_val_if_fail(a != NULL, NULL);
	return_val_if_fail(b != NULL, NULL);

	ret = poly_init(ret);

	if (ret == NULL)
	{
		msg_error("couldn't allocate memory for the result of the sum!");
		return NULL;
	}

	const List *am = &a->monoms;
	const List *bm = &b->monoms;

	List *a_iter = am->next;
	List *b_iter = bm->next;

	while (1) 
	{
		if (a_iter == am && b_iter == bm)
			break;

		if (a_iter != am && b_iter != bm)
		{
			Monom *a_monom = list_entry(a_iter, Monom, entry);
			Monom *b_monom = list_entry(b_iter, Monom, entry);

			if (a_monom->degree == b_monom->degree)
			{
				int coef_sub = a_monom->coef - b_monom->coef;

				int err = __poly_append(ret, coef_sub, a_monom->degree);
				if (err != 0)
					break;

				a_iter = a_iter->next;
				b_iter = b_iter->next;
			}
			else if (a_monom->degree < b_monom->degree)
			{
				int err = __poly_append(ret, a_monom->coef * -1, a_monom->degree);
				if (err != 0)
					break;

				a_iter = a_iter->next;
			}
			else
			{
				int err = __poly_append(ret, b_monom->coef * -1, b_monom->degree);
				if (err != 0) 
					break;

				b_iter = b_iter->next;
			}
		}
		else if (a_iter != am)
		{
			Monom *a_monom = list_entry(a_iter, Monom, entry);

			int err = __poly_append(ret, a_monom->coef * -1, a_monom->degree);
			if (err != 0)
				break;

			a_iter = a_iter->next;
		}
		else
		{
			Monom *b_monom = list_entry(b_iter, Monom, entry);

			int err = __poly_append(ret, b_monom->coef * -1, b_monom->degree);
			if (err != 0)
				break;

			b_iter = b_iter->next;
		}
	}

	return ret;
}

void poly_add(Polynom *poly, const Polynom *add)
{
	return_if_fail(poly != NULL);
	return_if_fail(add != NULL);

	List *iter;

	list_foreach(iter, &add->monoms)
	{
		Monom *im = list_entry(iter, Monom, entry);
		
		int err = __poly_add_monom(poly, im->coef, im->degree, FALSE);
		if (err != 0)
			break;
	}
}

void poly_sub(Polynom *poly, const Polynom *sub)
{
	return_if_fail(poly != NULL);
	return_if_fail(sub != NULL);

	List *iter;

	list_foreach(iter, &sub->monoms)
	{
		Monom *im = list_entry(iter, Monom, entry);

		int err = __poly_add_monom(poly, im->coef * -1, im->degree, FALSE);
		if (err != 0)
			break;
	}
}

void poly_mul(Polynom *poly, const Polynom *mul)
{
	return_if_fail(poly != NULL);
	return_if_fail(mul != NULL);
}

void poly_print(Polynom *poly)
{
	return_if_fail(poly != NULL);

	if (list_empty(&poly->monoms))
	{
		printf("0\n");
		return;
	}

	List *iter;

	list_foreach(iter, &poly->monoms)
	{
		Monom *im = list_entry(iter, Monom, entry);
		printf("%dx^%d ", im->coef, im->degree);
	}

	printf("\n");
}
