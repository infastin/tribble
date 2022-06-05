#include "trb-macros.h"
#include "trb-utils.h"
#include "trb-vector.h"

#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

void test_push_destroy()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	trb_vector_push_back(&vec, trb_get_ptr(u32, 10));
	assert(vec.len == 1);

	trb_vector_push_front(&vec, trb_get_ptr(u32, 20));
	assert(vec.len == 2);

	trb_vector_push_back(&vec, trb_get_ptr(u32, 30));
	assert(vec.len == 3);

	trb_vector_push_front(&vec, trb_get_ptr(u32, 40));
	assert(vec.len == 4);

	trb_vector_push_back(&vec, NULL);
	assert(vec.len == 5);

	u32 arr[5] = { 40, 20, 10, 30, 0 };

	for (usize i = 0; i < 5; ++i) {
		u32 val = trb_vector_get(&vec, u32, i);
		assert(val == arr[i]);
	}

	trb_vector_destroy(&vec, NULL);

	assert(vec.capacity == 0);
	assert(vec.len == 0);
	assert(vec.data == NULL);
}

void test_insert()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	trb_vector_push_back(&vec, trb_get_ptr(u32, 10));
	trb_vector_push_back(&vec, trb_get_ptr(u32, 20));
	trb_vector_push_back(&vec, trb_get_ptr(u32, 30));
	trb_vector_push_back(&vec, trb_get_ptr(u32, 40));
	trb_vector_push_back(&vec, trb_get_ptr(u32, 50));

	trb_vector_insert(&vec, 3, trb_get_ptr(u32, 35));
	assert(vec.len == 6);

	trb_vector_insert(&vec, 2, trb_get_ptr(u32, 22));
	assert(vec.len == 7);

	trb_vector_insert(&vec, 6, trb_get_ptr(u32, 33));
	assert(vec.len == 8);

	trb_vector_insert(&vec, 22, trb_get_ptr(u32, 2));
	assert(vec.len == 23);

	u32 arr[8] = { 10, 20, 22, 30, 35, 40, 33, 50 };

	for (usize i = 0; i < 8; ++i) {
		u32 val = trb_vector_get(&vec, u32, i);
		assert(val == arr[i]);
	}

	assert(trb_vector_insert(&vec, USIZE_MAX, trb_get_ptr(u32, 10)) == FALSE);

	trb_vector_destroy(&vec, NULL);
}

void test_insert_many()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[6] = { 11, 22, 33, 44, 55, 66 };
	u32 arr3[5] = { 12, 23, 34, 45, 56 };

	trb_vector_push_back_many(&vec, arr1, 4);
	assert(vec.len == 4);

	trb_vector_push_front_many(&vec, arr2, 6);
	assert(vec.len == 10);

	trb_vector_insert_many(&vec, 5, arr3, 5);
	assert(vec.len == 15);

	u32 arr[15] = { 11, 22, 33, 44, 55, 12, 23, 34, 45, 56, 66, 10, 20, 30, 40 };

	for (usize i = 0; i < 15; ++i) {
		u32 val = trb_vector_get(&vec, u32, i);
		assert(val == arr[i]);
	}

	assert(trb_vector_push_back_many(&vec, arr1, USIZE_MAX) == FALSE);

	trb_vector_destroy(&vec, NULL);
}

void test_pop_remove()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	assert(trb_vector_pop_back(&vec, NULL) == FALSE);
	assert(trb_vector_pop_front(&vec, NULL) == FALSE);

	u32 arr1[8] = { 10, 20, 30, 40, 11, 22, 33, 44 };

	trb_vector_push_back_many(&vec, arr1, 8);

	u32 val;

	trb_vector_pop_back(&vec, &val);
	assert(val == 44);
	assert(vec.len == 7);

	trb_vector_pop_front(&vec, &val);
	assert(val == 10);
	assert(vec.len == 6);

	trb_vector_remove(&vec, 3, &val);
	assert(val == 11);
	assert(vec.len == 5);

	assert(trb_vector_remove(&vec, 10, NULL) == FALSE);

	u32 arr[5] = { 20, 30, 40, 22, 33 };

	for (usize i = 0; i < 5; ++i) {
		u32 val = trb_vector_get(&vec, u32, i);
		assert(val == arr[i]);
	}

	trb_vector_destroy(&vec, NULL);
}

void test_remove_range()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	assert(trb_vector_remove_range(&vec, 10, U64_MAX, NULL) == FALSE);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	trb_vector_push_back_many(&vec, arr1, 4);
	trb_vector_push_back_many(&vec, arr2, 4);
	trb_vector_push_back_many(&vec, arr3, 4);
	trb_vector_push_back_many(&vec, arr4, 4);

	u32 arr[4];

	trb_vector_remove_range(&vec, 4, 4, arr);
	assert(vec.len == 12);

	for (usize i = 0; i < 4; ++i) {
		assert(arr[i] == arr2[i]);
	}

	trb_vector_remove_range(&vec, 8, 4, arr);
	assert(vec.len == 8);

	for (usize i = 0; i < 4; ++i) {
		assert(arr[i] == arr4[i]);
	}

	trb_vector_remove_range(&vec, 0, 4, arr);
	assert(vec.len == 4);

	for (usize i = 0; i < 4; ++i) {
		assert(arr[i] == arr1[i]);
	}

	trb_vector_remove_range(&vec, 0, 4, arr);
	assert(vec.len == 0);

	for (usize i = 0; i < 4; ++i) {
		assert(arr[i] == arr3[i]);
	}

	assert(trb_vector_remove_range(&vec, 0, 5, NULL) == FALSE);
	assert(vec.len == 0);

	trb_vector_destroy(&vec, NULL);
}

void test_sort_reverse()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	trb_vector_push_back_many(&vec, arr1, 4);
	trb_vector_push_back_many(&vec, arr2, 4);
	trb_vector_push_back_many(&vec, arr3, 4);
	trb_vector_push_back_many(&vec, arr4, 4);

	TrbSlice vec_slice;
	trb_vector_slice(&vec, &vec_slice, 0, vec.len);
	trb_quicksort(&vec_slice, (TrbCmpFunc) trb_u32cmp);
	assert(vec.len == 16);

	u32 arr[16] = { 2, 7, 10, 11, 12, 20, 22, 30, 33, 40, 44, 45, 55, 77, 88, 98 };

	for (usize i = 0; i < 16; ++i) {
		u32 val = trb_vector_get(&vec, u32, i);
		assert(val == arr[i]);
	}

	trb_reverse(&vec_slice);
	assert(vec.len == 16);

	u32 arr_[16] = { 98, 88, 77, 55, 45, 44, 40, 33, 30, 22, 20, 12, 11, 10, 7, 2 };

	for (usize i = 0; i < 16; ++i) {
		u32 val = trb_vector_get(&vec, u32, i);
		assert(val == arr_[i]);
	}

	trb_vector_destroy(&vec, NULL);
}

void test_set_get()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	trb_vector_push_back_many(&vec, arr1, 4);
	trb_vector_push_back_many(&vec, arr2, 4);
	trb_vector_push_back_many(&vec, arr3, 4);
	trb_vector_push_back_many(&vec, arr4, 4);

	*trb_vector_ptr(&vec, u32, 3) = 3;
	u32 val = trb_vector_get(&vec, u32, 3);
	assert(val != 40);
	assert(val == 3);
	assert(vec.len == 16);

	*trb_vector_ptr(&vec, u32, 4) = 922;
	val = trb_vector_get(&vec, u32, 4);
	assert(val != 11);
	assert(val == 922);
	assert(vec.len == 16);

	*trb_vector_ptr(&vec, u32, 5) = 777;
	val = trb_vector_get(&vec, u32, 5);
	assert(val != 40);
	assert(val == 777);
	assert(vec.len == 16);

	*trb_vector_ptr(&vec, u32, 6) = 13209;
	val = trb_vector_get(&vec, u32, 6);
	assert(val != 40);
	assert(val == 13209);
	assert(vec.len == 16);

	trb_vector_destroy(&vec, NULL);
}

void test_set_range()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	trb_vector_push_back_many(&vec, arr1, 4);
	trb_vector_push_back_many(&vec, arr2, 4);
	trb_vector_push_back_many(&vec, arr3, 4);
	trb_vector_push_back_many(&vec, arr4, 4);

	u32 arr5[4] = { 95, 159, 32, 73 };

	memcpy(trb_vector_ptr(&vec, u32, 4), arr5, 4 * sizeof(u32));

	u32 arr[16] = { 10, 20, 30, 40, 95, 159, 32, 73, 98, 88, 77, 45, 12, 55, 7, 2 };

	for (usize i = 0; i < 16; ++i) {
		u32 val = trb_vector_get(&vec, u32, i);
		assert(val == arr[i]);
	}

	trb_vector_destroy(&vec, NULL);
}

void test_get_range()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	trb_vector_push_back_many(&vec, arr1, 4);
	trb_vector_push_back_many(&vec, arr2, 4);
	trb_vector_push_back_many(&vec, arr3, 4);
	trb_vector_push_back_many(&vec, arr4, 4);

	u32 *arr = trb_vector_ptr(&vec, u32, 4);

	for (usize i = 0; i < 4; ++i) {
		u32 val = trb_vector_get(&vec, u32, i + 4);
		assert(val == arr[i]);
	}

	trb_vector_destroy(&vec, NULL);
}

void test_steal()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	trb_vector_push_back_many(&vec, arr1, 4);
	trb_vector_push_back_many(&vec, arr2, 4);
	trb_vector_push_back_many(&vec, arr3, 4);
	trb_vector_push_back_many(&vec, arr4, 4);

	u32 arr[16] = { 10, 20, 30, 40, 11, 22, 33, 44, 98, 88, 77, 45, 12, 55, 7, 2 };
	u32 *arr_ = trb_vector_steal(&vec, NULL);

	for (usize i = 0; i < 16; ++i) {
		assert(arr_[i] == arr[i]);
	}

	assert(vec.capacity != 0);
	assert(vec.len == 0);

	trb_vector_push_back_many(&vec, arr1, 4);
	trb_vector_push_back_many(&vec, arr2, 4);
	trb_vector_push_back_many(&vec, arr3, 4);
	trb_vector_push_back_many(&vec, arr4, 4);

	u32 *arr__ = trb_vector_steal0(&vec, NULL);

	for (usize i = 0; i < 16; ++i) {
		assert(arr__[i] == arr_[i]);
	}

	free(arr__);

	assert(vec.capacity == 0);
	assert(vec.len == 0);
	assert(vec.data == NULL);
}

void test_shrink()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	trb_vector_push_back_many(&vec, arr1, 4);
	trb_vector_push_back_many(&vec, arr2, 4);
	trb_vector_push_back_many(&vec, arr3, 4);
	trb_vector_push_back_many(&vec, arr4, 4);

	trb_vector_shrink(&vec);

	assert(vec.capacity == 16);
	assert(vec.len == 16);

	trb_vector_destroy(&vec, NULL);
}

void test_reserve()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	trb_vector_require(&vec, 16);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	trb_vector_push_back_many(&vec, arr1, 4);
	assert(vec.len == 4);
	assert(vec.capacity == 24);

	trb_vector_push_back_many(&vec, arr2, 4);
	assert(vec.len == 8);
	assert(vec.capacity == 24);

	trb_vector_push_back_many(&vec, arr3, 4);
	assert(vec.len == 12);
	assert(vec.capacity == 24);

	trb_vector_push_back_many(&vec, arr4, 4);
	assert(vec.len == 16);
	assert(vec.capacity == 24);

	trb_vector_destroy(&vec, NULL);
}

void test_search()
{
	TrbVector vec;
	trb_vector_init(&vec, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	trb_vector_push_back_many(&vec, arr1, 4);
	trb_vector_push_back_many(&vec, arr2, 4);
	trb_vector_push_back_many(&vec, arr3, 4);
	trb_vector_push_back_many(&vec, arr4, 4);

	usize index;
	assert(trb_vector_search(&vec, trb_get_ptr(u32, 33), (TrbCmpFunc) trb_u32cmp, &index));
	assert(trb_vector_get(&vec, u32, index) == 33);

	assert(trb_vector_search(&vec, trb_get_ptr(u32, 12), (TrbCmpFunc) trb_u32cmp, &index));
	assert(trb_vector_get(&vec, u32, index) == 12);

	assert(trb_vector_search(&vec, trb_get_ptr(u32, 110), (TrbCmpFunc) trb_u32cmp, NULL) == FALSE);

	trb_vector_destroy(&vec, NULL);
}

int main()
{
	test_push_destroy();
	test_insert();
	test_insert_many();
	test_pop_remove();
	test_remove_range();
	test_sort_reverse();
	test_set_get();
	test_steal();
	test_shrink();
	test_reserve();
	test_search();
	test_set_range();
	test_get_range();

	return 0;
}
