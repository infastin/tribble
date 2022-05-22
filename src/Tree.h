#ifndef TREE_H_HI5LCNKY
#define TREE_H_HI5LCNKY

#include "Macros.h"
#include "Types.h"

typedef struct _TrbTreeNode TrbTreeNode;
typedef struct _TrbTree TrbTree;

/**
 * TrbTreeNode:
 * @parent: The parent of the node.
 * @left: The left child of the node.
 * @right: The right child of the node.
 * @height: The height of the node.
 *
 * A node in a #TrbTree.
 **/
struct _TrbTreeNode {
	TrbTreeNode *parent;
	TrbTreeNode *left;
	TrbTreeNode *right;
	usize height;
};

/**
 * TrbTree:
 * @root: The root of the tree.
 * @cmp_func: The function for comparing nodes.
 * @cmpd_func: The function for comparing nodes using user data.
 * @data: User data.
 * @with_data: Indicates whether #TrbTree has been initialized with data or not.
 *
 * A self-balancing AVL tree.
 **/
struct _TrbTree {
	TrbTreeNode *root;

	union {
		TrbCmpFunc cmp_func;
		TrbCmpDataFunc cmpd_func;
	};

	void *data;
	bool with_data;
};

/**
 * trb_tree_init:
 * @self: (nullable): The pointer to the tree to be initialized.
 * @cmp_func: The function for comparing nodes.
 *
 * Creates a new #TrbTree.
 *
 * Returns: (nullable): A new #TrbTree.
 * Can return %NULL if an allocation error occurs.
 **/
TrbTree *trb_tree_init(TrbTree *self, TrbCmpFunc cmp_func);

/**
 * trb_tree_init_data:
 * @self: (nullable): The pointer to the tree to be initialized.
 * @cmpd_func: The function for comparing nodes using user data.
 * @data: User data.
 *
 * Creates a new #TrbTree with the comparison function that accepts user data.
 *
 * Returns: (nullable): A new #TrbTree.
 * Can return %NULL if an allocation error occurs.
 **/
TrbTree *trb_tree_init_data(TrbTree *self, TrbCmpDataFunc cmpd_func, void *data);

/**
 * trb_tree_insert:
 * @self: The tree where to insert the node.
 * @node: The node to be inserted.
 *
 * Inserts the node into the tree.
 *
 * Returns: %TRUE on success.
 **/
bool trb_tree_insert(TrbTree *self, TrbTreeNode *node);

/**
 * trb_tree_remove:
 * @self: The tree where to remove the node.
 * @node: The node to be removed.
 *
 * Removes the node from the tree.
 **/
void trb_tree_remove(TrbTree *self, TrbTreeNode *node);

/**
 * trb_tree_lookup:
 * @self: The tree where to search for the node.
 * @node: The similar node for comparison.
 *
 * Searches for the element in the tree.
 *
 * Returns: (nullable): The pointer to the node,
 * or %NULL if it isn't in the tree.
 **/
TrbTreeNode *trb_tree_lookup(const TrbTree *self, const TrbTreeNode *node);

/**
 * trb_tree_inorder:
 * @self: The tree to be traversed.
 * @func: (scope call): The function to call for each visited node.
 * @userdata: The data to pass to the function.
 *
 * Traverses the tree. It is inorder traversal.
 **/
void trb_tree_inorder(TrbTree *self, TrbUserFunc func, void *userdata);

/**
 * trb_tree_preorder:
 * @self: The tree to be traversed.
 * @func: (scope call): The function to call for each visited node.
 * @userdata: The data to pass to the function.
 *
 * Traverses the tree. It is preoder traversal.
 **/
void trb_tree_preorder(TrbTree *self, TrbUserFunc func, void *userdata);

/**
 * trb_tree_postorder:
 * @self: The tree to be traversed.
 * @func: (scope call): The function to call for each visited node.
 * @userdata: The data to pass to the function.
 *
 * Traverses the tree. It is postorder traversal.
 **/
void trb_tree_postorder(TrbTree *self, TrbUserFunc func, void *userdata);

/**
 * trb_tree_copy:
 * @dst: (optional) (inout): The pointer to the destination tree.
 * @src: The pointer to the source tree.
 * @copy_func: (scope call): The function for copying nodes.
 * @status: (optional) (out): The pointer to retrieve the status of execution (%TRUE for success).
 *
 * Creates a copy of the tree.
 *
 * Returns: (nullable): A copy of the tree.
 * Can return %NULL if an allocation error occurs.
 **/
TrbTree *trb_tree_copy(TrbTree *dst, const TrbTree *src, TrbCopyFunc copy_func, bool *status);

/**
 * trb_tree_destroy:
 * @self: The tree which nodes are to be freed.
 * @free_func: (scope call) (nullable): The function for freeing nodes.
 *
 * Frees all nodes in the tree.
 **/
void trb_tree_destroy(TrbTree *self, TrbFreeFunc free_func);

/**
 * trb_tree_free:
 * @self: The tree to be freed.
 * @free_func: (scope call) (nullable): The function for freeing nodes.
 *
 * Frees all nodes in the tree and the tree itself.
 **/
void trb_tree_free(TrbTree *self, TrbFreeFunc free_func);

/* Inits the node */
#define trb_tree_node_init(node) \
	do {                         \
		(node)->parent = NULL;   \
		(node)->left = NULL;     \
		(node)->right = NULL;    \
		(node)->height = 1;      \
	} while (0)

/* Gets the struct for this entry */
#define trb_tree_node_entry(node, type, member) \
	trb_container_of(node, type, member)

#endif /* end of include guard: TREE_H_HI5LCNKY */
