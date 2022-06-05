#ifndef SLIST_H_VRV3KNJL
#define SLIST_H_VRV3KNJL

#include "trb-macros.h"
#include "trb-types.h"

typedef struct _TrbSList TrbSList;

/**
 * TrbSList:
 * @next: The next element in the list.
 *
 * A circular singly-linked list.
 **/
struct _TrbSList {
	TrbSList *next;
};

/**
 * trb_slist_init:
 * @self: (nullable): The pointer to the list to be initialized.
 *
 * Creates a new singly-linked list.
 *
 * Returns: (nullable): A new singly-linked list.
 * Can return %NULL if an alloction error occurs.
 **/
TrbSList *trb_slist_init(TrbSList *self);

/**
 * trb_slist_push_back:
 * @self: The list where to add the node.
 * @node: The entry to be added.
 *
 * Adds a new entry onto the end of the list.
 **/
void trb_slist_push_back(TrbSList *self, TrbSList *node);

/**
 * trb_slist_push_front:
 * @self: The list to add the node.
 * @node: The entry to be added.
 *
 * Adds a new entry onto the beginning of the list.
 **/
void trb_slist_push_front(TrbSList *self, TrbSList *node);

/**
 * trb_slist_empty:
 * @self: The list to be checked.
 *
 * Checks whether the list is empty or not.
 *
 * Returns: %TRUE if empty, %FALSE if not.
 **/
bool trb_slist_empty(const TrbSList *self);

/**
 * trb_slist_len:
 * @self: The list with some/none elements.
 *
 * Returns the number of elements in the list.
 *
 * Returns: The number of elements in the list.
 **/
usize trb_slist_len(const TrbSList *self);

/**
 * trb_slist_reverse:
 * @self: The list to be reversed.
 *
 * Reverses the list.
 **/
void trb_slist_reverse(TrbSList *self);

/**
 * trb_slist_sort:
 * @self: The list to be sorted.
 * @cmp_func: (scope call): The comparison function for sorting entries.
 *
 * Sorts the list.
 **/
void trb_slist_sort(TrbSList *self, TrbCmpFunc cmp_func);

/**
 * trb_slist_sort_data:
 * @self: The list to be sorted.
 * @cmpd_func: (scope call): The comparison function for sorting entries using user data.
 * @data: User data.
 *
 * Sorts the list using user data.
 **/
void trb_slist_sort_data(TrbSList *self, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_slist_nth:
 * @self: The list with the element.
 * @n: The position of the element.
 *
 * Gets the element at the given position.
 *
 * Returns: (nullable): The element,
 * or %NULL if position is beyond the end of the list.
 **/
TrbSList *trb_slist_nth(TrbSList *self, usize n);

/**
 * trb_slist_position:
 * @self: The list with the element.
 * @node: The element in the list.
 *
 * Gets the position of the element in the list.
 *
 * Returns: The position of the element, or -1 if the element is not found.
 **/
usize trb_slist_position(TrbSList *self, TrbSList *node);

/**
 * trb_slist_lookup:
 * @self: The list with the element.
 * @node: The similar node for comparison.
 * @cmp_func: (scope call): The function for comparing elements.
 * @index: (optional) (out): The pointer to retrieve the position of the element.
 *
 * Searches for the element in the list.
 *
 * Returns: (nullable): The element,
 * or %NULL if it isn't in the list.
 **/
TrbSList *trb_slist_lookup(TrbSList *self, const TrbSList *node, TrbCmpFunc cmp_func, usize *index);

/**
 * trb_slist_lookup_data:
 * @self: The list with the element.
 * @node: The similar node for comparison.
 * @cmpd_func: (scope call): The function for comparing elements.
 * @data: User data.
 * @index: (optional) (out): The pointer to retrieve the position of the element.
 *
 * Searches for the element in the list using user data.
 *
 * Returns: (nullable): The element,
 * or %NULL if it isn't in the list.
 **/
TrbSList *trb_slist_lookup_data(TrbSList *self, const TrbSList *node, TrbCmpDataFunc cmpd_func, void *data, usize *index);

/**
 * trb_slist_insert_after:
 * @sibling: The node where to insert the other node after.
 * @node: The node to be inserted.
 *
 * Inserts one node after other node.
 **/
void trb_slist_insert_after(TrbSList *sibling, TrbSList *node);

/**
 * trb_slist_insert_before:
 * @self: The list where the node is to be inserted.
 * @sibling: The node before which the other is to be inserted.
 * @node: The node to be inserted.
 *
 * Inserts one node after other node.
 **/
void trb_slist_insert_before(TrbSList *self, TrbSList *sibling, TrbSList *node);

/**
 * trb_slist_remove:
 * @self: The list with the node.
 * @node: The node to be removed.
 *
 * Removes the node from the list.
 **/
void trb_slist_remove(TrbSList *self, TrbSList *node);

/**
 * trb_slist_pop_back:
 * @self: The list that will lose its last node.
 *
 * Removes the last node from the list.
 *
 * Returns: The node that was removed.
 *          Can return %NULL if the list is empty.
 **/
TrbSList *trb_slist_pop_back(TrbSList *self);

/**
 * trb_slist_pop_front:
 * @self: The list that will lose its first node.
 *
 * Removes the first node from the list.
 *
 * Returns: The node that was removed.
 *          Can return %NULL if the list is empty.
 **/
TrbSList *trb_slist_pop_front(TrbSList *self);

/**
 * trb_slist_splice:
 * @self: The list to be transfered.
 * @node: The node after which the list will be inserted.
 *
 * Transfers elements from one list to another.
 **/
void trb_slist_splice(TrbSList *self, TrbSList *node);

/**
 * trb_slist_copy:
 * @src: The source list.
 * @dst: (optional) (inout): The destination list.
 * @copy_func: (scope call): The function for copying nodes.
 * @status: (optional) (out): The pointer to retrieve the status of execution (%TRUE for success).
 *
 * Copies the list.
 *
 * Returns: (nullable): A copy of the list.
 * Can return %NULL if an allocation error occurs.
 **/
TrbSList *trb_slist_copy(const TrbSList *src, TrbSList *dst, TrbCopyFunc copy_func, bool *status);

/**
 * trb_slist_destroy:
 * @self: The list to be freed.
 * @free_func: (scope call) (nullable): The function for freeing nodes.
 *
 * Frees all nodes in the list.
 **/
void trb_slist_destroy(TrbSList *self, TrbFreeFunc free_func);

/**
 * trb_slist_free:
 * @self: The list to be freed.
 * @free_func: (scope call) (nullable): The function for freeing nodes.
 *
 * Frees the list completely.
 **/
void trb_slist_free(TrbSList *self, TrbFreeFunc free_func);

/* Inits the node */
#define trb_slist_node_init(node) \
	do {                          \
		(node)->next = (node);    \
	} while (0)

/* Gets the struct for this entry */
#define trb_slist_entry(node, type, member) \
	trb_container_of(node, type, member)

/* Iterates over the list */
#define trb_slist_foreach(iter, list) \
	for (iter = (list)->next; iter != (list); iter = iter->next)

#endif /* end of include guard: SLIST_H_VRV3KNJL */
