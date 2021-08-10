#ifndef ARRAY_H_KSABYJ3T
#define ARRAY_H_KSABYJ3T

#include <stdint.h>

#include "Types.h"

typedef struct _Array Array;

struct _Array
{
	/* Public */
	void *mass;
	uint len;

	/* Private */
	uint private[3];
};

/**
 * @brief Creates a new array
 *
 * @param arr pointer to the array to init (can be NULL)
 * @param clear TRUE if elements should be cleared to 0 when allocated
 * @param zero_terminated TRUE if array should have terminating zero
 * @param elemsize size of each elemets in bytes
 *
 * @return new array
 */
Array* array_init(Array *arr, bool clear, bool zero_terminated, uint elemsize);

/**
 * @brief Creates copy of an array
 *
 * @param dst pointer to the array to copy (can be NULL)
 * @param src array to be copied
 *
 * @return copy of an array
 */
Array* array_copy(Array *dst, const Array *src);

/**
 * @brief Adds element onto the end of an array
 *
 * @param arr array to add
 * @param data pointer to the data to add
 *
 * @return 0 on success
 */
int array_append(Array *arr, const void *data);

/**
 * @brief Adds elements onto the end of an array
 *
 * @param arr array to add
 * @param data pointer to the elements to add
 * @param len number of elements to add
 *
 * @return 0 on success
 */
int array_append_many(Array *arr, const void *data, uint len);

/**
 * @brief Adds element onto the start of an array
 *
 * @param arr array to add
 * @param data pointer to data to add
 *
 * @return 0 on success
 */
int array_prepend(Array *arr, const void *data);

/**
 * @brief Adds elements onto the start of an array
 *
 * @param arr array to add
 * @param data pointer to the elements to add
 * @param len number of elements to add
 *
 * @return 0 on success
 */
int array_prepend_many(Array *arr, const void *data, uint len);

/**
 * @brief Inserts element to array at the given index
 *
 * @param arr array to insert
 * @param index index to place element at
 * @param data pointer to the data to insert
 *
 * @return 0 on success
 */
int array_insert(Array *arr, uint index, const void *data);

/**
 * @brief Inserts elements to array at the given index
 *
 * @param arr array to insert
 * @param index index to place elements at
 * @param data pointer to the elements to insert
 * @param len number of elements to insert
 *
 * @return 0 on success
 */
int array_insert_many(Array *arr, uint index, const void *data, uint len);

/**
 * @brief Sets value of entry at the given index in array
 *
 * @param arr array to set
 * @param index index of the entry to set value
 * @param data data to be set
 *
 * @return 0 on success
 */
int array_set(Array *arr, uint index, const void *data);

/**
 * @brief Removes entry at the given index from array
 *
 * @param arr array to remove
 * @param index index of the entry to be removed
 * @param ret pointer to retrieve removed data (can be NULL)
 *
 * @return 0 on success
 */
int array_remove_index(Array *arr, uint index, void *ret);

/**
 * @brief Removes last element from array
 *
 * @param arr array to remove
 * @param ret pointer to retrieve removed data (can be NULL)
 *
 * @return 0 on success
 */
int array_pop(Array *arr, void *ret);

/**
 * @brief Reverses array
 *
 * @param arr array to be reversed
 */
void array_reverse(Array *arr);

/**
 * @brief Removes range from array
 *
 * @param arr arr to remove
 * @param index index of the first element to remove
 * @param len number of elements to remove
 * @param ret pointer to retrieve removed data (can be NULL)
 *
 * @return 0 on success
 */
int array_remove_range(Array *arr, uint index, uint len, void *ret);

/**
 * @brief Sorts an array
 *
 * @param arr array to be sorted
 * @param cmp_func compare function for sorting entries
 */
void array_sort(Array *arr, CmpFunc cmp_func);

/**
 * @brief Gets value of entry at the given index in array
 *
 * @param arr array to get
 * @param index index of the entry to be got
 * @param ret pointer to retrieve value of the entry
 *
 * @return 0 on success
 */
int array_get(const Array *arr, uint index, void *ret);

/**
 * @brief Steals array buffer. Array creates new buffer.
 *
 * @param arr array to steal buffer
 * @param ret pointer to retrieve buffer
 * @param len pointer to retrieve number of elements in array (can be NULL)
 * @param to_copy if true, array buffer will be copied to ret and then recreated, 
 *                otherwise pointer to the buffer will be returned
 *
 * @return 0 on success
 */
int array_steal(Array *arr, void *ret, uint *len, bool to_copy);

/**
 * @brief Steals array buffer. Array's buffer becomes NULL.
 *
 * @param arr array to steal buffer
 * @param ret pointer to retrieve buffer
 * @param len pointer to retrieve number of elements in array (can be NULL)
 * @param to_copy if true, array buffer will be copied to ret and then recreated, 
 *                otherwise pointer to the buffer will be returned
 *
 * @return 0 on success
 */
int array_steal0(Array *arr, void *ret, uint *len, bool to_copy);

/**
 * @brief Frees buffer of an array
 *
 * @param arr array which buffer will be freed
 */
void array_purge(Array *arr);

/**
 * @brief Frees array completely
 *
 * @param arr array to be deleted
 */
void array_free(Array *arr);

/**
 * @brief Finds entry in array
 *
 * @param arr array to find
 * @param target pointer to the data to find
 * @param cmp_func function for comparing values
 * @param index pointer to retrieve index of found value (can be NULL)
 *
 * @return true if found, false if not
 */
bool array_search(Array *arr, const void *target, CmpFunc cmp_func, uint *index);

#endif /* end of include guard: ARRAY_H_KSABYJ3T */
