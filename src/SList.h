#ifndef SLIST_H_VRV3KNJL
#define SLIST_H_VRV3KNJL

#include "Macros.h"
#include "Types.h"

typedef struct _SList SList;

struct _SList {
	SList *next;
};

/**
 * slist_init:
 * @list: The pointer to the list to be initialized (can be `NULL`).
 *
 * Creates a new singly-linked list.
 *
 * Returns: A new singly-linked list. Can return `NULL` if an alloction error occurs.
 **/
SList *slist_init(SList *list);

/**
 * slist_push_back:
 * @list: The list where to add the node.
 * @node: The entry to be added.
 *
 * Adds a new entry onto the end of the list.
 **/
void slist_push_back(SList *list, SList *node);

/**
 * slist_push_front:
 * @list: The list to add the node.
 * @node: The entry to be added.
 *
 * Adds a new entry onto the beginning of the list.
 **/
void slist_push_front(SList *list, SList *node);

/**
 * slist_empty:
 * @list: The list to be checked.
 *
 * Checks whether the list is empty or not.
 *
 * Returns: `TRUE` if empty, `FALSE` if not.
 **/
bool slist_empty(const SList *list);

/**
 * slist_len:
 * @list: The list with some/none elements.
 *
 * Returns the number of elements in the list.
 *
 * Returns: The number of elements in the list.
 **/
usize slist_len(const SList *list);

/**
 * slist_reverse:
 * @list: The list to be reversed.
 *
 * Reverses the list.
 **/
void slist_reverse(SList *list);

/**
 * slist_sort:
 * @list: The list to be sorted.
 * @cmp_func: The compare function for sorting entries.
 *
 * Sorts the list.
 **/
void slist_sort(SList *list, CmpFunc cmp_func);

/**
 * slist_nth:
 * @list: The list with the element.
 * @n: The position of the element.
 *
 * Gets the element at the given position.
 *
 * Returns: The element, or `NULL` if position is beyond the end of the list.
 **/
SList *slist_nth(SList *list, usize n);

/**
 * slist_position:
 * @list: The list with the element.
 * @node: The element in the list.
 *
 * Gets the position of the element in the list.
 *
 * Returns: The position of the element, or -1 if the element is not found.
 **/
usize slist_position(SList *list, SList *node);

/**
 * slist_position:
 * @list: The list with the element.
 * @node: The similar node for comparison (can be `NULL`).
 * @cmp_func: The function for comparing elements.
 * @index: The pointer to retrieve the position of the element (can be `NULL`).
 *
 * Searches for the element in the list.
 *
 * Returns: The element, or `NULL` if it isn't in the list.
 **/
SList *slist_lookup(SList *list, const SList *node, CmpFunc cmp_func, usize *index);

/**
 * slist_insert_after:
 * @sibling: The node where to insert the other node after.
 * @node: The node to be inserted.
 *
 * Inserts one node after other node.
 **/
void slist_insert_after(SList *sibling, SList *node);

/**
 * slist_insert_before:
 * @list: The list where the node is to be inserted.
 * @sibling: The node before which the other is to be inserted.
 * @node: The node to be inserted.
 *
 * Inserts one node after other node.
 **/
void slist_insert_before(SList *list, SList *sibling, SList *node);

/**
 * slist_remove:
 * @list: The list with the node.
 * @node: The node to be removed.
 *
 * Removes the node from the list.
 **/
void slist_remove(SList *list, SList *node);

/**
 * slist_pop_back:
 * @list: The list that will lose its last node.
 *
 * Removes the last node from the list.
 *
 * Returns: The node that was removed.
 *          Can return `NULL` if the list is empty.
 **/
SList *slist_pop_back(SList *list);

/**
 * slist_pop_front:
 * @list: The list that will lose its first node.
 *
 * Removes the first node from the list.
 *
 * Returns: The node that was removed.
 *          Can return `NULL` if the list is empty.
 **/
SList *slist_pop_front(SList *list);

/**
 * slist_splice:
 * @list: The list to be transfered.
 * @node: The node after which the list will be inserted.
 *
 * Transfers elements from one list to another.
 **/
void slist_splice(SList *list, SList *node);

/**
 * slist_copy:
 * @dst: The destination list (can be `NULL`).
 * @src: The source list.
 * @copy_func: The function for copying nodes.
 * @status: The pointer to retrieve the status
 * 			of execution (`TRUE` for success, can be `NULL`).
 *
 * Copies the list.
 *
 * Returns: A copy of the list. Can return `NULL` if an allocation error occurs.
 **/
SList *slist_copy(SList *dst, const SList *src, CopyFunc copy_func, bool *status);

/**
 * slist_destroy:
 * @list: The list to be freed.
 * @free_func: The function for freeing nodes.
 *
 * Frees all nodes in the list.
 **/
void slist_destroy(SList *list, FreeFunc free_func);

/**
 * slist_free:
 * @vec: The list to be freed.
 * @free_func: The function for freeing nodes.
 *
 * Frees the list completely.
 **/
void slist_free(SList *list, FreeFunc free_func);

/* Inits the node */
#define slist_node_init(node)  \
	do {                       \
		(node)->next = (node); \
	} while (0)

/* Gets the struct for this entry */
#define slist_entry(node, type, member) \
	container_of(node, type, member)

/* Iterates over the list */
#define slist_foreach(iter, list) \
	for (iter = (list)->next; iter != (list); iter = iter->next)

#endif /* end of include guard: SLIST_H_VRV3KNJL */
