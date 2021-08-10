#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "Polynom.h"
#include "Macros.h"

int main(int argc, char *argv[])
{
	srand(time(0));

	Polynom poly;
	poly_init(&poly);

	for (int i = 0; i < 5; ++i) 
	{
		poly_set(&poly, rand() % 100, i);
	}

	poly_set(&poly, 32, 10);
	poly_set(&poly, 44, 6);

	Polynom pol;
	poly_init(&pol);

	for (int i = 0; i < 7; ++i) 
	{
		poly_set(&pol, rand() % 100, i);
	}

	Polynom prod;
	poly_product(&prod, &poly, &pol);

	poly_print(&prod);

	return 0;
}
