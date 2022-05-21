#ifndef LIST_H_L7SVYUA4
#define LIST_H_L7SVYUA4

#include "Macros.h"
#include "Types.h"

typedef struct _TrbList TrbList;

/**
 * TrbList:
 * @next: The next element in the list.
 * @prev: The previous element in the list.
 *
 * A circular doubly-linked list.
 **/
struct _TrbList {
	TrbList *next;
	TrbList *prev;
};

/**
 * trb_list_init:
 * @self: The pointer to the list to be initialized.
 *
 * Creates a new doubly-linked list.
 *
 * Returns: A new doubly-linked list. Can return %NULL if an allocation error occurs.
 **/
TrbList *trb_list_init(TrbList *self);

/**
 * trb_list_push_back:
 * @self: The list where to add the node.
 * @node: The entry to be added.
 *
 * Adds a new entry onto the end of the list.
 **/
void trb_list_push_back(TrbList *self, TrbList *node);

/**
 * trb_list_push_front:
 * @self: The list to add the node.
 * @node: The entry to be added.
 *
 * Adds a new entry onto the beginning of the list.
 **/
void trb_list_push_front(TrbList *self, TrbList *node);

/**
 * trb_list_reverse:
 * @self: The list to be reversed.
 *
 * Reverses the list.
 **/
void trb_list_reverse(TrbList *self);

/**
 * trb_list_sort:
 * @self: The list to be sorted.
 * @cmp_func: The comparison function for sorting entries.
 *
 * Sorts the list.
 **/
void trb_list_sort(TrbList *self, TrbCmpFunc cmp_func);

/**
 * trb_list_sort_data:
 * @self: The list to be sorted.
 * @cmpd_func: The comparison function for sorting entries using user data.
 * @data: User data.
 *
 * Sorts the list using user data.
 **/
void trb_list_sort_data(TrbList *self, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_list_nth:
 * @self: The list with the element.
 * @n: The position of the element.
 *
 * Gets the element at the given position.
 *
 * Returns: The element, or %NULL if position is beyond the end of the list.
 **/
TrbList *trb_list_nth(TrbList *self, usize n);

/**
 * trb_list_position:
 * @self: The list with the element.
 * @node: The element in the list.
 *
 * Gets the position of the element in the list.
 *
 * Returns: The position of the element, or -1 if the element is not found.
 **/
usize trb_list_position(TrbList *self, TrbList *node);

/**
 * trb_list_lookup:
 * @self: The list with the element.
 * @node: The similar node for comparison.
 * @cmp_func: The function for comparing elements.
 * @index: (optional) (out): The pointer to retrieve the position of the element.
 *
 * Searches for the element in the list.
 *
 * Returns: The element, or %NULL if it isn't in the list.
 **/
TrbList *trb_list_lookup(TrbList *self, const TrbList *node, TrbCmpFunc cmp_func, usize *index);

/**
 * trb_list_lookup_data:
 * @self: The list with the element.
 * @node: The similar node for comparison.
 * @cmpd_func: The function for comparing elements.
 * @data: User data.
 * @index: (optional) (out): The pointer to retrieve the position of the element.
 *
 * Searches for the element in the list using user data.
 *
 * Returns: The element, or %NULL if it isn't in the list.
 **/
TrbList *trb_list_lookup_data(TrbList *self, const TrbList *node, TrbCmpDataFunc cmpd_func, void *data, usize *index);

/**
 * trb_list_insert_after:
 * @sibling: The node where to insert the other node after.
 * @node: The node to be inserted.
 *
 * Inserts one node after other node.
 **/
void trb_list_insert_after(TrbList *sibling, TrbList *node);

/**
 * trb_list_insert_before:
 * @sibling: The node where to insert the other node before.
 * @node: The node to be inserted.
 *
 * Inserts one node after other node.
 **/
void trb_list_insert_before(TrbList *sibling, TrbList *node);

/**
 * trb_list_remove:
 * @node: The node to be removed.
 *
 * Removes the node from a list.
 **/
void trb_list_remove(TrbList *node);

/**
 * trb_list_pop_back:
 * @self: The list that will lose its last node.
 *
 * Removes the last node from the list.
 *
 * Returns: The node that was removed. Can return %NULL if the list is empty.
 **/
TrbList *trb_list_pop_back(TrbList *self);

/**
 * trb_list_pop_front:
 * @self: The list that will lose its first node.
 *
 * Removes the first node from the list.
 *
 * Returns: The node that was removed. Can return %NULL if the list is empty.
 **/
TrbList *trb_list_pop_front(TrbList *self);

/**
 * trb_list_empty:
 * @self: The list to be checked.
 *
 * Checks whether the list is empty or not.
 *
 * Returns: %TRUE if empty, %FALSE if not.
 **/
bool trb_list_empty(const TrbList *self);

/**
 * trb_list_len:
 * @self: The list with some/none elements.
 *
 * Returns the number of elements in the list.
 *
 * Returns: The number of elements in the list.
 **/
usize trb_list_len(const TrbList *self);

/**
 * trb_list_splice:
 * @self: The list to be transfered.
 * @node: The node after which the list will be inserted.
 *
 * Transfers elements from one list to another.
 **/
void trb_list_splice(TrbList *self, TrbList *node);

/**
 * trb_list_copy:
 * @dst: The destination list.
 * @src: The source list.
 * @copy_func: The function for copying nodes.
 * @status: (optional) (out): The pointer to retrieve the status of execution (%TRUE for success).
 *
 * Copies the list.
 *
 * Returns: A copy of the list. Can return %NULL if an allocation error occurs.
 **/
TrbList *trb_list_copy(TrbList *dst, const TrbList *src, TrbCopyFunc copy_func, bool *status);

/**
 * trb_list_destroy:
 * @self: The list to be freed.
 * @free_func: The function for freeing nodes.
 *
 * Frees all nodes in the list.
 **/
void trb_list_destroy(TrbList *self, TrbFreeFunc free_func);

/**
 * trb_list_free:
 * @self: The list to be freed.
 * @free_func: The function for freeing nodes.
 *
 * Frees the list completely.
 **/
void trb_list_free(TrbList *self, TrbFreeFunc free_func);

/* Inits the node */
#define trb_list_node_init(node) \
	do {                         \
		(node)->next = (node);   \
		(node)->prev = (node);   \
	} while (0)

/* Gets the struct for this entry */
#define trb_list_entry(node, type, member) \
	trb_container_of(node, type, member)

/* Iterates over the list */
#define trb_list_foreach(iter, list) \
	for (iter = (list)->next; iter != (list); iter = iter->next)

/* Iterates over the list backwards */
#define trb_list_foreach_reverse(iter, list) \
	for (iter = (list)->prev; iter != (list); iter = iter->prev)

#endif /* end of include guard: LIST_H_L7SVYUA4 */
