#include "Macros.h"
#include "Messages.h"
#include "Utils.h"
#include "Vector.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_push_purge()
{
	Vector vec;
	vector_init(&vec, FALSE, FALSE, 4);

	vector_push_back(&vec, get_ptr(u32, 10));
	assert(vec.len == 1);

	vector_push_front(&vec, get_ptr(u32, 20));
	assert(vec.len == 2);

	vector_push_back(&vec, get_ptr(u32, 30));
	assert(vec.len == 3);

	vector_push_front(&vec, get_ptr(u32, 40));
	assert(vec.len == 4);

	vector_push_back(&vec, NULL);
	assert(vec.len == 5);

	u32 arr[5] = { 40, 20, 10, 30, 0 };

	for (usize i = 0; i < 5; ++i) {
		u32 val = vector_get_unsafe(&vec, u32, i);
		assert(val == arr[i]);
	}

	vector_purge(&vec, NULL);

	assert(vec.capacity == 0);
	assert(vec.len == 0);
	assert(vec.data == NULL);
}

void test_insert()
{
	Vector vec;
	vector_init(&vec, FALSE, FALSE, 4);

	vector_push_back(&vec, get_ptr(u32, 10));
	vector_push_back(&vec, get_ptr(u32, 20));
	vector_push_back(&vec, get_ptr(u32, 30));
	vector_push_back(&vec, get_ptr(u32, 40));
	vector_push_back(&vec, get_ptr(u32, 50));

	vector_insert(&vec, 3, get_ptr(u32, 35));
	assert(vec.len == 6);

	vector_insert(&vec, 2, get_ptr(u32, 22));
	assert(vec.len == 7);

	vector_insert(&vec, 6, get_ptr(u32, 33));
	assert(vec.len == 8);

	vector_insert(&vec, 22, get_ptr(u32, 2));
	assert(vec.len == 23);

	u32 arr[8] = { 10, 20, 22, 30, 35, 40, 33, 50 };

	for (usize i = 0; i < 8; ++i) {
		u32 val = vector_get_unsafe(&vec, u32, i);
		assert(val == arr[i]);
	}

	assert(vector_insert(&vec, USIZE_MAX, get_ptr(u32, 10)) == FALSE);

	vector_purge(&vec, NULL);
}

void test_insert_many()
{
	Vector vec;
	vector_init(&vec, FALSE, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[6] = { 11, 22, 33, 44, 55, 66 };
	u32 arr3[5] = { 12, 23, 34, 45, 56 };

	vector_push_back_many(&vec, arr1, 4);
	assert(vec.len == 4);

	vector_push_front_many(&vec, arr2, 6);
	assert(vec.len == 10);

	vector_insert_many(&vec, 5, arr3, 5);
	assert(vec.len == 15);

	u32 arr[15] = { 11, 22, 33, 44, 55, 12, 23, 34, 45, 56, 66, 10, 20, 30, 40 };

	for (usize i = 0; i < 15; ++i) {
		u32 val = vector_get_unsafe(&vec, u32, i);
		assert(val == arr[i]);
	}

	assert(vector_push_back_many(&vec, arr1, USIZE_MAX) == FALSE);

	vector_purge(&vec, NULL);
}

void test_pop_remove()
{
	Vector vec;
	vector_init(&vec, FALSE, FALSE, 4);

	u32 arr1[8] = { 10, 20, 30, 40, 11, 22, 33, 44 };

	vector_push_back_many(&vec, arr1, 8);

	u32 val;

	vector_pop_back(&vec, &val);
	assert(val == 44);
	assert(vec.len == 7);

	vector_pop_front(&vec, &val);
	assert(val == 10);
	assert(vec.len == 6);

	vector_remove_index(&vec, 3, &val);
	assert(val == 11);
	assert(vec.len == 5);

	u32 arr[5] = { 20, 30, 40, 22, 33 };

	for (usize i = 0; i < 5; ++i) {
		u32 val = vector_get_unsafe(&vec, u32, i);
		assert(val == arr[i]);
	}

	vector_purge(&vec, NULL);
}

void test_remove_range()
{
	Vector vec;
	vector_init(&vec, FALSE, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	vector_push_back_many(&vec, arr1, 4);
	vector_push_back_many(&vec, arr2, 4);
	vector_push_back_many(&vec, arr3, 4);
	vector_push_back_many(&vec, arr4, 4);

	u32 arr[4];

	vector_remove_range(&vec, 4, 4, arr);
	assert(vec.len == 12);

	for (usize i = 0; i < 4; ++i) {
		assert(arr[i] == arr2[i]);
	}

	vector_remove_range(&vec, 8, 4, arr);
	assert(vec.len == 8);

	for (usize i = 0; i < 4; ++i) {
		assert(arr[i] == arr4[i]);
	}

	vector_remove_range(&vec, 0, 4, arr);
	assert(vec.len == 4);

	for (usize i = 0; i < 4; ++i) {
		assert(arr[i] == arr1[i]);
	}

	vector_remove_range(&vec, 0, 4, arr);
	assert(vec.len == 0);

	for (usize i = 0; i < 4; ++i) {
		assert(arr[i] == arr3[i]);
	}

	assert(vector_remove_range(&vec, 0, 5, NULL) == FALSE);
	assert(vec.len == 0);

	vector_purge(&vec, NULL);
}

void test_zero()
{
	Vector vec;
	vector_init(&vec, FALSE, TRUE, 4);

	u32 zero = vector_get_unsafe(&vec, u32, 0);
	assert(zero == 0);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[6] = { 11, 22, 33, 44, 55, 66 };
	u32 arr3[5] = { 12, 23, 34, 45, 56 };

	vector_push_back_many(&vec, arr1, 4);
	assert(vec.len == 4);

	u32 non_zero = vector_get_unsafe(&vec, u32, 0);
	zero = vector_get_unsafe(&vec, u32, 4);
	assert(non_zero != 0);
	assert(zero == 0);

	vector_push_front_many(&vec, arr2, 6);
	assert(vec.len == 10);

	non_zero = vector_get_unsafe(&vec, u32, 4);
	zero = vector_get_unsafe(&vec, u32, 10);
	assert(non_zero != 0);
	assert(zero == 0);

	vector_insert_many(&vec, 5, arr3, 5);
	assert(vec.len == 15);

	non_zero = vector_get_unsafe(&vec, u32, 10);
	zero = vector_get_unsafe(&vec, u32, 15);
	assert(non_zero != 0);
	assert(zero == 0);

	vector_remove_range(&vec, 10, 5, NULL);
	assert(vec.len == 10);

	zero = vector_get_unsafe(&vec, u32, 10);
	assert(zero == 0);

	vector_pop_back(&vec, NULL);
	assert(vec.len == 9);

	zero = vector_get_unsafe(&vec, u32, 9);
	assert(zero == 0);

	vector_purge(&vec, NULL);
}

void test_sort_reverse()
{
	Vector vec;
	vector_init(&vec, FALSE, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	vector_push_back_many(&vec, arr1, 4);
	vector_push_back_many(&vec, arr2, 4);
	vector_push_back_many(&vec, arr3, 4);
	vector_push_back_many(&vec, arr4, 4);

	vector_sort(&vec, u32cmp);
	assert(vec.len == 16);

	u32 arr[16] = { 2, 7, 10, 11, 12, 20, 22, 30, 33, 40, 44, 45, 55, 77, 88, 98 };

	for (usize i = 0; i < 16; ++i) {
		u32 val = vector_get_unsafe(&vec, u32, i);
		assert(val == arr[i]);
	}

	vector_reverse(&vec);
	assert(vec.len == 16);

	u32 arr_[16] = { 98, 88, 77, 55, 45, 44, 40, 33, 30, 22, 20, 12, 11, 10, 7, 2 };

	for (usize i = 0; i < 16; ++i) {
		u32 val = vector_get_unsafe(&vec, u32, i);
		assert(val == arr_[i]);
	}

	vector_purge(&vec, NULL);
}

void test_set_get()
{
	Vector vec;
	vector_init(&vec, FALSE, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	vector_push_back_many(&vec, arr1, 4);
	vector_push_back_many(&vec, arr2, 4);
	vector_push_back_many(&vec, arr3, 4);
	vector_push_back_many(&vec, arr4, 4);

	vector_set(&vec, 3, get_ptr(u32, 3));
	u32 val = vector_get_unsafe(&vec, u32, 3);
	assert(val != 40);
	assert(val == 3);
	assert(vec.len == 16);

	vector_set(&vec, 4, get_ptr(u32, 922));
	val = vector_get_unsafe(&vec, u32, 4);
	assert(val != 11);
	assert(val == 922);
	assert(vec.len == 16);

	vector_set(&vec, 5, get_ptr(u32, 777));
	val = vector_get_unsafe(&vec, u32, 5);
	assert(val != 40);
	assert(val == 777);
	assert(vec.len == 16);

	vector_set(&vec, 6, get_ptr(u32, 13209));
	val = vector_get_unsafe(&vec, u32, 6);
	assert(val != 40);
	assert(val == 13209);
	assert(vec.len == 16);

	u32 val1 = vector_get_unsafe(&vec, u32, 10);
	u32 val2;
	vector_get(&vec, 10, &val2);
	assert(val1 == val2);

	val1 = vector_get_unsafe(&vec, u32, 13);
	vector_get(&vec, 13, &val2);
	assert(val1 == val2);

	val1 = vector_get_unsafe(&vec, u32, 14);
	vector_get(&vec, 14, &val2);
	assert(val1 == val2);

	assert(vector_get(&vec, 20, &val2) == FALSE);

	vector_purge(&vec, NULL);
}

void test_overwrite()
{
	Vector vec;
	vector_init(&vec, FALSE, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	vector_push_back_many(&vec, arr1, 4);
	vector_push_back_many(&vec, arr2, 4);
	vector_push_back_many(&vec, arr3, 4);
	vector_push_back_many(&vec, arr4, 4);

	u32 arr5[4] = { 95, 159, 32, 73 };

	vector_overwrite(&vec, 4, arr5, 4);

	u32 arr[16] = { 10, 20, 30, 40, 95, 159, 32, 73, 98, 88, 77, 45, 12, 55, 7, 2 };

	for (usize i = 0; i < 16; ++i) {
		u32 val = vector_get_unsafe(&vec, u32, i);
		assert(val == arr[i]);
	}

	vector_purge(&vec, NULL);
}

void test_steal()
{
	Vector vec;
	vector_init(&vec, FALSE, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	vector_push_back_many(&vec, arr1, 4);
	vector_push_back_many(&vec, arr2, 4);
	vector_push_back_many(&vec, arr3, 4);
	vector_push_back_many(&vec, arr4, 4);

	u32 arr[16] = { 10, 20, 30, 40, 11, 22, 33, 44, 98, 88, 77, 45, 12, 55, 7, 2 };
	u32 arr_[16];

	vector_steal(&vec, arr_, NULL, TRUE);

	for (usize i = 0; i < 16; ++i) {
		assert(arr_[i] == arr[i]);
	}

	assert(vec.capacity == 1);
	assert(vec.len == 0);

	vector_push_back_many(&vec, arr1, 4);
	vector_push_back_many(&vec, arr2, 4);
	vector_push_back_many(&vec, arr3, 4);
	vector_push_back_many(&vec, arr4, 4);

	u32 *arr__;
	vector_steal0(&vec, &arr__, NULL, FALSE);

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
	Vector vec;
	vector_init(&vec, FALSE, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	vector_push_back_many(&vec, arr1, 4);
	vector_push_back_many(&vec, arr2, 4);
	vector_push_back_many(&vec, arr3, 4);
	vector_push_back_many(&vec, arr4, 4);

	vector_shrink(&vec);

	assert(vec.capacity == 16);
	assert(vec.len == 16);

	vector_purge(&vec, NULL);
}

void test_reserve()
{
	Vector vec;
	vector_init(&vec, FALSE, FALSE, 4);

	vector_reserve(&vec, 16);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	vector_push_back_many(&vec, arr1, 4);
	assert(vec.len == 4);
	assert(vec.capacity == 24);

	vector_push_back_many(&vec, arr2, 4);
	assert(vec.len == 8);
	assert(vec.capacity == 24);

	vector_push_back_many(&vec, arr3, 4);
	assert(vec.len == 12);
	assert(vec.capacity == 24);

	vector_push_back_many(&vec, arr4, 4);
	assert(vec.len == 16);
	assert(vec.capacity == 24);

	vector_purge(&vec, NULL);
}

void test_search()
{
	Vector vec;
	vector_init(&vec, FALSE, FALSE, 4);

	u32 arr1[4] = { 10, 20, 30, 40 };
	u32 arr2[4] = { 11, 22, 33, 44 };
	u32 arr3[4] = { 98, 88, 77, 45 };
	u32 arr4[4] = { 12, 55, 7, 2 };

	vector_push_back_many(&vec, arr1, 4);
	vector_push_back_many(&vec, arr2, 4);
	vector_push_back_many(&vec, arr3, 4);
	vector_push_back_many(&vec, arr4, 4);

	usize index;
	assert(vector_search(&vec, get_ptr(u32, 33), u32cmp, &index));
	assert(vector_get_unsafe(&vec, u32, index) == 33);

	assert(vector_search(&vec, get_ptr(u32, 12), u32cmp, &index));
	assert(vector_get_unsafe(&vec, u32, index) == 12);

	assert(vector_search(&vec, get_ptr(u32, 110), u32cmp, NULL) == FALSE);

	vector_purge(&vec, NULL);
}

int main(int argc, char *argv[])
{
	test_push_purge();
	test_insert();
	test_insert_many();
	test_pop_remove();
	test_zero();
	test_sort_reverse();
	test_set_get();
	test_steal();
	test_shrink();
	test_reserve();
	test_search();
	test_overwrite();

	return 0;
}
