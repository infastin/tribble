#include "List.h"
#include "Macros.h"
#include "Messages.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _IntList IntList;

struct _IntList {
	int value;
	TrbList entry;
};

TrbList *int_trb_list_new_node(int value)
{
	IntList *node = trb_talloc(IntList, 1);
	trb_return_val_if_fail(node != NULL, NULL);

	node->value = value;
	trb_list_node_init(&node->entry);

	return &node->entry;
}

void int_trb_list_push_back(TrbList *list, int value)
{
	IntList *node = trb_talloc(IntList, 1);
	trb_return_if_fail(node != NULL);

	node->value = value;
	trb_list_node_init(&node->entry);
	trb_list_push_back(list, &node->entry);
}

void int_trb_list_push_front(TrbList *list, int value)
{
	IntList *node = trb_talloc(IntList, 1);
	trb_return_if_fail(node != NULL);

	node->value = value;
	trb_list_node_init(&node->entry);
	trb_list_push_front(list, &node->entry);
}

int int_trb_list_cmp(const void *a, const void *b)
{
	const IntList *ia = trb_list_entry(a, IntList, entry);
	const IntList *ib = trb_list_entry(b, IntList, entry);

	return ia->value - ib->value;
}

void int_trb_list_free(void *ptr)
{
	IntList *node = trb_list_entry(ptr, IntList, entry);
	free(node);
}

void *int_trb_list_copy(const void *ptr)
{
	IntList *node = trb_list_entry(ptr, IntList, entry);
	IntList *copy = trb_talloc(IntList, 1);

	copy->value = node->value;
	trb_list_node_init(&copy->entry);

	return &copy->entry;
}

void test_push_destroy()
{
	TrbList list1;
	trb_list_init(&list1);

	TrbList list2;
	trb_list_init(&list2);

	TrbList *node1 = int_trb_list_new_node(10);
	TrbList *node2 = int_trb_list_new_node(20);
	TrbList *node3 = int_trb_list_new_node(30);
	TrbList *node4 = int_trb_list_new_node(40);

	int_trb_list_push_back(&list1, 10);
	int_trb_list_push_front(&list1, 20);
	int_trb_list_push_back(&list1, 30);
	int_trb_list_push_front(&list1, 40);

	node4->next = node2;
	node4->prev = &list2;
	node2->prev = node4;
	node2->next = node1;
	node1->prev = node2;
	node1->next = node3;
	node3->prev = node1;
	node3->next = &list2;

	list2.next = node4;
	list2.prev = node3;

	TrbList *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = trb_list_entry(iter1, IntList, entry);
		IntList *ii2 = trb_list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	trb_list_destroy(&list1, int_trb_list_free);
	trb_list_destroy(&list2, int_trb_list_free);

	assert(list1.next == &list1 && list1.prev == &list1);
	assert(list2.next == &list2 && list2.prev == &list2);
}

void test_reverse()
{
	TrbList list1;
	trb_list_init(&list1);

	TrbList list2;
	trb_list_init(&list2);

	int_trb_list_push_back(&list1, 10);
	int_trb_list_push_front(&list1, 20);
	int_trb_list_push_back(&list1, 30);
	int_trb_list_push_front(&list1, 40);

	int_trb_list_push_front(&list2, 10);
	int_trb_list_push_back(&list2, 20);
	int_trb_list_push_front(&list2, 30);
	int_trb_list_push_back(&list2, 40);

	trb_list_reverse(&list2);

	TrbList *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = trb_list_entry(iter1, IntList, entry);
		IntList *ii2 = trb_list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	trb_list_reverse(&list2);
	trb_list_reverse(&list1);

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = trb_list_entry(iter1, IntList, entry);
		IntList *ii2 = trb_list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	trb_list_destroy(&list1, int_trb_list_free);
	trb_list_destroy(&list2, int_trb_list_free);
}

void test_sort()
{
	TrbList list1;
	trb_list_init(&list1);

	TrbList list2;
	trb_list_init(&list2);

	int_trb_list_push_back(&list1, 30);
	int_trb_list_push_back(&list1, 1);
	int_trb_list_push_back(&list1, 20);
	int_trb_list_push_back(&list1, 333);
	int_trb_list_push_back(&list1, 99);
	int_trb_list_push_back(&list1, 2);

	int_trb_list_push_back(&list2, 1);
	int_trb_list_push_back(&list2, 2);
	int_trb_list_push_back(&list2, 20);
	int_trb_list_push_back(&list2, 30);
	int_trb_list_push_back(&list2, 99);
	int_trb_list_push_back(&list2, 333);

	trb_list_sort(&list1, int_trb_list_cmp);

	TrbList *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = trb_list_entry(iter1, IntList, entry);
		IntList *ii2 = trb_list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	trb_list_destroy(&list1, int_trb_list_free);
	trb_list_destroy(&list2, int_trb_list_free);
}

void test_copy()
{
	TrbList list1;
	trb_list_init(&list1);

	TrbList list2;
	trb_list_init(&list2);

	int_trb_list_push_back(&list1, 1);
	int_trb_list_push_back(&list1, 2);
	int_trb_list_push_back(&list1, 20);
	int_trb_list_push_back(&list1, 30);
	int_trb_list_push_back(&list1, 99);
	int_trb_list_push_back(&list1, 333);

	trb_list_copy(&list2, &list1, int_trb_list_copy, NULL);

	TrbList *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = trb_list_entry(iter1, IntList, entry);
		IntList *ii2 = trb_list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	trb_list_destroy(&list1, int_trb_list_free);
	trb_list_destroy(&list2, int_trb_list_free);
}

void test_len_empty()
{
	TrbList list;
	trb_list_init(&list);

	int_trb_list_push_back(&list, 1);
	int_trb_list_push_back(&list, 2);
	int_trb_list_push_back(&list, 20);
	int_trb_list_push_back(&list, 30);
	int_trb_list_push_back(&list, 99);
	int_trb_list_push_back(&list, 333);

	assert(trb_list_len(&list) == 6);

	trb_list_destroy(&list, int_trb_list_free);

	assert(trb_list_empty(&list));
}

void test_nth_position()
{
	TrbList list;
	trb_list_init(&list);

	TrbList *node1 = int_trb_list_new_node(1);
	TrbList *node2 = int_trb_list_new_node(2);

	int_trb_list_push_back(&list, 10);
	int_trb_list_push_back(&list, 20);
	trb_list_push_back(&list, node1);
	int_trb_list_push_back(&list, 33);
	int_trb_list_push_back(&list, 9);
	trb_list_push_back(&list, node2);
	int_trb_list_push_back(&list, 11);

	assert(trb_list_nth(&list, 2) == node1);
	assert(trb_list_nth(&list, 5) == node2);

	assert(trb_list_position(&list, node1) == 2);
	assert(trb_list_position(&list, node2) == 5);

	trb_list_destroy(&list, int_trb_list_free);
}

void test_insert()
{
	TrbList list1;
	trb_list_init(&list1);

	TrbList list2;
	trb_list_init(&list2);

	TrbList *node1 = int_trb_list_new_node(1);
	TrbList *node2 = int_trb_list_new_node(2);
	TrbList *node3 = int_trb_list_new_node(20);
	TrbList *node4 = int_trb_list_new_node(30);

	trb_list_push_back(&list1, node1);
	trb_list_insert_after(node1, node2);
	int_trb_list_push_back(&list1, 99);
	int_trb_list_push_back(&list1, 333);
	trb_list_push_back(&list1, node3);
	trb_list_insert_before(node3, node4);

	int_trb_list_push_back(&list2, 1);
	int_trb_list_push_back(&list2, 2);
	int_trb_list_push_back(&list2, 99);
	int_trb_list_push_back(&list2, 333);
	int_trb_list_push_back(&list2, 20);
	int_trb_list_push_back(&list2, 30);

	TrbList *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = trb_list_entry(iter1, IntList, entry);
		IntList *ii2 = trb_list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	trb_list_destroy(&list1, int_trb_list_free);
	trb_list_destroy(&list2, int_trb_list_free);
}

void test_remove_pop()
{
	TrbList list1;
	trb_list_init(&list1);

	TrbList list2;
	trb_list_init(&list2);

	TrbList *node1 = int_trb_list_new_node(1);
	TrbList *node2 = int_trb_list_new_node(2);
	TrbList *node3 = int_trb_list_new_node(20);
	TrbList *node4 = int_trb_list_new_node(30);

	trb_list_push_back(&list1, node1);
	trb_list_push_back(&list1, node2);
	int_trb_list_push_back(&list1, 99);
	int_trb_list_push_back(&list1, 333);
	trb_list_push_back(&list1, node3);
	trb_list_push_back(&list1, node4);

	int_trb_list_push_back(&list2, 99);
	int_trb_list_push_back(&list2, 333);

	trb_list_remove(node3);
	trb_list_remove(node2);
	trb_list_pop_front(&list1);
	trb_list_pop_back(&list1);

	TrbList *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = trb_list_entry(iter1, IntList, entry);
		IntList *ii2 = trb_list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	int_trb_list_free(node1);
	int_trb_list_free(node2);
	int_trb_list_free(node3);
	int_trb_list_free(node4);

	trb_list_destroy(&list1, int_trb_list_free);
	trb_list_destroy(&list2, int_trb_list_free);
}

void test_splice()
{
	TrbList list1;
	trb_list_init(&list1);

	TrbList list2;
	trb_list_init(&list2);

	TrbList list3;
	trb_list_init(&list3);

	int_trb_list_push_back(&list1, 1);
	int_trb_list_push_back(&list1, 2);
	int_trb_list_push_back(&list1, 30);
	int_trb_list_push_back(&list1, 11);
	int_trb_list_push_back(&list1, 44);
	int_trb_list_push_back(&list1, 99);
	int_trb_list_push_back(&list1, 3);

	TrbList *node = int_trb_list_new_node(22);

	int_trb_list_push_back(&list2, 7);
	int_trb_list_push_back(&list2, 8);
	int_trb_list_push_back(&list2, 757);
	trb_list_push_back(&list2, node);
	int_trb_list_push_back(&list2, 66);
	int_trb_list_push_back(&list2, 159);
	int_trb_list_push_back(&list2, 78);

	int_trb_list_push_back(&list3, 7);
	int_trb_list_push_back(&list3, 8);
	int_trb_list_push_back(&list3, 757);
	int_trb_list_push_back(&list3, 22);
	int_trb_list_push_back(&list3, 1);
	int_trb_list_push_back(&list3, 2);
	int_trb_list_push_back(&list3, 30);
	int_trb_list_push_back(&list3, 11);
	int_trb_list_push_back(&list3, 44);
	int_trb_list_push_back(&list3, 99);
	int_trb_list_push_back(&list3, 3);
	int_trb_list_push_back(&list3, 66);
	int_trb_list_push_back(&list3, 159);
	int_trb_list_push_back(&list3, 78);

	trb_list_splice(&list1, node);

	assert(list1.next == &list1);

	TrbList *iter2, *iter3;

	for (
		iter2 = list2.next, iter3 = list3.next;
		iter2 != &list2 && iter3 != &list3;
		iter2 = iter2->next, iter3 = iter3->next
	) {
		IntList *ii2 = trb_list_entry(iter2, IntList, entry);
		IntList *ii3 = trb_list_entry(iter3, IntList, entry);

		assert(ii2->value == ii3->value);
	}

	trb_list_destroy(&list2, int_trb_list_free);
	trb_list_destroy(&list3, int_trb_list_free);
}

int main(int argc, char *argv[])
{
	test_push_destroy();
	test_reverse();
	test_sort();
	test_copy();
	test_len_empty();
	test_nth_position();
	test_insert();
	test_remove_pop();
	test_splice();
}
