#ifndef VECTOR_H_KSABYJ3T
#define VECTOR_H_KSABYJ3T

#include "Types.h"

#include <stdint.h>

typedef struct _Vector Vector;

struct _Vector {
	void *data;
	usize len;
	usize capacity;
	usize elemsize;
	usize zero_terminated : 1;
	usize sorted : 1;
	usize clear : 1;
};

/**
 * @brief Creates a new dynamic size array.
 *
 * @param vec The pointer to the array to be initialized (can be NULL).
 * @param clear TRUE if elements should be cleared to 0 when allocated.
 * @param zero_terminated TRUE if the array should have terminating zero.
 * @param elemsize The size of each element in bytes.
 *
 * @return A new dynamic size array.
 */
Vector *vector_init(Vector *vec, bool clear, bool zero_terminated, usize elemsize);

/**
 * @brief Creates the copy of the array.
 *
 * @param dst The pointer to the array to copy (can be NULL).
 * @param src The array to be copied.
 *
 * @return The copy of the array.
 */
Vector *vector_copy(Vector *dst, const Vector *src);

/**
 * @brief Adds the element onto the end of the array.
 *
 * @param vec The array where to add the element.
 * @param data The pointer to the data to be added.
 *
 * @return TRUE on success.
 */
bool vector_push_back(Vector *vec, const void *data);

/**
 * @brief Adds elements onto the end of the array.
 *
 * @param vec The array where to add elements.
 * @param data The pointer to the elements to be added.
 * @param len The number of elements to be added.
 *
 * @return TRUE on success.
 */
bool vector_push_back_many(Vector *vec, const void *data, usize len);

/**
 * @brief Adds the element onto the start of the array.
 *
 * @param vec The array where to add the element.
 * @param data The pointer to the data to be added.
 *
 * @return TRUE on success.
 */
bool vector_push_front(Vector *vec, const void *data);

/**
 * @brief Adds elements onto the start of the array.
 *
 * @param vec The array where to add elements.
 * @param data The pointer to elements to be added.
 * @param len The number of elements to be added.
 *
 * @return TRUE on success.
 */
bool vector_push_front_many(Vector *vec, const void *data, usize len);

/**
 * @brief Inserts the element to the array at the given index.
 *
 * @param vec The array where to insert.
 * @param index The position to place the element at.
 * @param data The pointer to the data to be inserted.
 *
 * @return TRUE on success.
 */
bool vector_insert(Vector *vec, usize index, const void *data);

/**
 * @brief Inserts elements to the array at the given index.
 *
 * @param vec The array where to insert.
 * @param index The position to place elements at.
 * @param data The pointer to elements to be inserted.
 * @param len The number of elements to be inserted.
 *
 * @return TRUE on success.
 */
bool vector_insert_many(Vector *vec, usize index, const void *data, usize len);

/**
 * @brief Changes the value of the entry at the given index in the array
 *        with the given data.
 *
 * @param vec The array where to change.
 * @param index The position of the entry in which to change the value.
 * @param data The data to be set.
 *
 * @return TRUE on success.
 */
bool vector_set(Vector *vec, usize index, const void *data);

/**
 * @brief Removes the entry at the given index from the array.
 *
 * @param vec The array where to remove.
 * @param index The position of the entry to be removed.
 * @param ret The pointer to retrieve removed data (can be NULL).
 *
 * @return TRUE on success.
 */
bool vector_remove_index(Vector *vec, usize index, void *ret);

/**
 * @brief Removes the last element from the array.
 *
 * @param vec The array where to remove.
 * @param ret The pointer to retrieve removed data (can be NULL).
 *
 * @return TRUE on success.
 */
bool vector_pop_back(Vector *vec, void *ret);

/**
 * @brief Removes the first element from the array.
 *
 * @param vec The array where to remove.
 * @param ret The pointer to retrieve removed data (can be NULL).
 *
 * @return TRUE on success.
 */
bool vector_pop_front(Vector *vec, void *ret);

/**
 * @brief Reverses the array.
 *
 * @param vec The array to be reversed.
 */
void vector_reverse(Vector *vec);

/**
 * @brief Removes the range from the array.
 *
 * @param vec The vec where to remove.
 * @param index The index of the first element to be removed.
 * @param len The number of elements to be removed.
 * @param ret The pointer to retrieve removed data (can be NULL).
 *
 * @return TRUE on success.
 */
bool vector_remove_range(Vector *vec, usize index, usize len, void *ret);

/**
 * @brief Removes all elements from the array.
 *
 * @param vec The vec where to remove.
 * @param ret The pointer to retrieve removed data (can be NULL).
 *
 * @return TRUE on success.
 */
bool vector_remove_all(Vector *vec, void *ret);

/**
 * @brief Sorts the array.
 *
 * @param vec The array to be sorted.
 * @param cmp_func The compare function for sorting entries.
 */
void vector_sort(Vector *vec, CmpFunc cmp_func);

/**
 * @brief Gets the value of the entry at the given index in array.
 *
 * @param vec The array where to get.
 * @param index The position of the entry which value is to be got.
 * @param ret The pointer to retrieve value of the entry.
 *
 * @return TRUE on success.
 */
bool vector_get(const Vector *vec, usize index, void *ret);

/**
 * @brief Steals the array buffer.
 *        Array creates a new buffer.
 *
 * @param vec The array where to steal the buffer.
 * @param ret The pointer to retrieve the buffer.
 * @param len The pointer to retrieve the number of elements in the array (can be NULL).
 * @param to_copy If TRUE, the array buffer will be copied to ret and then recreated,
 *                otherwise pointer to the buffer will be returned.
 *
 * @return TRUE on success.
 */
bool vector_steal(Vector *vec, void *ret, usize *len, bool to_copy);

/**
 * @brief Steals the array buffer.
 *        Array's buffer becomes NULL.
 *
 * @param vec The array where to steal buffer.
 * @param ret The pointer to retrieve the buffer.
 * @param len The pointer to retrieve the number of the elements in the array (can be NULL).
 * @param to_copy if TRUE, the array buffer will be copied to ret,
 *                otherwise pointer to the buffer will be returned.
 *
 * @return TRUE on success.
 */
bool vector_steal0(Vector *vec, void *ret, usize *len, bool to_copy);

/**
 * @brief Reserves memory in the array buffer.
 *
 * @param vec The array where memory is to be reserved.
 * @param newcap A new capacity of the array.
 *
 * @return TRUE on success.
 */
bool vector_reserve(Vector *vec, usize newcap);

/**
 * @brief Frees unused memory of the array buffer.
 *
 * @param vec The array which buffer is to shrunk.
 *
 * @return TRUE on success.
 */
bool vector_shrink(Vector *vec);

/**
 * @brief Frees the array buffer.
 *
 * @param vec The array which buffer is to be freed.
 * @param free_func The function for freeing elements (can be NULL).
 */
void vector_purge(Vector *vec, FreeFunc free_func);

/**
 * @brief Frees the array completely.
 *
 * @param vec The array to be deleted.
 * @param free_func The function for freeing elements (can be NULL).
 */
void vector_free(Vector *vec, FreeFunc free_func);

/**
 * @brief Finds the entry in the array.
 *
 * @param vec The array where to search.
 * @param target The pointer to the data to be found.
 * @param cmp_func The function for comparing values.
 * @param index The pointer to retrieve the index of found value (can be NULL).
 *
 * @return TRUE if found, FALSE if not.
 */
bool vector_search(Vector *vec, const void *target, CmpFunc cmp_func, usize *index);

/**
 * @brief Unsafe version of vector_get.
 * @param vec The array where to get.
 * @param type The type to be got.
 * @param index The position of the entry which value is to be got.
 * @return Value of the entry.
 */
#define vector_get_unsafe(vec, type, index) (*(type *) &((char *) ((vec)->data))[(index) * (vec)->elemsize])

#endif /* end of include guard: ARRAY_H_KSABYJ3T */
