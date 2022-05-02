#include "SList.h"

#include "Messages.h"

void slist_push_back(SList *list, SList *node)
{
	return_if_fail(list != NULL);
	return_if_fail(node != NULL);

	SList *end;

	for (end = list->next; end->next != list; end = end->next)
		continue;

	end->next = node;
	node->next = list;
}

void slist_push_front(SList *list, SList *node)
{
	return_if_fail(list != NULL);
	return_if_fail(node != NULL);

	node->next = list->next;
	list->next = node;
}

bool slist_empty(const SList *list)
{
	return_val_if_fail(list != NULL, FALSE);

	return list->next == list;
}

u32 slist_len(const SList *list)
{
	return_val_if_fail(list != NULL, 0);

	if (list->next == list)
		return 0;

	u32 len = 0;
	SList *iter;

	slist_foreach (iter, list)
		len++;

	return len;
}

void slist_splice(SList *list, SList *node)
{
	return_if_fail(list != NULL);
	return_if_fail(node != NULL);

	SList *start = list->next;
	SList *end;

	for (end = list->next; end->next != list; end = end->next)
		continue;

	end->next = node->next;
	node->next = start;

	list->next = list;
}

void slist_reverse(SList *list)
{
	return_if_fail(list != NULL);

	if (list->next == list)
		return;

	SList *current = list->next;
	SList *prev = list;

	while (current != list) {
		SList *next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}

	list->next = prev;
}

void slist_insert_after(SList *sibling, SList *node)
{
	return_if_fail(sibling != NULL);
	return_if_fail(node != NULL);

	node->next = sibling->next;
	sibling->next = node;
}

void slist_insert_before(SList *list, SList *sibling, SList *node)
{
	return_if_fail(list != NULL);
	return_if_fail(sibling != NULL);
	return_if_fail(node != NULL);

	SList *iter;
	SList *prev = list;

	for (iter = list->next; iter != list; iter = iter->next)
	{
		if (iter == sibling) {
			prev->next = node;
			node->next = sibling;
		}

		prev = iter;
	}
}

static SList *__slist_merge(SList *list, SList *first, SList *second, CmpFunc cmp_func)
{
	SList *result = NULL;
	SList **linkp = &result;

	SList *f = first;
	SList *s = second;

	while (1) {
		if (f == list && s == list)
			break;

		if (f == list || f == NULL) {
			*linkp = s;
			break;
		}

		if (s == list) {
			*linkp = f;
			break;
		}

		if (cmp_func(f, s) <= 0) {
			*linkp = f;
			linkp = &f->next;
			f = f->next;
		} else {
			*linkp = s;
			linkp = &s->next;
			s = s->next;
		}
	}

	return result;
}

void slist_sort(SList *list, CmpFunc cmp_func)
{
	return_if_fail(list != NULL);
	return_if_fail(cmp_func != NULL);

	if (list->next == list)
		return;

	SList *array[32] = { 0 };
	SList *result;
	i32 max_i = 0;
	i32 i;

	result = list->next;

	while (result != list) {
		SList *next;

		next = result->next;
		result->next = list;

		for (i = 0; (i < 32) && (array[i] != NULL); ++i) {
			result = __slist_merge(list, array[i], result, cmp_func);
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
		result = __slist_merge(list, array[j], result, cmp_func);

	list->next = result;
}

SList *slist_nth(SList *list, u32 n)
{
	return_val_if_fail(list != NULL, NULL);

	u32 i = 0;
	SList *iter;

	slist_foreach (iter, list) {
		if (i++ == n) {
			return iter;
		}
	}

	return NULL;
}

u32 slist_position(SList *list, SList *node)
{
	return_val_if_fail(list != NULL, -1);

	u32 i = 0;
	SList *iter;

	slist_foreach (iter, list) {
		if (iter == node) {
			return i;
		}

		i++;
	}

	return -1;
}

void slist_remove(SList *list, SList *node)
{
	return_if_fail(list != NULL);
	return_if_fail(node != NULL);

	SList *iter;
	SList *prev = list;

	slist_foreach (iter, list) {
		if (iter == node) {
			prev->next = iter->next;
			iter->next = iter;

			return;
		}

		prev = iter;
	}
}

SList *slist_pop_back(SList *list)
{
	return_val_if_fail(list != NULL, NULL);

	if (list->next == list)
		return NULL;

	SList *last;
	SList *prev = list;

	for (last = list->next; last->next != list; last = last->next)
		prev = last;

	last->next = last;
	prev->next = list;

	return last;
}

SList *slist_pop_front(SList *list)
{
	return_val_if_fail(list != NULL, NULL);

	if (list->next == list)
		return NULL;

	SList *first = list->next;
	list->next = first->next;
	first->next = first;

	return first;
}

i32 slist_copy(SList *dst, const SList *src, CopyFunc copy_func)
{
	return_val_if_fail(dst != NULL, -1);
	return_val_if_fail(src != NULL, -1);
	return_val_if_fail(copy_func != NULL, -1);

	if (src->next == src)
		return 0;

	SList *fast = src->next;
	SList *slow = dst;
	SList *copy = NULL;

	while (fast != src) {
		copy = copy_func(fast);

		if (copy == NULL)
			break;

		slow->next = copy;

		slow = slow->next;
		fast = fast->next;
	}

	slow->next = dst;

	do_if_fail (copy != NULL)
		return -2;

	return 0;
}

void slist_purge(SList *list, FreeFunc free_func)
{
	return_if_fail(list != NULL);
	return_if_fail(free_func != NULL);

	if (list->next == list)
		return;

	SList *current = list->next;

	while (current != list) {
		SList *next = current->next;
		free_func(current);
		current = next;
	}

	list->next = list;
}