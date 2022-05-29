#ifndef SLICE_H_NBSLOBN2
#define SLICE_H_NBSLOBN2

#include "Types.h"

typedef struct _TrbSlice TrbSlice;

/**
 * TrbSlice:
 * @at: The function for obtaining elements in the slice.
 * @data: The slice data.
 * @elemsize: The size of each element in the slice.
 * @start: The start position in the slice data.
 * @end: The end position in the slice data.
 *
 * It is a data structure that is used to represent a portion
 * of the data in any container whose elements can be directly accessed.
 *
 * This example show how to sort an array:
 * ```c
 * i32 array[14] = { 27, 54, 14, 14, 48, 17, 44, 76, 95, 10, 97, 62, 55, 1 };
 *
 * TrbSlice slice;
 * trb_slice_init(&slice, array, 4, 0, 14);
 *
 * trb_inssort(&slice, (TrbCmpFunc) trb_i32cmp);
 *
 * for (usize i = 0; i < 14; ++i) {
 *     printf("%d ", array[i]);
 * }
 * ```
 *
 * You should get such output:
 * ```
 * 1 10 14 14 17 27 44 48 54 55 62 76 95 97
 * ```
 **/
struct _TrbSlice {
	void *(*at)(const TrbSlice *self, usize index);
	void *data;
	usize elemsize;
	usize start;
	usize end;
};

/**
 * trb_slice_init:
 * @self: (nullable): The pointer to the slice to be initialized.
 * @array: The array to be sliced.
 * @elemsize: The size of each element in the slice.
 * @start: The start position in the @array.
 * @end: The end position in the @array.
 *
 * Creates a new #TrbSlice from an array.
 *
 * Returns: (nullable): A new #TrbSlice.
 * Can return %NULL if an allocation error occurs.
 **/
TrbSlice *trb_slice_init(TrbSlice *self, void *array, usize elemsize, usize start, usize end);

/**
 * trb_slice_reslice:
 * @src: The source slice.
 * @dst: (nullable): The destination slice.
 * @start: The start position in the @dst data.
 * It is relative to the @src start position.
 * @end: The end position in the @dst data.
 * It is relative to the @src start position.
 *
 * Creates a new #TrbSlice of another #TrbSlice.
 *
 * Returns: (nullable): A new #TrbSlice.
 * Can return %NULL if an error occurs.
 **/
TrbSlice *trb_slice_reslice(TrbSlice *src, TrbSlice *dst, usize start, usize end);

/**
 * trb_slice_len:
 * @self: The slice.
 *
 * Gets the slice length.
 *
 * Returns: The slice length.
 **/
#define trb_slice_len(self) ((self)->end - (self)->start)

#endif /* end of include guard: SLICE_H_NBSLOBN2 */
