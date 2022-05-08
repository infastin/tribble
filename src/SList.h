#ifndef SLIST_H_VRV3KNJL
#define SLIST_H_VRV3KNJL

#include "Macros.h"
#include "Types.h"

typedef struct _SList SList;

struct _SList {
	SList *next;
};

/**
 * @brief Creates a new singly-linked list
 *
 * @param list The pointer to the list to be initialized (can be NULL)
 *
 * @return A new singly-linked list
 */
SList *slist_init(SList *list);

/**
 * @brief Adds a new entry onto the end of the list.
 *
 * @param list The list where to add the node.
 * @param node The entry to be added.
 */
void slist_push_back(SList *list, SList *node);

/**
 * @brief Adds a new entry onto the start of the list.
 *
 * @param list The list to add the node.
 * @param node The entry to be added.
 */
void slist_push_front(SList *list, SList *node);

/**
 * @brief Checks whether the list is empty or not.
 *
 * @param list The list to be checked.
 *
 * @return TRUE if empty, FALSE if not.
 */
bool slist_empty(const SList *list);

/**
 * @brief Returns the number of elements in the list.
 *
 * @param list The list with some/none elements.
 *
 * @return The number of elements in the list.
 */
usize slist_len(const SList *list);

/**
 * @brief Reverses the list.
 *
 * @param list The list to be reversed.
 */
void slist_reverse(SList *list);

/**
 * @brief Sorts the list.
 *
 * @param list The list to be sorted.
 * @param cmp_func The compare function for sorting entries.
 */
void slist_sort(SList *list, CmpFunc cmp_func);

/**
 * @brief Gets the element at the given position.
 *
 * @param list The list with the element.
 * @param n The position of the element.
 *
 * @return The element, or NULL if position is beyond the end of the list.
 */
SList *slist_nth(SList *list, usize n);

/**
 * @brief Gets the position of the element in the list.
 *
 * @param list The list with the element.
 * @param node The element in the list.
 *
 * @return The position of the element, or -1 if the element is not found.
 */
usize slist_position(SList *list, SList *node);

/**
 * @brief Finds the element in the list.
 *
 * @param list The list with the element.
 * @param node The similar node for comparison.
 * @param cmp_func The function for comparing elements.
 * @param index The pointer to retrieve the position of the element (can be NULL).
 *
 * @return The element, or NULL if it isn't in the list.
 */
SList *slist_lookup(SList *list, const SList *node, CmpFunc cmp_func, usize *index);

/**
 * @brief Inserts one node after other node.
 *
 * @param sibling The node where to insert the other node after.
 * @param node The node to be inserted.
 */
void slist_insert_after(SList *sibling, SList *node);

/**
 * @brief Inserts one node after other node.
 *
 * @param list The list where the node is to be inserted.
 * @param sibling The node before which the other is to be inserted.
 * @param node The node to be inserted.
 */
void slist_insert_before(SList *list, SList *sibling, SList *node);

/**
 * @brief Removes the node from the list.
 *
 * @param list The list with the node.
 * @param node The node to be removed.
 */
void slist_remove(SList *list, SList *node);

/**
 * @brief Removes the last node from the list.
 *
 * @param list The list that will lose its last node.
 *
 * @return The node that was removed.
 */
SList *slist_pop_back(SList *list);

/**
 * @brief Removes the first node from the list.
 *
 * @param list The list that will lose its first node.
 *
 * @return The node that was removed.
 */
SList *slist_pop_front(SList *list);

/**
 * @brief Transfers elements from one list to another.
 *
 * @param list The list to be transfered.
 * @param node The node after which the list will be inserted.
 */
void slist_splice(SList *list, SList *node);

/**
 * @brief Copies the list.
 *
 * @param dst The destination list.
 * @param src The source list.
 * @param copy_func The function for copying nodes.
 *
 * @return TRUE on success.
 */
bool slist_copy(SList *dst, const SList *src, CopyFunc copy_func);

/**
 * @brief Frees all nodes in the list.
 *
 * @param list The list to free.
 * @param free_func The function for freeing nodes.
 */
void slist_purge(SList *list, FreeFunc free_func);

/* Inits a node */
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
