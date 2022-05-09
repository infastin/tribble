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
	List entry;
};

List *int_list_new_node(int value)
{
	IntList *node = talloc(IntList, 1);
	return_val_if_fail(node != NULL, NULL);

	node->value = value;
	list_node_init(&node->entry);

	return &node->entry;
}

void int_list_push_back(List *list, int value)
{
	IntList *node = talloc(IntList, 1);
	return_if_fail(node != NULL);

	node->value = value;
	list_node_init(&node->entry);
	list_push_back(list, &node->entry);
}

void int_list_push_front(List *list, int value)
{
	IntList *node = talloc(IntList, 1);
	return_if_fail(node != NULL);

	node->value = value;
	list_node_init(&node->entry);
	list_push_front(list, &node->entry);
}

int int_list_cmp(const void *a, const void *b)
{
	const IntList *ia = list_entry(a, IntList, entry);
	const IntList *ib = list_entry(b, IntList, entry);

	return ia->value - ib->value;
}

void int_list_free(void *ptr)
{
	IntList *node = list_entry(ptr, IntList, entry);
	free(node);
}

void *int_list_copy(const void *ptr)
{
	IntList *node = list_entry(ptr, IntList, entry);
	IntList *copy = talloc(IntList, 1);

	copy->value = node->value;
	list_node_init(&copy->entry);

	return &copy->entry;
}

void test_push_purge()
{
	List list1;
	list_init(&list1);

	List list2;
	list_init(&list2);

	List *node1 = int_list_new_node(10);
	List *node2 = int_list_new_node(20);
	List *node3 = int_list_new_node(30);
	List *node4 = int_list_new_node(40);

	int_list_push_back(&list1, 10);
	int_list_push_front(&list1, 20);
	int_list_push_back(&list1, 30);
	int_list_push_front(&list1, 40);

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

	List *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = list_entry(iter1, IntList, entry);
		IntList *ii2 = list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	list_purge(&list1, int_list_free);
	list_purge(&list2, int_list_free);

	assert(list1.next == &list1 && list1.prev == &list1);
	assert(list2.next == &list2 && list2.prev == &list2);
}

void test_reverse()
{
	List list1;
	list_init(&list1);

	List list2;
	list_init(&list2);

	int_list_push_back(&list1, 10);
	int_list_push_front(&list1, 20);
	int_list_push_back(&list1, 30);
	int_list_push_front(&list1, 40);

	int_list_push_front(&list2, 10);
	int_list_push_back(&list2, 20);
	int_list_push_front(&list2, 30);
	int_list_push_back(&list2, 40);

	list_reverse(&list2);

	List *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = list_entry(iter1, IntList, entry);
		IntList *ii2 = list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	list_reverse(&list2);
	list_reverse(&list1);

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = list_entry(iter1, IntList, entry);
		IntList *ii2 = list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	list_purge(&list1, int_list_free);
	list_purge(&list2, int_list_free);
}

void test_sort()
{
	List list1;
	list_init(&list1);

	List list2;
	list_init(&list2);

	int_list_push_back(&list1, 30);
	int_list_push_back(&list1, 1);
	int_list_push_back(&list1, 20);
	int_list_push_back(&list1, 333);
	int_list_push_back(&list1, 99);
	int_list_push_back(&list1, 2);

	int_list_push_back(&list2, 1);
	int_list_push_back(&list2, 2);
	int_list_push_back(&list2, 20);
	int_list_push_back(&list2, 30);
	int_list_push_back(&list2, 99);
	int_list_push_back(&list2, 333);

	list_sort(&list1, int_list_cmp);

	List *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = list_entry(iter1, IntList, entry);
		IntList *ii2 = list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	list_purge(&list1, int_list_free);
	list_purge(&list2, int_list_free);
}

void test_copy()
{
	List list1;
	list_init(&list1);

	List list2;
	list_init(&list2);

	int_list_push_back(&list1, 1);
	int_list_push_back(&list1, 2);
	int_list_push_back(&list1, 20);
	int_list_push_back(&list1, 30);
	int_list_push_back(&list1, 99);
	int_list_push_back(&list1, 333);

	list_copy(&list2, &list1, int_list_copy, NULL);

	List *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = list_entry(iter1, IntList, entry);
		IntList *ii2 = list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	list_purge(&list1, int_list_free);
	list_purge(&list2, int_list_free);
}

void test_len_empty()
{
	List list;
	list_init(&list);

	int_list_push_back(&list, 1);
	int_list_push_back(&list, 2);
	int_list_push_back(&list, 20);
	int_list_push_back(&list, 30);
	int_list_push_back(&list, 99);
	int_list_push_back(&list, 333);

	assert(list_len(&list) == 6);

	list_purge(&list, int_list_free);

	assert(list_empty(&list));
}

void test_nth_position()
{
	List list;
	list_init(&list);

	List *node1 = int_list_new_node(1);
	List *node2 = int_list_new_node(2);

	int_list_push_back(&list, 10);
	int_list_push_back(&list, 20);
	list_push_back(&list, node1);
	int_list_push_back(&list, 33);
	int_list_push_back(&list, 9);
	list_push_back(&list, node2);
	int_list_push_back(&list, 11);

	assert(list_nth(&list, 2) == node1);
	assert(list_nth(&list, 5) == node2);

	assert(list_position(&list, node1) == 2);
	assert(list_position(&list, node2) == 5);

	list_purge(&list, int_list_free);
}

void test_insert()
{
	List list1;
	list_init(&list1);

	List list2;
	list_init(&list2);

	List *node1 = int_list_new_node(1);
	List *node2 = int_list_new_node(2);
	List *node3 = int_list_new_node(20);
	List *node4 = int_list_new_node(30);

	list_push_back(&list1, node1);
	list_insert_after(node1, node2);
	int_list_push_back(&list1, 99);
	int_list_push_back(&list1, 333);
	list_push_back(&list1, node3);
	list_insert_before(node3, node4);

	int_list_push_back(&list2, 1);
	int_list_push_back(&list2, 2);
	int_list_push_back(&list2, 99);
	int_list_push_back(&list2, 333);
	int_list_push_back(&list2, 20);
	int_list_push_back(&list2, 30);

	List *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = list_entry(iter1, IntList, entry);
		IntList *ii2 = list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	list_purge(&list1, int_list_free);
	list_purge(&list2, int_list_free);
}

void test_remove_pop()
{
	List list1;
	list_init(&list1);

	List list2;
	list_init(&list2);

	List *node1 = int_list_new_node(1);
	List *node2 = int_list_new_node(2);
	List *node3 = int_list_new_node(20);
	List *node4 = int_list_new_node(30);

	list_push_back(&list1, node1);
	list_push_back(&list1, node2);
	int_list_push_back(&list1, 99);
	int_list_push_back(&list1, 333);
	list_push_back(&list1, node3);
	list_push_back(&list1, node4);

	int_list_push_back(&list2, 99);
	int_list_push_back(&list2, 333);

	list_remove(node3);
	list_remove(node2);
	list_pop_front(&list1);
	list_pop_back(&list1);

	List *iter1, *iter2;

	for (
		iter1 = list1.next, iter2 = list2.next;
		iter1 != &list1 && iter2 != &list2;
		iter1 = iter1->next, iter2 = iter2->next
	) {
		IntList *ii1 = list_entry(iter1, IntList, entry);
		IntList *ii2 = list_entry(iter1, IntList, entry);

		assert(ii1->value == ii2->value);
	}

	int_list_free(node1);
	int_list_free(node2);
	int_list_free(node3);
	int_list_free(node4);

	list_purge(&list1, int_list_free);
	list_purge(&list2, int_list_free);
}

void test_splice()
{
	List list1;
	list_init(&list1);

	List list2;
	list_init(&list2);

	List list3;
	list_init(&list3);

	int_list_push_back(&list1, 1);
	int_list_push_back(&list1, 2);
	int_list_push_back(&list1, 30);
	int_list_push_back(&list1, 11);
	int_list_push_back(&list1, 44);
	int_list_push_back(&list1, 99);
	int_list_push_back(&list1, 3);

	List *node = int_list_new_node(22);

	int_list_push_back(&list2, 7);
	int_list_push_back(&list2, 8);
	int_list_push_back(&list2, 757);
	list_push_back(&list2, node);
	int_list_push_back(&list2, 66);
	int_list_push_back(&list2, 159);
	int_list_push_back(&list2, 78);

	int_list_push_back(&list3, 7);
	int_list_push_back(&list3, 8);
	int_list_push_back(&list3, 757);
	int_list_push_back(&list3, 22);
	int_list_push_back(&list3, 1);
	int_list_push_back(&list3, 2);
	int_list_push_back(&list3, 30);
	int_list_push_back(&list3, 11);
	int_list_push_back(&list3, 44);
	int_list_push_back(&list3, 99);
	int_list_push_back(&list3, 3);
	int_list_push_back(&list3, 66);
	int_list_push_back(&list3, 159);
	int_list_push_back(&list3, 78);

	list_splice(&list1, node);

	assert(list1.next == &list1);

	List *iter2, *iter3;

	for (
		iter2 = list2.next, iter3 = list3.next;
		iter2 != &list2 && iter3 != &list3;
		iter2 = iter2->next, iter3 = iter3->next
	) {
		IntList *ii2 = list_entry(iter2, IntList, entry);
		IntList *ii3 = list_entry(iter3, IntList, entry);

		assert(ii2->value == ii3->value);
	}

	list_purge(&list2, int_list_free);
	list_purge(&list3, int_list_free);
}

int main(int argc, char *argv[])
{
	test_push_purge();
	test_reverse();
	test_sort();
	test_copy();
	test_len_empty();
	test_nth_position();
	test_insert();
	test_remove_pop();
	test_splice();
}
