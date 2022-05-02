#include "List.h"

#include "Messages.h"

void list_push_back(List *list, List *node)
{
	return_if_fail(list != NULL);
	return_if_fail(node != NULL);

	List *end = list->prev;

	end->next = node;
	node->prev = end;
	node->next = list;
	list->prev = node;
}

void list_push_front(List *list, List *node)
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

	return list->next == list;
}

u32 list_len(const List *list)
{
	return_val_if_fail(list != NULL, 0);

	if (list->next == list)
		return 0;

	u32 len = 0;
	List *iter;

	list_foreach (iter, list)
		len++;

	return len;
}

List *list_pop_back(List *list)
{
	return_val_if_fail(list != NULL, NULL);

	if (list->next == list)
		return NULL;

	List *last = list->prev;

	last->prev->next = last->next;
	last->next->prev = last->prev;
	last->next = last;
	last->prev = last;

	return last;
}

List *list_pop_front(List *list)
{
	return_val_if_fail(list != NULL, NULL);

	if (list->prev == list)
		return NULL;

	List *first = list->next;

	first->prev->next = first->next;
	first->next->prev = first->prev;
	first->next = first;
	first->prev = first;

	return first;
}

void list_reverse(List *list)
{
	return_if_fail(list != NULL);

	if (list->next == list)
		return;

	List *current = list->next;

	while (current != list) {
		List *next = current->next;
		current->next = current->prev;
		current->prev = next;
		current = next;
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

	list->next = list;
	list->prev = list;
}

static List *__list_merge(List *list, List *first, List *second, CmpFunc cmp_func)
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

			break;
		}

		if (s == list) {
			f->prev = prev;
			*linkp = f;

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
	i32 max_i = 0;
	i32 i;

	result = list->next;

	while (result != list) {
		List *next;

		next = result->next;
		result->next = list;

		for (i = 0; (i < 32) && (array[i] != NULL); ++i) {
			result = __list_merge(list, array[i], result, cmp_func);
			array[i] = NULL;
		}

		if (i == 32)
			i--;

		if (i > max_i)
			max_i = i;

		array[i] = result;
		result = next;
	}

	for (i32 j = 0; j <= max_i; ++j)
		result = __list_merge(list, array[j], result, cmp_func);

	list->next = result;

	List *end;
	for (end = list->next; end->next != list; end = end->next)
		continue;

	list->prev = end;
}

List *list_nth(List *list, u32 n)
{
	return_val_if_fail(list != NULL, NULL);

	u32 i = 0;
	List *iter;

	list_foreach (iter, list) {
		if (i++ == n) {
			return iter;
		}
	}

	return NULL;
}

u32 list_position(List *list, List *node)
{
	return_val_if_fail(list != NULL, -1);

	u32 i = 0;
	List *iter;

	list_foreach (iter, list) {
		if (iter == node) {
			return i;
		}

		i++;
	}

	return -1;
}

void list_remove(List *node)
{
	return_if_fail(node != NULL);

	node->prev->next = node->next;
	node->next->prev = node->prev;

	node->next = node;
	node->prev = node;
}

i32 list_copy(List *dst, const List *src, CopyFunc copy_func)
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

	dst->prev = slow;
	slow->next = dst;

	do_if_fail (copy != NULL)
		return -2;

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
