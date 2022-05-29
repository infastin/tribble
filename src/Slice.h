#ifndef SLICE_H_NBSLOBN2
#define SLICE_H_NBSLOBN2

#include "Types.h"

typedef struct _TrbSlice TrbSlice;

struct _TrbSlice {
	void *(*at)(TrbSlice *self, usize index);
	void *data;
	usize elemsize;
	usize start;
	usize end;
};

TrbSlice *trb_slice_init(TrbSlice *self, void *data, usize elemsize, usize start, usize end);

TrbSlice *trb_slice_reslice(TrbSlice *dst, TrbSlice *src, usize start, usize end);

#define trb_slice_len(self) ((self)->end - (self)->start)

#endif /* end of include guard: SLICE_H_NBSLOBN2 */
