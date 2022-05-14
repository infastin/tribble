#include "List.h"

#include "Messages.h"

List *list_init(List *list)
{
	if (list == NULL) {
		list = talloc(List, 1);

		if (list == NULL) {
			msg_error("couldn't allocate memory for the list!");
			return NULL;
		}
	}

	list->next = list;
	list->prev = list;

	return list;
}

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

usize list_len(const List *list)
{
	return_val_if_fail(list != NULL, 0);

	if (list->next == list)
		return 0;

	usize len = 0;
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
	List *result = list;
	List *prev = list;
	List **linkp = &result;

	List *f = first;
	List *s = second;

	while (1) {
		if (f == list && s == list)
			break;

		if (f == list) {
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

	List *array[32] = { [0 ... 31] = list };
	List *result;
	i32 max_i = 0;
	i32 i;

	result = list->next;

	while (result != list) {
		List *next;

		next = result->next;
		result->next = list;

		for (i = 0; (i < 32) && (array[i] != list); ++i) {
			result = __list_merge(list, array[i], result, cmp_func);
			array[i] = list;
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

List *list_nth(List *list, usize n)
{
	return_val_if_fail(list != NULL, NULL);

	usize i = 0;
	List *iter;

	list_foreach (iter, list) {
		if (i++ == n) {
			return iter;
		}
	}

	return NULL;
}

usize list_position(List *list, List *node)
{
	return_val_if_fail(list != NULL, -1);

	usize i = 0;
	List *iter;

	list_foreach (iter, list) {
		if (iter == node) {
			return i;
		}

		i++;
	}

	return -1;
}

List *list_lookup(List *list, const List *node, CmpFunc cmp_func, usize *index)
{
	return_val_if_fail(list != NULL, NULL);
	return_val_if_fail(cmp_func != NULL, NULL);

	usize i = 0;
	List *iter;

	list_foreach (iter, list) {
		if (cmp_func(iter, node) == 0) {
			if (index != NULL)
				*index = i;
			return iter;
		}

		i++;
	}

	return NULL;
}

void list_remove(List *node)
{
	return_if_fail(node != NULL);

	node->prev->next = node->next;
	node->next->prev = node->prev;

	node->next = node;
	node->prev = node;
}

List *list_copy(List *dst, const List *src, CopyFunc copy_func, bool *status)
{
	return_val_if_fail(src != NULL, FALSE);
	return_val_if_fail(copy_func != NULL, FALSE);

	if (dst == NULL) {
		dst = talloc(List, 1);

		if (dst == NULL) {
			msg_error("couldn't allocate memory for a copy of the list!");

			if (status != NULL)
				*status = FALSE;

			return NULL;
		}
	}

	if (status != NULL)
		*status = TRUE;

	if (src->next == src)
		return dst;

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
		if (status != NULL)
			*status = FALSE;

	return dst;
}

void list_destroy(List *list, FreeFunc free_func)
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

void list_free(List *list, FreeFunc free_func)
{
	return_if_fail(list != NULL);
	return_if_fail(free_func != NULL);

	list_destroy(list, free_func);
	free(list);
}
