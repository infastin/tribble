#ifndef LIST_H_L7SVYUA4
#define LIST_H_L7SVYUA4

#include "Macros.h"
#include "Types.h"

typedef struct _List List;

struct _List {
	List *next;
	List *prev;
};

/**
 * @brief Adds a new entry onto the end of the list
 *
 * @param list The list where to add the node
 * @param node The entry to be added
 */
void list_push_back(List *list, List *node);

/**
 * @brief Adds a new entry onto the start of the list
 *
 * @param list The list to add the node
 * @param node The entry to be added
 */
void list_push_front(List *list, List *node);

/**
 * @brief Reverses the list
 *
 * @param list The list to be reversed
 */
void list_reverse(List *list);

/**
 * @brief Sorts the list
 *
 * @param list The list to be sorted
 * @param cmp_func The compare function to sort entries
 */
void list_sort(List *list, CmpFunc cmp_func);

/**
 * @brief Gets the element at the given position
 *
 * @param list The list with the element
 * @param n The position of the element
 *
 * @return The element, or NULL if position is beyond the end of the list
 */
List *list_nth(List *list, uint32_t n);

/**
 * @brief Gets the position of the element in the list
 *
 * @param list The list with the element
 * @param node The element in the list
 *
 * @return The position of the element, or -1 if the element is not found
 */
uint32_t list_position(List *list, List *node);

/**
 * @brief Inserts one node after other node
 *
 * @param sibling The node where to insert the other node after
 * @param node The node to be inserted
 */
void list_insert_after(List *sibling, List *node);

/**
 * @brief Inserts one node after other node
 *
 * @param sibling The node where to insert the other node before
 * @param node The node to be inserted
 */
void list_insert_before(List *sibling, List *node);

/**
 * @brief Removes the node from a list
 *
 * @param node The node to be removed
 */
void list_remove(List *node);

/**
 * @brief Removes the last node from the list
 *
 * @param list The list that will lose its last node
 *
 * @return The node that was removed
 */
List *list_pop_back(List *list);

/**
 * @brief Removes the first node from the list
 *
 * @param list The list that will lose its first node
 *
 * @return The node that was removed
 */
List *list_pop_front(List *list);

/**
 * @brief Checks whether the list is empty or not
 *
 * @param list The list to be checked
 *
 * @return TRUE if empty, FALSE if not
 */
bool list_empty(const List *list);

/**
 * @brief Returns the number of elements in the list
 *
 * @param list The list with some/none elements
 *
 * @return The number of elements in the list
 */
uint32_t list_len(const List *list);

/**
 * @brief Transfers elements from one list to another
 *
 * @param list The list to be transfered
 * @param node The node after which the list will be inserted
 */
void list_splice(List *list, List *node);

/**
 * @brief Copies the list
 *
 * @param dst The destination list
 * @param src The source list
 * @param copy_func The function for copying nodes
 *
 * @return 0 on success
 */
int32_t list_copy(List *dst, const List *src, CopyFunc copy_func);

/**
 * @brief Frees all nodes in the list
 *
 * @param list The list to free
 * @param free_func The function to free nodes
 */
void list_purge(List *list, FreeFunc free_func);

/* Inits a node */
#define list_node_init(node)   \
	do {                       \
		(node)->next = (node); \
		(node)->prev = (node); \
	} while (0)

/* Inits a head */
#define list_head_init(head) list_node_init(head)

/* Inits a list */
#define list_init(name) \
	List name = { &(name), &(name) }

/* Gets the struct for this entry */
#define list_entry(node, type, member) \
	container_of(node, type, member)

/* Iterates over the list */
#define list_foreach(iter, list) \
	for (iter = (list)->next; iter != (list); iter = iter->next)

/* Iterates over the list backwards */
#define list_foreach_reverse(iter, list) \
	for (iter = (list)->prev; iter != (list); iter = iter->prev)

#endif /* end of include guard: LIST_H_L7SVYUA4 */
