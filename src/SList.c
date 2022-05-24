#include "SList.h"

#include "Messages.h"

TrbSList *trb_slist_init(TrbSList *self)
{
	if (self == NULL) {
		self = trb_talloc(TrbSList, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the list!");
			return NULL;
		}
	}

	self->next = self;

	return self;
}

void trb_slist_push_back(TrbSList *self, TrbSList *node)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(node != NULL);

	TrbSList *end;

	for (end = self->next; end->next != self; end = end->next)
		continue;

	end->next = node;
	node->next = self;
}

void trb_slist_push_front(TrbSList *self, TrbSList *node)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(node != NULL);

	node->next = self->next;
	self->next = node;
}

bool trb_slist_empty(const TrbSList *self)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	return self->next == self;
}

usize trb_slist_len(const TrbSList *self)
{
	trb_return_val_if_fail(self != NULL, 0);

	if (self->next == self)
		return 0;

	usize len = 0;
	TrbSList *iter;

	trb_slist_foreach (iter, self)
		len++;

	return len;
}

void trb_slist_splice(TrbSList *self, TrbSList *node)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(node != NULL);

	TrbSList *start = self->next;
	TrbSList *end;

	for (end = self->next; end->next != self; end = end->next)
		continue;

	end->next = node->next;
	node->next = start;

	self->next = self;
}

void trb_slist_reverse(TrbSList *self)
{
	trb_return_if_fail(self != NULL);

	if (self->next == self)
		return;

	TrbSList *current = self->next;
	TrbSList *prev = self;

	while (current != self) {
		TrbSList *next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}

	self->next = prev;
}

void trb_slist_insert_after(TrbSList *sibling, TrbSList *node)
{
	trb_return_if_fail(sibling != NULL);
	trb_return_if_fail(node != NULL);

	node->next = sibling->next;
	sibling->next = node;
}

void trb_slist_insert_before(TrbSList *self, TrbSList *sibling, TrbSList *node)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(sibling != NULL);
	trb_return_if_fail(node != NULL);

	TrbSList *iter;
	TrbSList *prev = self;

	for (iter = self->next; iter != self; iter = iter->next) {
		if (iter == sibling) {
			prev->next = node;
			node->next = sibling;
		}

		prev = iter;
	}
}

static TrbSList *__trb_slist_merge(TrbSList *self, TrbSList *first, TrbSList *second, TrbCmpFunc cmp_func)
{
	TrbSList *result = self;
	TrbSList **linkp = &result;

	TrbSList *f = first;
	TrbSList *s = second;

	while (1) {
		if (f == self && s == self)
			break;

		if (f == self) {
			*linkp = s;
			break;
		}

		if (s == self) {
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

void trb_slist_sort(TrbSList *self, TrbCmpFunc cmp_func)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(cmp_func != NULL);

	if (self->next == self)
		return;

	TrbSList *array[USIZE_WIDTH] = { [0 ...(USIZE_WIDTH - 1)] = self };
	TrbSList *result;
	u32 max_i = 0;
	u32 i;

	result = self->next;

	while (result != self) {
		TrbSList *next;

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
			result = __trb_slist_merge(self, array[i], result, cmp_func);
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
		result = __trb_slist_merge(self, array[j], result, cmp_func);

	self->next = result;
}

static TrbSList *__trb_slist_merge_data(TrbSList *self, TrbSList *first, TrbSList *second, TrbCmpDataFunc cmpd_func, void *data)
{
	TrbSList *result = self;
	TrbSList **linkp = &result;

	TrbSList *f = first;
	TrbSList *s = second;

	while (1) {
		if (f == self && s == self)
			break;

		if (f == self) {
			*linkp = s;
			break;
		}

		if (s == self) {
			*linkp = f;
			break;
		}

		if (cmpd_func(f, s, data) <= 0) {
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

void trb_slist_sort_data(TrbSList *self, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(cmpd_func != NULL);

	if (self->next == self)
		return;

	TrbSList *array[USIZE_WIDTH] = { [0 ...(USIZE_WIDTH - 1)] = self };
	TrbSList *result;
	u32 max_i = 0;
	u32 i;

	result = self->next;

	while (result != self) {
		TrbSList *next;

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
			result = __trb_slist_merge_data(self, array[i], result, cmpd_func, data);
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
		result = __trb_slist_merge_data(self, array[j], result, cmpd_func, data);

	self->next = result;
}

TrbSList *trb_slist_nth(TrbSList *self, usize n)
{
	trb_return_val_if_fail(self != NULL, NULL);

	usize i = 0;
	TrbSList *iter;

	trb_slist_foreach (iter, self) {
		if (i++ == n) {
			return iter;
		}
	}

	return NULL;
}

usize trb_slist_position(TrbSList *self, TrbSList *node)
{
	trb_return_val_if_fail(self != NULL, -1);

	usize i = 0;
	TrbSList *iter;

	trb_slist_foreach (iter, self) {
		if (iter == node) {
			return i;
		}

		i++;
	}

	return -1;
}

TrbSList *trb_slist_lookup(TrbSList *self, const TrbSList *node, TrbCmpFunc cmp_func, usize *index)
{
	trb_return_val_if_fail(self != NULL, NULL);
	trb_return_val_if_fail(cmp_func != NULL, NULL);

	usize i = 0;
	TrbSList *iter;

	trb_slist_foreach (iter, self) {
		if (cmp_func(iter, node) == 0) {
			if (index != NULL)
				*index = i;
			return iter;
		}

		i++;
	}

	return NULL;
}

TrbSList *trb_slist_lookup_data(TrbSList *self, const TrbSList *node, TrbCmpDataFunc cmpd_func, void *data, usize *index)
{
	trb_return_val_if_fail(self != NULL, NULL);
	trb_return_val_if_fail(cmpd_func != NULL, NULL);

	usize i = 0;
	TrbSList *iter;

	trb_slist_foreach (iter, self) {
		if (cmpd_func(iter, node, data) == 0) {
			if (index != NULL)
				*index = i;
			return iter;
		}

		i++;
	}

	return NULL;
}

void trb_slist_remove(TrbSList *self, TrbSList *node)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(node != NULL);

	TrbSList *iter;
	TrbSList *prev = self;

	trb_slist_foreach (iter, self) {
		if (iter == node) {
			prev->next = iter->next;
			iter->next = iter;

			return;
		}

		prev = iter;
	}
}

TrbSList *trb_slist_pop_back(TrbSList *self)
{
	trb_return_val_if_fail(self != NULL, NULL);

	if (self->next == self)
		return NULL;

	TrbSList *last;
	TrbSList *prev = self;

	for (last = self->next; last->next != self; last = last->next)
		prev = last;

	last->next = last;
	prev->next = self;

	return last;
}

TrbSList *trb_slist_pop_front(TrbSList *self)
{
	trb_return_val_if_fail(self != NULL, NULL);

	if (self->next == self)
		return NULL;

	TrbSList *first = self->next;
	self->next = first->next;
	first->next = first;

	return first;
}

TrbSList *trb_slist_copy(TrbSList *dst, const TrbSList *src, TrbCopyFunc copy_func, bool *status)
{
	trb_return_val_if_fail(src != NULL, FALSE);
	trb_return_val_if_fail(copy_func != NULL, FALSE);

	if (dst == NULL) {
		dst = trb_talloc(TrbSList, 1);

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

	TrbSList *fast = src->next;
	TrbSList *slow = dst;
	TrbSList *copy = NULL;

	while (fast != src) {
		copy = copy_func(fast);

		if (copy == NULL)
			break;

		slow->next = copy;

		slow = slow->next;
		fast = fast->next;
	}

	slow->next = dst;

	trb_do_if_fail (copy != NULL)
		if (status != NULL)
			*status = FALSE;

	return dst;
}

void trb_slist_destroy(TrbSList *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);

	if (self->next == self)
		return;

	if (free_func == NULL)
		free_func = free;

	TrbSList *current = self->next;

	while (current != self) {
		TrbSList *next = current->next;
		free_func(current);
		current = next;
	}

	self->next = self;
}

void trb_slist_free(TrbSList *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);
	trb_slist_destroy(self, free_func);
	free(self);
}
