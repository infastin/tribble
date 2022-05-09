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
 * list_init:
 * @list: The pointer to the list to be initialized (can be `NULL`)
 *
 * Creates a new doubly-linked list
 *
 * Returns: A new doubly-linked list
 **/
List *list_init(List *list);

/**
 * list_push_back:
 * @list: The list where to add the node.
 * @node: The entry to be added.
 *
 * Adds a new entry onto the end of the list.
 **/
void list_push_back(List *list, List *node);

/**
 * list_push_front:
 * @list: The list to add the node.
 * @node: The entry to be added.
 *
 * Adds a new entry onto the beginning of the list.
 **/
void list_push_front(List *list, List *node);

/**
 * list_reverse:
 * @list: The list to be reversed.
 *
 * Reverses the list.
 **/
void list_reverse(List *list);

/**
 * list_sort:
 * @list: The list to be sorted.
 * @cmp_func: The compare function for sorting entries.
 *
 * Sorts the list.
 **/
void list_sort(List *list, CmpFunc cmp_func);

/**
 * list_nth:
 * @list: The list with the element.
 * @n: The position of the element.
 *
 * Gets the element at the given position.
 *
 * Returns: The element, or `NULL` if position is beyond the end of the list.
 **/
List *list_nth(List *list, usize n);

/**
 * list_position:
 * @list: The list with the element.
 * @node: The element in the list.
 *
 * Gets the position of the element in the list.
 *
 * Returns: The position of the element, or -1 if the element is not found.
 **/
usize list_position(List *list, List *node);

/**
 * list_lookup:
 * @list: The list with the element.
 * @node: The similar node for comparison.
 * @cmp_func: The function for comparing elements.
 * @index: The pointer to retrieve the position of the element (can be `NULL`).
 *
 * Searches for the element in the list.
 *
 * Returns: The element, or `NULL` if it isn't in the list.
 **/
List *list_lookup(List *list, const List *node, CmpFunc cmp_func, usize *index);

/**
 * list_insert_after:
 * @sibling: The node where to insert the other node after.
 * @node: The node to be inserted.
 *
 * Inserts one node after other node.
 **/
void list_insert_after(List *sibling, List *node);

/**
 * list_insert_before:
 * @sibling: The node where to insert the other node before.
 * @node: The node to be inserted.
 *
 * Inserts one node after other node.
 **/
void list_insert_before(List *sibling, List *node);

/**
 * list_remove:
 * @node: The node to be removed.
 *
 * Removes the node from a list.
 **/
void list_remove(List *node);

/**
 * list_pop_back:
 * @list: The list that will lose its last node.
 *
 * Removes the last node from the list.
 *
 * Returns: The node that was removed.
 **/
List *list_pop_back(List *list);

/**
 * list_pop_front:
 * @list: The list that will lose its first node.
 *
 * Removes the first node from the list.
 *
 * Returns: The node that was removed.
 **/
List *list_pop_front(List *list);

/**
 * list_empty:
 * @list: The list to be checked.
 *
 * Checks whether the list is empty or not.
 *
 * Returns: `TRUE` if empty, `FALSE` if not.
 **/
bool list_empty(const List *list);

/**
 * list_len:
 * @list: The list with some/none elements.
 *
 * Returns the number of elements in the list.
 *
 * Returns: The number of elements in the list.
 **/
usize list_len(const List *list);

/**
 * list_splice:
 * @list: The list to be transfered.
 * @node: The node after which the list will be inserted.
 *
 * Transfers elements from one list to another.
 **/
void list_splice(List *list, List *node);

/**
 * list_copy:
 * @dst: The destination list (can be `NULL`).
 * @src: The source list.
 * @copy_func: The function for copying nodes.
 * @status: The pointer to retrieve the status
 * 			of execution (`TRUE` for success, can be `NULL`).
 *
 * Copies the list.
 *
 * Returns: A copy of the list
 **/
List *list_copy(List *dst, const List *src, CopyFunc copy_func, bool *status);

/**
 * list_purge:
 * @list: The list to be freed.
 * @free_func: The function for freeing nodes.
 *
 * Frees all nodes in the list.
 **/
void list_purge(List *list, FreeFunc free_func);

/**
 * list_free:
 * @vec: The list to be freed.
 * @free_func: The function for freeing nodes.
 *
 * Frees the list completely.
 **/
void list_free(List *list, FreeFunc free_func);

/* Inits the node */
#define list_node_init(node)   \
	do {                       \
		(node)->next = (node); \
		(node)->prev = (node); \
	} while (0)

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
