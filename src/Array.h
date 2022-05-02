#ifndef ARRAY_H_KSABYJ3T
#define ARRAY_H_KSABYJ3T

#include "Types.h"

#include <stdint.h>

typedef struct _Array Array;

struct _Array {
	/* Public */
	void *mass;
	uint32_t len;

	/* Private */
	uint32_t capacity;
	uint32_t elemsize;
	uint32_t zero_terminated : 1;
	uint32_t sorted : 1;
	uint32_t clear : 1;
};

/**
 * @brief Creates a new array
 *
 * @param arr The pointer to the array to be initialized (can be NULL)
 * @param clear TRUE if elements should be cleared to 0 when allocated
 * @param zero_terminated TRUE if array should have terminating zero
 * @param elemsize The size of each element in bytes
 *
 * @return A new array
 */
Array *array_init(Array *arr, bool clear, bool zero_terminated, uint32_t elemsize);

/**
 * @brief Creates the copy of the array
 *
 * @param dst The pointer to the array to copy (can be NULL)
 * @param src The array to be copied
 *
 * @return The copy of the array
 */
Array *array_copy(Array *dst, const Array *src);

/**
 * @brief Adds the element onto the end of the array
 *
 * @param arr The array where to add the element
 * @param data The pointer to the data to be added
 *
 * @return TRUE on success
 */
bool array_push_back(Array *arr, const void *data);

/**
 * @brief Adds elements onto the end of the array
 *
 * @param arr The array where to add elements
 * @param data The pointer to the elements to be added
 * @param len The number of elements to be added
 *
 * @return TRUE on success
 */
bool array_push_back_many(Array *arr, const void *data, uint32_t len);

/**
 * @brief Adds the element onto the start of the array
 *
 * @param arr The array where to add the element
 * @param data The pointer to the data to be added
 *
 * @return TRUE on success
 */
bool array_push_front(Array *arr, const void *data);

/**
 * @brief Adds elements onto the start of the array
 *
 * @param arr The array where to add elements
 * @param data The pointer to elements to be added
 * @param len The number of elements to be added
 *
 * @return TRUE on success
 */
bool array_push_front_many(Array *arr, const void *data, uint32_t len);

/**
 * @brief Inserts the element to the array at the given index
 *
 * @param arr The array where to insert
 * @param index The position to place the element at
 * @param data The pointer to the data to be inserted
 *
 * @return TRUE on success
 */
bool array_insert(Array *arr, uint32_t index, const void *data);

/**
 * @brief Inserts elements to the array at the given index
 *
 * @param arr The array where to insert
 * @param index The position to place elements at
 * @param data The pointer to elements to be inserted
 * @param len The number of elements to be inserted
 *
 * @return TRUE on success
 */
bool array_insert_many(Array *arr, uint32_t index, const void *data, uint32_t len);

/**
 * @brief Changes the value of the entry at the given index in the array
 *        with the given data
 *
 * @param arr The array where to change
 * @param index The position of the entry in which to change the value
 * @param data The data to be set
 *
 * @return TRUE on success
 */
bool array_set(Array *arr, uint32_t index, const void *data);

/**
 * @brief Removes the entry at the given index from the array
 *
 * @param arr The array where to remove
 * @param index The position of the entry to be removed
 * @param ret The pointer to retrieve removed data (can be NULL)
 *
 * @return TRUE on success
 */
bool array_remove_index(Array *arr, uint32_t index, void *ret);

/**
 * @brief Removes the last element from the array
 *
 * @param arr The array where to remove
 * @param ret The pointer to retrieve removed data (can be NULL)
 *
 * @return TRUE on success
 */
bool array_pop_back(Array *arr, void *ret);

/**
 * @brief Removes the first element from the array
 *
 * @param arr The array where to remove
 * @param ret The pointer to retrieve removed data (can be NULL)
 *
 * @return TRUE on success
 */
bool array_pop_front(Array *arr, void *ret);

/**
 * @brief Reverses the array
 *
 * @param arr The array to be reversed
 */
void array_reverse(Array *arr);

/**
 * @brief Removes the range from the array
 *
 * @param arr The arr where to remove
 * @param index The index of the first element to be removed
 * @param len The number of elements to be removed
 * @param ret The pointer to retrieve removed data (can be NULL)
 *
 * @return TRUE on success
 */
bool array_remove_range(Array *arr, uint32_t index, uint32_t len, void *ret);

/**
 * @brief Removes all elements from the array
 *
 * @param arr The arr where to remove
 * @param ret The pointer to retrieve removed data (can be NULL)
 *
 * @return TRUE on success
 */
bool array_remove_all(Array *arr, void *ret);

/**
 * @brief Sorts the array
 *
 * @param arr The array to be sorted
 * @param cmp_func The compare function for sorting entries
 */
void array_sort(Array *arr, CmpFunc cmp_func);

/**
 * @brief Gets the value of the entry at the given index in array
 *
 * @param arr The array where to get
 * @param index The position of the entry which value is to be got
 * @param ret The pointer to retrieve value of the entry
 *
 * @return TRUE on success
 */
bool array_get(const Array *arr, uint32_t index, void *ret);

/**
 * @brief Steals the array buffer.
 *        Array creates a new buffer.
 *
 * @param arr The array where to steal the buffer
 * @param ret The pointer to retrieve the buffer
 * @param len The pointer to retrieve the number of elements in the array (can be NULL)
 * @param to_copy if TRUE, the array buffer will be copied to ret and then recreated,
 *                otherwise pointer to the buffer will be returned
 *
 * @return TRUE on success
 */
bool array_steal(Array *arr, void *ret, uint32_t *len, bool to_copy);

/**
 * @brief Steals the array buffer.
 *        Array's buffer becomes NULL.
 *
 * @param arr The array where to steal buffer
 * @param ret The pointer to retrieve the buffer
 * @param len The pointer to retrieve the number of the elements in the array (can be NULL)
 * @param to_copy if TRUE, the array buffer will be copied to ret and then recreated,
 *                otherwise pointer to the buffer will be returned
 *
 * @return TRUE on success
 */
bool array_steal0(Array *arr, void *ret, uint32_t *len, bool to_copy);

/**
 * @brief Frees the array buffer.
 *        Elements itself won't be freed.
 *
 * @param arr The array which buffer is to be freed
 */
void array_purge(Array *arr);

/**
 * @brief Frees the array completely.
 *        Elements itself won't be freed.
 *
 * @param arr The array to be deleted
 */
void array_free(Array *arr);

/**
 * @brief Finds the entry in the array
 *
 * @param arr The array where to search
 * @param target The pointer to the data to be found
 * @param cmp_func The function for comparing values
 * @param index The pointer to retrieve the index of found value (can be NULL)
 *
 * @return TRUE if found, FALSE if not
 */
bool array_search(Array *arr, const void *target, CmpFunc cmp_func, uint32_t *index);

#endif /* end of include guard: ARRAY_H_KSABYJ3T */
