#ifndef TREE_H_HI5LCNKY
#define TREE_H_HI5LCNKY

#include "Macros.h"
#include "Types.h"

typedef struct _TreeNode TreeNode;
typedef struct _Tree Tree;

struct _TreeNode {
	TreeNode *parent;
	TreeNode *left;
	TreeNode *right;
	isize height;
};

struct _Tree {
	TreeNode *root;
	CmpFunc cmp_func;
};

/**
 * @brief Creates a new tree.
 *
 * @param tree The pointer to the tree to be initialized (can be NULL).
 * @param cmp_func The function for comparing nodes.
 *
 * @return A new tree.
 */
Tree *tree_init(Tree *tree, CmpFunc cmp_func);

/**
 * @brief Inserts the node into the tree.
 *
 * @param tree The tree where to insert the node.
 * @param node The node to be inserted.
 *
 * @return TRUE on success.
 */
bool tree_insert(Tree *tree, TreeNode *node);

/**
 * @brief Removes the node from the tree.
 *
 * @param tree The tree where to remove the node.
 * @param node The node to be removed.
 */
void tree_remove(Tree *tree, TreeNode *node);

/**
 * @brief Finds the element in the tree.
 *
 * @param tree The tree where to search for the node.
 * @param node The similar node for comparison.
 *
 * @return pointer The element, or NULL if it isn't in the tree.
 */
TreeNode *tree_lookup(const Tree *tree, const TreeNode *node);

/**
 * @brief Traverses the tree. It is inorder traversal.
 *
 * @param tree The tree to be traversed.
 * @param func The function to call for each visited node.
 * @param userdata The data to pass to the function.
 */
void tree_inorder(Tree *tree, UserFunc func, void *userdata);

/**
 * @brief Traverses the tree. It is preoder traversal.
 *
 * @param tree The tree to be traversed.
 * @param func The function to call for each visited node.
 * @param userdata The data to pass to the function.
 */
void tree_preorder(Tree *tree, UserFunc func, void *userdata);

/**
 * @brief Traverses the tree. It is postorder traversal.
 *
 * @param tree The tree to be traversed.
 * @param func The function to call for each visited node.
 * @param userdata The data to pass to the function.
 */
void tree_postorder(Tree *tree, UserFunc func, void *userdata);

/**
 * @brief Creates a copy of the tree.
 *
 * @param dst The pointer to the destination tree (can be NULL).
 * @param src The pointer to the source tree.
 * @param status The pointer to retrieve the status (TRUE for success).
 * @param copy_func The function for copying nodes.
 *
 * @return A copy of the tree.
 */
Tree *tree_copy(Tree *dst, const Tree *src, CopyFunc copy_func, bool *status);

/**
 * @brief Frees all nodes in the tree.
 *
 * @param tree The tree which nodes are to be freed.
 * @param free_func The function for freeing nodes.
 */
void tree_purge(Tree *tree, FreeFunc free_func);

/**
 * @brief Frees all nodes in the tree and the tree itself.
 *
 * @param tree The tree to be freed.
 * @param free_func The function for freeing nodes.
 */
void tree_free(Tree *tree, FreeFunc free_func);

/* Inits node */
#define tree_node_init(node)   \
	do {                       \
		(node)->parent = NULL; \
		(node)->left = NULL;   \
		(node)->right = NULL;  \
		(node)->height = 1;    \
	} while (0)

/* Gets the struct for this entry */
#define tree_node_entry(node, type, member) \
	container_of(node, type, member)

#endif /* end of include guard: TREE_H_HI5LCNKY */
