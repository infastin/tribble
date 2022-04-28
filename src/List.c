#include "List.h"

#include "Messages.h"

#include <stddef.h>

void list_append(List *list, List *node)
{
	return_if_fail(list != NULL);
	return_if_fail(node != NULL);

	List *end = list->prev;

	end->next = node;
	node->prev = end;
	node->next = list;
	list->prev = node;
}

void list_prepend(List *list, List *node)
{
	return_if_fail(list != NULL);
	return_if_fail(node != NULL);

	List *start = list->next;

	start->prev = node;
	node->next = start;
	node->prev = list;
	list->next = node;
}

void list_insert_after(List *sibling, List *node)
{
	return_if_fail(sibling != NULL);
	return_if_fail(node != NULL);

	node->prev = sibling;
	node->next = sibling->next;
	sibling->next->prev = node;
	sibling->next = node;
}

void list_insert_before(List *sibling, List *node)
{
	return_if_fail(sibling != NULL);
	return_if_fail(node != NULL);

	node->next = sibling;
	node->prev = sibling->prev;
	sibling->prev->next = node;
	sibling->prev = node;
}

bool list_empty(const List *list)
{
	return_val_if_fail(list != NULL, FALSE);

	if (list->next == list)
		return TRUE;

	return FALSE;
}

List *list_pop(List *list)
{
	return_val_if_fail(list != NULL, NULL);

	if (list->next == list)
		return NULL;

	List *last = list->prev;
	list_remove(last);
	return last;
}

void list_reverse(List *list)
{
	return_if_fail(list != NULL);

	if (list->next == list)
		return;

	List *current = list->next;

	while (current != list) {
		List *tmp = current->next;
		current->next = current->prev;
		current->prev = tmp;
		current = tmp;
	}

	List *oldstart = list->next;
	list->next = list->prev;
	list->prev = oldstart;
}

void list_splice(List *list, List *node)
{
	return_if_fail(list != NULL);
	return_if_fail(node != NULL);

	if (list->next == list)
		return;

	List *start = list->next;
	List *end = list->prev;
	List *at = node->next;

	start->prev = node;
	node->next = start;

	end->next = at;
	at->prev = end;
}

static List *__list_merge(List *list, List *first, List *second, List **end, CmpFunc cmp_func)
{
	List *result = NULL;
	List *prev = list;
	List **linkp = &result;

	List *f = first;
	List *s = second;

	while (1) {
		if (f == list && s == list)
			break;

		if (f == list || f == NULL) {
			s->prev = prev;
			*linkp = s;

			if (end != NULL)
				*end = s;

			break;
		}

		if (s == list) {
			f->prev = prev;
			*linkp = f;

			if (end != NULL)
				*end = f;

			break;
		}

		if (cmp_func(f, s) <= 0) {
			f->prev = prev;
			prev = *linkp = f;
			linkp = &f->next;
			f = f->next;
		} else {
			s->prev = prev;
			prev = *linkp = s;
			linkp = &s->next;
			s = s->next;
		}
	}

	return result;
}

void list_sort(List *list, CmpFunc cmp_func)
{
	return_if_fail(list != NULL);
	return_if_fail(cmp_func != NULL);

	if (list->next == list)
		return;

	List *array[32] = { 0 };
	List *result;
	int max_i = 0;
	int i;

	result = list->next;

	while (result != list) {
		List *next;

		next = result->next;
		result->next = list;

		for (i = 0; (i < 32) && (array[i] != NULL); ++i) {
			result = __list_merge(list, array[i], result, NULL, cmp_func);
			array[i] = NULL;
		}

		if (i == 32)
			i--;

		if (i > max_i)
			max_i = i;

		array[i] = result;
		result = next;
	}

	List *end;

	for (int j = 0; j < max_i + 1; ++j)
		result = __list_merge(list, array[j], result, &end, cmp_func);

	list->next = result;

	if (end->next != list)
		for (; end->next != list; end = end->next)
			;

	list->prev = end;
}

void list_remove(List *node)
{
	return_if_fail(node != NULL);

	node->prev->next = node->next;
	node->next->prev = node->prev;

	node->next = node;
	node->prev = node;
}

static void __list_swap_case1(List *a, List *b)
{
	List *old_a_prev = a->prev;
	List *old_b_next = b->next;

	a->prev = b;
	a->next = old_b_next;

	b->next = a;
	b->prev = old_a_prev;

	old_a_prev->next = b;
	old_b_next->prev = a;
}

static void __list_swap_case2(List *a, List *b)
{
	List *old_a_prev = a->prev;
	List *old_a_next = a->next;

	List *old_b_prev = b->prev;
	List *old_b_next = b->next;

	old_a_prev->next = b;
	old_a_next->prev = b;

	old_b_prev->next = a;
	old_b_next->prev = a;

	a->prev = old_b_prev;
	a->next = old_b_next;

	b->prev = old_a_prev;
	b->next = old_a_next;
}

void list_swap(List *a, List *b)
{
	return_if_fail(a != NULL);
	return_if_fail(b != NULL);

	if (a == b)
		return;

	if (a->next == b)
		__list_swap_case1(a, b);
	else if (a->prev == b)
		__list_swap_case1(b, a);
	else
		__list_swap_case2(a, b);
}

int list_copy(List *dst, const List *src, CopyFunc copy_func)
{
	return_val_if_fail(dst != NULL, -1);
	return_val_if_fail(src != NULL, -1);
	return_val_if_fail(copy_func != NULL, -1);

	if (src->next == src)
		return 0;

	List *fast = src->next;
	List *slow = dst;
	List *copy = NULL;

	while (fast != src) {
		copy = copy_func(fast);

		if (copy == NULL)
			break;

		slow->next = copy;
		copy->prev = slow;

		slow = slow->next;
		fast = fast->next;
	}

	do_if_fail(copy != NULL)
	{
		dst->prev = slow;
		slow->next = dst;
		return -2;
	}

	dst->prev = copy;
	copy->next = dst;

	return 0;
}

void list_purge(List *list, FreeFunc free_func)
{
	return_if_fail(list != NULL);
	return_if_fail(free_func != NULL);

	if (list->next == list)
		return;

	List *current = list->next;

	while (current != list) {
		List *next = current->next;
		free_func(current);
		current = next;
	}

	list->next = list;
	list->prev = list;
}
