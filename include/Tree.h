#ifndef TREE_H_HI5LCNKY
#define TREE_H_HI5LCNKY

#include "Types.h"
#include "Macros.h"

typedef struct _TreeNode TreeNode;
typedef struct _Tree Tree;

struct _TreeNode
{
	/* Public */
	TreeNode *parent;
	TreeNode *left;
	TreeNode *right;

	/* Private */
	uint color : 1;
};

struct _Tree
{
	TreeNode *root;
	CmpFunc cmpf;
	CopyFunc cpyf;
};

/**
 * @brief Creates a new tree
 *
 * @param tree pointer to the tree to init (can be NULL)
 * @param cmp_func function for comparing nodes
 * @param copy_func function for copying nodes (can be NULL)
 *
 * @return new tree
 */
Tree* tree_init(Tree *tree, CmpFunc cmp_func, CopyFunc copy_func);

/**
 * @brief Inserts node into a tree
 *
 * @param tree tree to insert
 * @param node node to be inserted
 *
 * @return 0 on success
 */
int tree_insert(Tree *tree, TreeNode *node);

/**
 * @brief Swaps nodes in a tree
 *
 * @param tree tree to swap
 * @param a first node to be swapped
 * @param b second node to be swapped
 */
void tree_swap(Tree *tree, TreeNode *a, TreeNode *b);

/**
 * @brief Removes node from a tree
 *
 * @param tree tree to remove
 * @param node node to be removed
 */
void tree_remove(Tree *tree, TreeNode *node);

/**
 * @brief Finds a similar node in a tree
 *
 * @param tree tree to find
 * @param node node to be compared
 *
 * @return pointer to the similar node if found, NULL if not
 */
TreeNode* tree_lookup(const Tree *tree, const TreeNode *node);

/**
 * @brief Traverses a tree
 *
 * @param tree tree to be traversed
 * @param func function to call each visited node
 * @param userdata data to pass to the function
 */
void tree_traverse(Tree *tree, UserFunc func, void *userdata);

/**
 * @brief Creates copy of a tree
 *
 * @param dst pointer to the tree to copy (can be NULL)
 * @param src tree to be copied
 * @param err pointer to retrieve error codes (0 for success)
 *
 * @return copy of a tree
 */
Tree* tree_copy(Tree *dst, const Tree *src, int *err);

/**
 * @brief Frees all nodes in a tree
 *
 * @param tree tree to free
 * @param free_func function to free nodes
 */
void tree_purge(Tree *tree, FreeFunc free_func);

/* Inits node */
#define tree_node_init(node) do { \
		(node)->parent = NULL;    \
		(node)->left = NULL;      \
		(node)->right = NULL;     \
		(node)->color = 1;        \
	} while(0)

/* Gets the struct for this entry */
#define tree_node_entry(node, type, member) \
	container_of(node, type, member)

#endif /* end of include guard: TREE_H_HI5LCNKY */
