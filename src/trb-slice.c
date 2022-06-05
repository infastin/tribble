#include "trb-slice.h"

#include "trb-macros.h"
#include "trb-messages.h"

static void *__trb_slice_at(const TrbSlice *self, usize index)
{
	trb_return_val_if_fail(self != NULL, NULL);

	usize len = trb_slice_len(self);
	if (index >= len)
		return trb_array_cell(self->data, self->elemsize, self->end);

	return trb_array_cell(self->data, self->elemsize, self->start + index);
}

TrbSlice *trb_slice_init(TrbSlice *self, void *data, usize elemsize, usize start, usize end)
{
	trb_return_val_if_fail(data != NULL, NULL);
	trb_return_val_if_fail(elemsize != 0, NULL);
	trb_return_val_if_fail(start < end, NULL);

	if (self == NULL) {
		self = trb_talloc(TrbSlice, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the slice!");
			return NULL;
		}
	}

	self->at = __trb_slice_at;
	self->data = data;
	self->elemsize = elemsize;
	self->start = start;
	self->end = end;

	return self;
}

TrbSlice *trb_slice_reslice(TrbSlice *src, TrbSlice *dst, usize start, usize end)
{
	trb_return_val_if_fail(src != NULL, NULL);
	trb_return_val_if_fail(start <= end, NULL);

	usize len = src->end - src->start;
	if (end > len) {
		trb_msg_warn("interval [%zu:%zu) is out of bounds!", start, end);
		return NULL;
	}

	if (dst == NULL) {
		dst = trb_talloc(TrbSlice, 1);

		if (dst == NULL) {
			trb_msg_error("couldn't allocate memory for the slice!");
			return NULL;
		}
	}

	dst->at = src->at;
	dst->data = src->data;
	dst->elemsize = src->elemsize;
	dst->start = src->start + start;
	dst->end = src->start + end;

	return dst;
}
