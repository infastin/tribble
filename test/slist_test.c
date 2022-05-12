#include "Macros.h"
#include "Messages.h"
#include "SList.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _IntSList IntSList;

struct _IntSList {
	int value;
	SList entry;
};

SList *int_slist_new_node(int value)
{
	IntSList *node = talloc(IntSList, 1);
	return_val_if_fail(node != NULL, NULL);

	node->value = value;
	slist_node_init(&node->entry);

	return &node->entry;
}

void int_slist_push_back(SList *list, int value)
{
	IntSList *node = talloc(IntSList, 1);
	return_if_fail(node != NULL);

	node->value = value;
	slist_node_init(&node->entry);
	slist_push_back(list, &node->entry);
}

void int_slist_push_front(SList *list, int value)
{
	IntSList *node = talloc(IntSList, 1);
	return_if_fail(node != NULL);

	node->value = value;
	slist_node_init(&node->entry);
	slist_push_front(list, &node->entry);
}

int int_slist_cmp(const void *a, const void *b)
{
	const IntSList *ia = slist_entry(a, IntSList, entry);
	const IntSList *ib = slist_entry(b, IntSList, entry);

	return ia->value - ib->value;
}

void int_slist_free(void *ptr)
{
	IntSList *node = slist_entry(ptr, IntSList, entry);
	free(node);
}

void *int_slist_copy(const void *ptr)
{
	IntSList *node = slist_entry(ptr, IntSList, entry);
	IntSList *copy = talloc(IntSList, 1);

	copy->value = node->value;
	slist_node_init(&copy->entry);

	return &copy->entry;
}

void test_push_destroy()
{
	SList list1;
	slist_init(&list1);

	SList list2;
	slist_init(&list2);

	SList *node1 = int_slist_new_node(10);
	SList *node2 = int_slist_new_node(20);
	SList *node3 = int_slist_new_node(30);
	SList *node4 = int_slist_new_node(40);

	int_slist_push_back(&list1, 10);
	int_slist_push_front(&list1, 20);
	int_slist_push_back(&list1, 30);
	int_slist_push_front(&list1, 40);

	node4->next = node2;
	node2->next = node1;
	node1->next = node3;
	node3->next = &list2;

	list2.next = node4;

	SList *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntSList *ii1 = slist_entry(iter1, IntSList, entry);
		IntSList *ii2 = slist_entry(iter1, IntSList, entry);

		assert(ii1->value == ii2->value);
	}

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntSList *ii1 = slist_entry(iter1, IntSList, entry);
		IntSList *ii2 = slist_entry(iter1, IntSList, entry);

		assert(ii1->value == ii2->value);
	}

	slist_destroy(&list1, int_slist_free);
	slist_destroy(&list2, int_slist_free);

	assert(list1.next == &list1);
	assert(list2.next == &list2);
}

void test_reverse()
{
	SList list1;
	slist_init(&list1);

	SList list2;
	slist_init(&list2);

	int_slist_push_back(&list1, 10);
	int_slist_push_front(&list1, 20);
	int_slist_push_back(&list1, 30);
	int_slist_push_front(&list1, 40);

	int_slist_push_front(&list2, 10);
	int_slist_push_back(&list2, 20);
	int_slist_push_front(&list2, 30);
	int_slist_push_back(&list2, 40);

	slist_reverse(&list2);

	SList *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntSList *ii1 = slist_entry(iter1, IntSList, entry);
		IntSList *ii2 = slist_entry(iter1, IntSList, entry);

		assert(ii1->value == ii2->value);
	}

	slist_reverse(&list2);
	slist_reverse(&list1);

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntSList *ii1 = slist_entry(iter1, IntSList, entry);
		IntSList *ii2 = slist_entry(iter1, IntSList, entry);

		assert(ii1->value == ii2->value);
	}

	slist_destroy(&list1, int_slist_free);
	slist_destroy(&list2, int_slist_free);
}

void test_sort()
{
	SList list1;
	slist_init(&list1);

	SList list2;
	slist_init(&list2);

	int_slist_push_front(&list1, 30);
	int_slist_push_front(&list1, 1);
	int_slist_push_front(&list1, 20);
	int_slist_push_front(&list1, 333);
	int_slist_push_front(&list1, 99);
	int_slist_push_front(&list1, 2);

	int_slist_push_front(&list2, 1);
	int_slist_push_front(&list2, 2);
	int_slist_push_front(&list2, 20);
	int_slist_push_front(&list2, 30);
	int_slist_push_front(&list2, 99);
	int_slist_push_front(&list2, 333);

	slist_reverse(&list2);
	slist_sort(&list1, int_slist_cmp);

	SList *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntSList *ii1 = slist_entry(iter1, IntSList, entry);
		IntSList *ii2 = slist_entry(iter1, IntSList, entry);

		assert(ii1->value == ii2->value);
	}

	slist_destroy(&list1, int_slist_free);
	slist_destroy(&list2, int_slist_free);
}

void test_copy()
{
	SList list1;
	slist_init(&list1);

	SList list2;
	slist_init(&list2);

	int_slist_push_front(&list1, 1);
	int_slist_push_front(&list1, 2);
	int_slist_push_front(&list1, 20);
	int_slist_push_front(&list1, 30);
	int_slist_push_front(&list1, 99);
	int_slist_push_front(&list1, 333);

	slist_copy(&list2, &list1, int_slist_copy, NULL);

	SList *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntSList *ii1 = slist_entry(iter1, IntSList, entry);
		IntSList *ii2 = slist_entry(iter1, IntSList, entry);

		assert(ii1->value == ii2->value);
	}

	slist_destroy(&list1, int_slist_free);
	slist_destroy(&list2, int_slist_free);
}

void test_len_empty()
{
	SList list;
	slist_init(&list);

	int_slist_push_front(&list, 1);
	int_slist_push_front(&list, 2);
	int_slist_push_front(&list, 20);
	int_slist_push_front(&list, 30);
	int_slist_push_front(&list, 99);
	int_slist_push_front(&list, 333);

	assert(slist_len(&list) == 6);

	slist_destroy(&list, int_slist_free);

	assert(slist_empty(&list));
}

void test_nth_position()
{
	SList list;
	slist_init(&list);

	SList *node1 = int_slist_new_node(1);
	SList *node2 = int_slist_new_node(2);

	int_slist_push_front(&list, 10);
	int_slist_push_front(&list, 20);
	slist_push_front(&list, node1);
	int_slist_push_front(&list, 33);
	int_slist_push_front(&list, 9);
	slist_push_front(&list, node2);
	int_slist_push_front(&list, 11);

	assert(slist_nth(&list, 4) == node1);
	assert(slist_nth(&list, 1) == node2);

	assert(slist_position(&list, node1) == 4);
	assert(slist_position(&list, node2) == 1);

	slist_destroy(&list, int_slist_free);
}

void test_insert()
{
	SList list1;
	slist_init(&list1);

	SList list2;
	slist_init(&list2);

	SList *node1 = int_slist_new_node(1);
	SList *node2 = int_slist_new_node(2);
	SList *node3 = int_slist_new_node(20);
	SList *node4 = int_slist_new_node(30);

	slist_push_front(&list1, node1);
	slist_insert_after(node1, node2);
	int_slist_push_front(&list1, 99);
	int_slist_push_front(&list1, 333);
	slist_push_front(&list1, node3);
	slist_insert_before(&list1, node3, node4);

	int_slist_push_front(&list2, 2);
	int_slist_push_front(&list2, 1);
	int_slist_push_front(&list2, 99);
	int_slist_push_front(&list2, 333);
	int_slist_push_front(&list2, 20);
	int_slist_push_front(&list2, 30);

	SList *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntSList *ii1 = slist_entry(iter1, IntSList, entry);
		IntSList *ii2 = slist_entry(iter1, IntSList, entry);

		assert(ii1->value == ii2->value);
	}

	slist_destroy(&list1, int_slist_free);
	slist_destroy(&list2, int_slist_free);
}

void test_remove_pop()
{
	SList list1;
	slist_init(&list1);

	SList list2;
	slist_init(&list2);

	SList *node1 = int_slist_new_node(1);
	SList *node2 = int_slist_new_node(2);
	SList *node3 = int_slist_new_node(20);
	SList *node4 = int_slist_new_node(30);

	slist_push_front(&list1, node1);
	slist_push_front(&list1, node2);
	int_slist_push_front(&list1, 99);
	int_slist_push_front(&list1, 333);
	slist_push_front(&list1, node3);
	slist_push_front(&list1, node4);

	int_slist_push_front(&list2, 99);
	int_slist_push_front(&list2, 333);

	slist_remove(&list1, node3);
	slist_remove(&list1, node2);
	slist_pop_front(&list1);
	slist_pop_back(&list1);

	SList *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntSList *ii1 = slist_entry(iter1, IntSList, entry);
		IntSList *ii2 = slist_entry(iter1, IntSList, entry);

		assert(ii1->value == ii2->value);
	}

	int_slist_free(node1);
	int_slist_free(node2);
	int_slist_free(node3);
	int_slist_free(node4);

	slist_destroy(&list1, int_slist_free);
	slist_destroy(&list2, int_slist_free);
}

void test_splice()
{
	SList list1;
	slist_init(&list1);

	SList list2;
	slist_init(&list2);

	SList list3;
	slist_init(&list3);

	int_slist_push_front(&list1, 1);
	int_slist_push_front(&list1, 2);
	int_slist_push_front(&list1, 30);
	int_slist_push_front(&list1, 11);
	int_slist_push_front(&list1, 44);
	int_slist_push_front(&list1, 99);
	int_slist_push_front(&list1, 3);

	SList *node = int_slist_new_node(22);

	int_slist_push_front(&list2, 7);
	int_slist_push_front(&list2, 8);
	int_slist_push_front(&list2, 757);
	slist_push_front(&list2, node);
	int_slist_push_front(&list2, 66);
	int_slist_push_front(&list2, 159);
	int_slist_push_front(&list2, 78);

	int_slist_push_front(&list3, 7);
	int_slist_push_front(&list3, 8);
	int_slist_push_front(&list3, 757);
	int_slist_push_front(&list3, 1);
	int_slist_push_front(&list3, 2);
	int_slist_push_front(&list3, 30);
	int_slist_push_front(&list3, 11);
	int_slist_push_front(&list3, 44);
	int_slist_push_front(&list3, 99);
	int_slist_push_front(&list3, 3);
	int_slist_push_front(&list3, 22);
	int_slist_push_front(&list3, 66);
	int_slist_push_front(&list3, 159);
	int_slist_push_front(&list3, 78);

	slist_splice(&list1, node);

	assert(list1.next == &list1);

	SList *iter2, *iter3;

	for (
		iter2 = list2.next, iter3 = list3.next;
		iter2 != &list2 && iter3 != &list3;
		iter2 = iter2->next, iter3 = iter3->next
	) {
		IntSList *ii2 = slist_entry(iter2, IntSList, entry);
		IntSList *ii3 = slist_entry(iter3, IntSList, entry);

		assert(ii2->value == ii3->value);
	}

	slist_destroy(&list2, int_slist_free);
	slist_destroy(&list3, int_slist_free);
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
