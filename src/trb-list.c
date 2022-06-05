#include "trb-list.h"

#include "trb-messages.h"

TrbList *trb_list_init(TrbList *self)
{
	if (self == NULL) {
		self = trb_talloc(TrbList, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the list!");
			return NULL;
		}
	}

	self->next = self;
	self->prev = self;

	return self;
}

void trb_list_push_back(TrbList *self, TrbList *node)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(node != NULL);

	TrbList *end = self->prev;

	end->next = node;
	node->prev = end;
	node->next = self;
	self->prev = node;
}

void trb_list_push_front(TrbList *self, TrbList *node)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(node != NULL);

	TrbList *start = self->next;

	start->prev = node;
	node->next = start;
	node->prev = self;
	self->next = node;
}

void trb_list_insert_after(TrbList *sibling, TrbList *node)
{
	trb_return_if_fail(sibling != NULL);
	trb_return_if_fail(node != NULL);

	node->prev = sibling;
	node->next = sibling->next;
	sibling->next->prev = node;
	sibling->next = node;
}

void trb_list_insert_before(TrbList *sibling, TrbList *node)
{
	trb_return_if_fail(sibling != NULL);
	trb_return_if_fail(node != NULL);

	node->next = sibling;
	node->prev = sibling->prev;
	sibling->prev->next = node;
	sibling->prev = node;
}

bool trb_list_empty(const TrbList *self)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	return self->next == self;
}

usize trb_list_len(const TrbList *self)
{
	trb_return_val_if_fail(self != NULL, 0);

	if (self->next == self)
		return 0;

	usize len = 0;
	TrbList *iter;

	trb_list_foreach (iter, self)
		len++;

	return len;
}

TrbList *trb_list_pop_back(TrbList *self)
{
	trb_return_val_if_fail(self != NULL, NULL);

	if (self->next == self)
		return NULL;

	TrbList *last = self->prev;

	last->prev->next = last->next;
	last->next->prev = last->prev;
	last->next = last;
	last->prev = last;

	return last;
}

TrbList *trb_list_pop_front(TrbList *self)
{
	trb_return_val_if_fail(self != NULL, NULL);

	if (self->prev == self)
		return NULL;

	TrbList *first = self->next;

	first->prev->next = first->next;
	first->next->prev = first->prev;
	first->next = first;
	first->prev = first;

	return first;
}

void trb_list_reverse(TrbList *self)
{
	trb_return_if_fail(self != NULL);

	if (self->next == self)
		return;

	TrbList *current = self->next;

	while (current != self) {
		TrbList *next = current->next;
		current->next = current->prev;
		current->prev = next;
		current = next;
	}

	TrbList *oldstart = self->next;
	self->next = self->prev;
	self->prev = oldstart;
}

void trb_list_splice(TrbList *self, TrbList *node)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(node != NULL);

	if (self->next == self)
		return;

	TrbList *start = self->next;
	TrbList *end = self->prev;
	TrbList *at = node->next;

	start->prev = node;
	node->next = start;

	end->next = at;
	at->prev = end;

	self->next = self;
	self->prev = self;
}

static TrbList *__trb_list_merge(TrbList *self, TrbList *first, TrbList *second, TrbCmpFunc cmp_func)
{
	TrbList *result = self;
	TrbList *prev = self;
	TrbList **linkp = &result;

	TrbList *f = first;
	TrbList *s = second;

	while (1) {
		if (f == self && s == self)
			break;

		if (f == self) {
			s->prev = prev;
			*linkp = s;

			break;
		}

		if (s == self) {
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

void trb_list_sort(TrbList *self, TrbCmpFunc cmp_func)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(cmp_func != NULL);

	if (self->next == self)
		return;

	TrbList *array[USIZE_WIDTH] = { [0 ...(USIZE_WIDTH - 1)] = self };
	TrbList *result;
	u32 max_i = 0;
	u32 i;

	result = self->next;

	while (result != self) {
		TrbList *next;

		for (next = result->next; next != self;) {
			if (cmp_func(result, next) > 0) {
				result->next = self;
				break;
			} else {
				result = next;
				next = next->next;
			}
		}

		for (i = 0; (i < USIZE_WIDTH) && (array[i] != self); ++i) {
			result = __trb_list_merge(self, array[i], result, cmp_func);
			array[i] = self;
		}

		if (i == USIZE_WIDTH)
			i--;

		if (i > max_i)
			max_i = i;

		array[i] = result;
		result = next;
	}

	for (u32 j = 0; j <= max_i; ++j)
		result = __trb_list_merge(self, array[j], result, cmp_func);

	self->next = result;

	TrbList *end;
	for (end = self->next; end->next != self; end = end->next)
		continue;

	self->prev = end;
}

static TrbList *__trb_list_merge_data(TrbList *self, TrbList *first, TrbList *second, TrbCmpDataFunc cmpd_func, void *data)
{
	TrbList *result = self;
	TrbList *prev = self;
	TrbList **linkp = &result;

	TrbList *f = first;
	TrbList *s = second;

	while (1) {
		if (f == self && s == self)
			break;

		if (f == self) {
			s->prev = prev;
			*linkp = s;

			break;
		}

		if (s == self) {
			f->prev = prev;
			*linkp = f;

			break;
		}

		if (cmpd_func(f, s, data) <= 0) {
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

void trb_list_sort_data(TrbList *self, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(cmpd_func != NULL);

	if (self->next == self)
		return;

	TrbList *array[USIZE_WIDTH] = { [0 ...(USIZE_WIDTH - 1)] = self };
	TrbList *result;
	u32 max_i = 0;
	u32 i;

	result = self->next;

	while (result != self) {
		TrbList *next;

		for (next = result->next; next != self;) {
			if (cmpd_func(result, next, data) > 0) {
				result->next = self;
				break;
			} else {
				result = next;
				next = next->next;
			}
		}

		for (i = 0; (i < USIZE_WIDTH) && (array[i] != self); ++i) {
			result = __trb_list_merge_data(self, array[i], result, cmpd_func, data);
			array[i] = self;
		}

		if (i == USIZE_WIDTH)
			i--;

		if (i > max_i)
			max_i = i;

		array[i] = result;
		result = next;
	}

	for (u32 j = 0; j <= max_i; ++j)
		result = __trb_list_merge_data(self, array[j], result, cmpd_func, data);

	self->next = result;

	TrbList *end;
	for (end = self->next; end->next != self; end = end->next)
		continue;

	self->prev = end;
}

TrbList *trb_list_nth(TrbList *self, usize n)
{
	trb_return_val_if_fail(self != NULL, NULL);

	usize i = 0;
	TrbList *iter;

	trb_list_foreach (iter, self) {
		if (i++ == n) {
			return iter;
		}
	}

	return NULL;
}

usize trb_list_position(TrbList *self, TrbList *node)
{
	trb_return_val_if_fail(self != NULL, -1);

	usize i = 0;
	TrbList *iter;

	trb_list_foreach (iter, self) {
		if (iter == node) {
			return i;
		}

		i++;
	}

	return -1;
}

TrbList *trb_list_lookup(TrbList *self, const TrbList *node, TrbCmpFunc cmp_func, usize *index)
{
	trb_return_val_if_fail(self != NULL, NULL);
	trb_return_val_if_fail(cmp_func != NULL, NULL);

	usize i = 0;
	TrbList *iter;

	trb_list_foreach (iter, self) {
		if (cmp_func(iter, node) == 0) {
			if (index != NULL)
				*index = i;
			return iter;
		}

		i++;
	}

	return NULL;
}

TrbList *trb_list_lookup_data(TrbList *self, const TrbList *node, TrbCmpDataFunc cmpd_func, void *data, usize *index)
{
	trb_return_val_if_fail(self != NULL, NULL);
	trb_return_val_if_fail(cmpd_func != NULL, NULL);

	usize i = 0;
	TrbList *iter;

	trb_list_foreach (iter, self) {
		if (cmpd_func(iter, node, data) == 0) {
			if (index != NULL)
				*index = i;
			return iter;
		}

		i++;
	}

	return NULL;
}

void trb_list_remove(TrbList *node)
{
	trb_return_if_fail(node != NULL);

	node->prev->next = node->next;
	node->next->prev = node->prev;

	node->next = node;
	node->prev = node;
}

TrbList *trb_list_copy(const TrbList *src, TrbList *dst, TrbCopyFunc copy_func, bool *status)
{
	trb_return_val_if_fail(src != NULL, FALSE);
	trb_return_val_if_fail(copy_func != NULL, FALSE);

	if (dst == NULL) {
		dst = trb_talloc(TrbList, 1);

		if (dst == NULL) {
			trb_msg_error("couldn't allocate memory for a copy of the list!");

			if (status != NULL)
				*status = FALSE;

			return NULL;
		}
	}

	if (status != NULL)
		*status = TRUE;

	if (src->next == src)
		return dst;

	TrbList *fast = src->next;
	TrbList *slow = dst;
	TrbList *copy = NULL;

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

	trb_do_if_fail (copy != NULL)
		if (status != NULL)
			*status = FALSE;

	return dst;
}

void trb_list_destroy(TrbList *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);

	if (self->next == self)
		return;

	if (free_func == NULL)
		free_func = free;

	TrbList *current = self->next;

	while (current != self) {
		TrbList *next = current->next;
		free_func(current);
		current = next;
	}

	self->next = self;
	self->prev = self;
}

void trb_list_free(TrbList *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);

	trb_list_destroy(self, free_func);
	free(self);
}
