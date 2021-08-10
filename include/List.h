#ifndef LIST_H_L7SVYUA4
#define LIST_H_L7SVYUA4

#include "Types.h"
#include "Macros.h"

typedef struct _List List;

struct _List
{
	List *next;
	List *prev;
};

/**
 * @brief Adds a new entry onto the end of a list
 *
 * @param list list where to add the node
 * @param node entry to be added
 */
void list_append(List *list, List *node);

/**
 * @brief Adds a new entry onto the start of a list
 *
 * @param list list to add the node
 * @param node entry to be added
 */
void list_prepend(List *list, List *node);

/**
 * @brief Reverses a list
 *
 * @param list list to be reversed
 */
void list_reverse(List *list);

/**
 * @brief Sorts a list
 *
 * @param list list to be sorted
 * @param cmp_func compare function to sort entries
 */
void list_sort(List *list, CmpFunc cmp_func);

/**
 * @brief Swap two nodes
 *
 * @param a first node to be swaped
 * @param b second node to be swaped
 */
void list_swap(List *a, List *b);

/**
 * @brief Inserts one node after other node
 *
 * @param sibling node where to insert the other node after
 * @param node node to be inserted
 */
void list_insert_after(List *sibling, List *node);

/**
 * @brief Inserts one node after other node
 *
 * @param sibling node where to insert the other node before
 * @param node node to be inserted
 */
void list_insert_before(List *sibling, List *node);

/**
 * @brief Removes node from a list
 *
 * @param node node to be removed
 */
void list_remove(List *node);

/**
 * @brief Removes last node from a list
 *
 * @param list list that will lose his last node
 *
 * @return node that was removed
 */
List* list_pop(List *list);

/**
 * @brief Checks whether list is empty or not
 *
 * @param list list to be checked
 *
 * @return true if empty, false if not
 */
bool list_empty(const List *list);

/**
 * @brief Joins two lists
 *
 * @param list list to be added
 * @param node the place where list will be added
 */
void list_splice(List *list, List *node);

/**
 * @brief Copies a list
 *
 * @param dst list to copy
 * @param src list to be copied
 * @param copy_func function for copying nodes
 *
 * @return 0 on success
 */
int list_copy(List *dst, const List *src, CopyFunc copy_func);

/**
 * @brief Frees all nodes in a list
 *
 * @param list list to free
 * @param free_func function to free nodes
 */
void list_purge(List *list, FreeFunc free_func);

/* Inits node */
#define list_node_init(node) do {                     \
		(node)->next = (node); (node)->prev = (node); \
	} while (0)

/* Inits head */
#define list_head_init(head) list_node_init(head)

/* Inits list */
#define list_init(name) \
	List name = { &(name), &(name) }

/* Gets the struct for this entry */
#define list_entry(node, type, member) \
	container_of(node, type, member)

/* Iterates over a list */
#define list_foreach(iter, list) \
	for (iter = (list)->next; iter != (list); iter = iter->next)

/* Iterates over a list backwards */
#define list_foreach_reverse(iter, list) \
	for (iter = (list)->prev; iter != (list); iter = iter->prev)

#endif /* end of include guard: LIST_H_L7SVYUA4 */
