#include "SList.h"
#include "Macros.h"
#include "Messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _IntSList IntSList;

struct _IntSList {
	int value;
	SList entry;
};

void int_slist_append(SList *list, int value)
{
	IntSList *node = talloc(IntSList, 1);
	return_if_fail(node != NULL);

	node->value = value;
	slist_node_init(&node->entry);
	slist_push_back(list, &node->entry);
}

int int_slist_cmp(const void *a, const void *b)
{
	const IntSList *ia = slist_entry(a, IntSList, entry);
	const IntSList *ib = slist_entry(b, IntSList, entry);

	return ia->value - ib->value;
}

int main(int argc, char *argv[])
{
	srand(time(0));

	slist_init(list);

	for (int i = 0; i < 10; ++i) {
		int_slist_append(&list, rand() % 100);
	}

	SList *iter;

	slist_foreach(iter, &list)
	{
		printf("%d ", slist_entry(iter, IntSList, entry)->value);
	}

	slist_reverse(&list);
	printf("\n");

	slist_foreach(iter, &list)
	{
		printf("%d ", slist_entry(iter, IntSList, entry)->value);
	}
}
