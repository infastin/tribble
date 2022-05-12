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
 * tree_init:
 * @tree: The pointer to the tree to be initialized (can be `NULL`).
 * @cmp_func: The function for comparing nodes.
 *
 * Creates a new tree.
 *
 * Returns: A new tree.
 **/
Tree *tree_init(Tree *tree, CmpFunc cmp_func);

/**
 * tree_insert:
 * @tree: The tree where to insert the node.
 * @node: The node to be inserted.
 *
 * Inserts the node into the tree.
 *
 * Returns: `TRUE` on success.
 **/
bool tree_insert(Tree *tree, TreeNode *node);

/**
 * tree_remove:
 * @tree: The tree where to remove the node.
 * @node: The node to be removed.
 *
 * Removes the node from the tree.
 **/
void tree_remove(Tree *tree, TreeNode *node);

/**
 * tree_lookup:
 * @tree: The tree where to search for the node.
 * @node: The similar node for comparison (can be `NULL`).
 *
 * Searches for the element in the tree.
 *
 * Returns: pointer The element, or `NULL` if it isn't in the tree.
 **/
TreeNode *tree_lookup(const Tree *tree, const TreeNode *node);

/**
 * tree_inorder:
 * @tree: The tree to be traversed.
 * @func: The function to call for each visited node.
 * @userdata: The data to pass to the function (can be `NULL`).
 *
 * Traverses the tree. It is inorder traversal.
 **/
void tree_inorder(Tree *tree, UserFunc func, void *userdata);

/**
 * tree_preorder:
 * @tree: The tree to be traversed.
 * @func: The function to call for each visited node.
 * @userdata: The data to pass to the function (can be `NULL`).
 *
 * Traverses the tree. It is preoder traversal.
 **/
void tree_preorder(Tree *tree, UserFunc func, void *userdata);

/**
 * tree_postorder:
 * @tree: The tree to be traversed.
 * @func: The function to call for each visited node.
 * @userdata: The data to pass to the function (can be `NULL`).
 *
 * Traverses the tree. It is postorder traversal.
 **/
void tree_postorder(Tree *tree, UserFunc func, void *userdata);

/**
 * tree_copy:
 * @dst: The pointer to the destination tree (can be `NULL`).
 * @src: The pointer to the source tree.
 * @copy_func: The function for copying nodes.
 * @status: The pointer to retrieve the status
 * 			of execution (`TRUE` for success, can be `NULL`).
 *
 * Creates a copy of the tree.
 *
 * Returns: A copy of the tree.
 **/
Tree *tree_copy(Tree *dst, const Tree *src, CopyFunc copy_func, bool *status);

/**
 * tree_destroy:
 * @tree: The tree which nodes are to be freed.
 * @free_func: The function for freeing nodes.
 *
 * Frees all nodes in the tree.
 **/
void tree_destroy(Tree *tree, FreeFunc free_func);

/**
 * tree_free:
 * @tree: The tree to be freed.
 * @free_func: The function for freeing nodes.
 *
 * Frees all nodes in the tree and the tree itself.
 **/
void tree_free(Tree *tree, FreeFunc free_func);

/* Inits the node */
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
