#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"
#include "Messages.h"
#include "Macros.h"

typedef struct _IntList IntList;

struct _IntList
{
	int value;
	List entry;
};

void int_list_append(List *list, int value)
{
	IntList *node = talloc(IntList, 1);
	return_if_fail(node != NULL);

	node->value = value;
	list_node_init(&node->entry);
	list_append(list, &node->entry);
}

int int_list_cmp(const void *a, const void *b)
{
	const IntList *ia = list_entry(a, IntList, entry);
	const IntList *ib = list_entry(b, IntList, entry);

	return ia->value - ib->value;
}

int main(int argc, char *argv[])
{
	srand(time(0));

	list_init(list);

	for (int i = 0; i < 10; ++i)
	{
		int_list_append(&list, rand() % 100);
	}

	List *iter;

	list_foreach(iter, &list)
	{
		printf("%d ", list_entry(iter, IntList, entry)->value);
	}

	list_sort(&list, int_list_cmp);
	printf("\n");

	list_foreach(iter, &list)
	{
		printf("%d ", list_entry(iter, IntList, entry)->value);
	}

	list_reverse(&list);
	printf("\n");

	list_foreach(iter, &list)
	{
		printf("%d ", list_entry(iter, IntList, entry)->value);
	}
}
