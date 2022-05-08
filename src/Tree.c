#include "Tree.h"

#include "List.h"
#include "Macros.h"
#include "Messages.h"
#include "Types.h"

#include <malloc.h>

typedef enum {
	LEFT,
	RIGHT
} dir_t;

#define max(a, b) ((a) > (b) ? (a) : (b))
#define height(n) ((n) == NULL ? 0 : (n)->height)
#define update_height(n) ((n)->height = max(height((n)->left), height((n)->right)) + 1)
#define balance(n) ((n) == NULL ? 0 : height((n)->right) - height((n)->left))

Tree *tree_init(Tree *tree, CmpFunc cmp_func)
{
	return_val_if_fail(cmp_func != NULL, NULL);

	if (tree == NULL) {
		tree = talloc(Tree, 1);

		if (tree == NULL) {
			msg_error("couldn't allocate memory for the tree!");
			return NULL;
		}
	}

	tree->root = NULL;
	tree->cmp_func = cmp_func;

	return tree;
}

static void __tree_rotate(Tree *tree, TreeNode *n, dir_t dir)
{
	TreeNode *c;
	TreeNode *p = n->parent;

	if (dir == LEFT) {
		c = n->right;

		n->right = c->left;
		c->left = n;
		c->parent = p;
		n->parent = c;

		if (n->right != NULL)
			n->right->parent = n;
	} else {
		c = n->left;

		n->left = c->right;
		c->right = n;
		c->parent = p;
		n->parent = c;

		if (n->left != NULL)
			n->left->parent = n;
	}

	if (p != NULL) {
		if (n == p->left)
			p->left = c;
		else
			p->right = c;
	} else {
		tree->root = c;
	}
}

static void __tree_rebalance(Tree *tree, TreeNode *pivot, TreeNode *node)
{
	TreeNode *current = node;
	TreeNode *pivot_parent = (pivot == NULL) ? NULL : pivot->parent;

	while (current != pivot_parent) {
		current->height = max(height(current->left), height(current->right)) + 1;
		current = current->parent;
	}

	if (pivot == NULL || (balance(pivot) > -2 && balance(pivot) < 2))
		return;

	dir_t dir;      // the direction of the imbalance
	TreeNode *bad;  // the child of the pivot node in the direction of the imbalance

	if (balance(pivot) == -2) {
		dir = LEFT;
		bad = pivot->left;
	} else {
		dir = RIGHT;
		bad = pivot->right;
	}

	if ((balance(bad) < 0) != dir) {
		__tree_rotate(tree, pivot, RIGHT - dir);
	} else {
		__tree_rotate(tree, bad, dir);
		update_height(bad);
		__tree_rotate(tree, pivot, RIGHT - dir);
	}

	update_height(pivot);
	update_height(pivot->parent);
}

bool tree_insert(Tree *tree, TreeNode *node)
{
	return_val_if_fail(tree != NULL, FALSE);
	return_val_if_fail(node != NULL, FALSE);

	if (tree->root == NULL) {
		tree->root = node;
		return TRUE;
	}

	TreeNode *current = tree->root;
	TreeNode *pivot = NULL;

	while (1) {
		i32 cmp = tree->cmp_func(current, node);

		if (balance(current) != 0)
			pivot = current;

		if (cmp > 0) {
			if (current->left == NULL) {
				current->left = node;
				break;
			}

			current = current->left;
			continue;
		}

		if (cmp < 0) {
			if (current->right == NULL) {
				current->right = node;
				break;
			}

			current = current->right;
			continue;
		}

		msg_warn("found the same entry in the tree!");
		return FALSE;
	}

	node->parent = current;
	__tree_rebalance(tree, pivot, node->parent);

	return TRUE;
}

static TreeNode *__tree_remove_prepare(Tree *tree, TreeNode *node)
{
	if (node->left == NULL && node->right == NULL)
		return node;

	if (node->left == NULL)
		return node->right;

	if (node->right == NULL)
		return node->left;

	for (node = node->right; node->left != NULL; node = node->left)
		continue;

	return node;
}

void tree_remove(Tree *tree, TreeNode *node)
{
	return_if_fail(tree != NULL);
	return_if_fail(node != NULL);

	TreeNode *s = __tree_remove_prepare(tree, node);
	TreeNode *current = s->parent;
	TreeNode *pivot = NULL;

	while (current != NULL) {
		if (balance(current) != 0) {
			pivot = current;
			break;
		}

		current = current->parent;
	}

	if (pivot == node)
		pivot = s;

	TreeNode *a;

	if (node == s) {
		s = NULL;
		a = node->parent;
	} else if (
		(node->left == s && node->right == NULL) ||
		(node->right == s && node->left == NULL)
	) {
		s->parent = node->parent;
		a = s;
	} else {
		if (s->parent == node) {
			s->parent = node->parent;
			s->left = node->left;
			s->left->parent = s;
			a = s;
		} else {
			s->parent->left = s->right;
			if (s->right != NULL)
				s->right->parent = s->parent->left;
			a = s->parent;
			s->parent = node->parent;
			s->left = node->left;
			s->left->parent = s;
			s->right = node->right;
			s->right->parent = s;
		}
	}

	if (tree->root == node)
		tree->root = s;
	else if (node->parent->left == node)
		node->parent->left = s;
	else if (node->parent->right == node)
		node->parent->right = s;

	__tree_rebalance(tree, pivot, a);

	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	node->height = 1;
}

Tree *tree_copy(Tree *dst, const Tree *src, CopyFunc copy_func, bool *status)
{
	return_val_if_fail(src != NULL, NULL);
	return_val_if_fail(copy_func != NULL, NULL);

	if (dst == NULL) {
		dst = talloc(Tree, 1);

		if (dst == NULL) {
			msg_error("couldn't allocate memory for the copy of the tree!");
			return NULL;
		}
	}

	dst->cmp_func = src->cmp_func;
	dst->root = NULL;

	if (status != NULL)
		*status = TRUE;

	if (src->root == NULL)
		return dst;

	dst->root = copy_func(src->root);

	if (dst->root == NULL) {
		msg_error("couldn't allocate memory for a node of the copy of the tree!");

		if (status != NULL)
			*status = FALSE;

		return dst;
	}

	TreeNode *copy = dst->root;
	TreeNode *orig = src->root;

	while (orig != NULL) {
		if (orig->left != NULL && copy->left == NULL) {
			copy->left = copy_func(orig->left);

			if (copy->left == NULL) {
				msg_error("couldn't allocate memory for a node of the copy of the tree!");

				if (status != NULL)
					*status = FALSE;

				return dst;
			}

			copy->left->parent = copy;

			orig = orig->left;
			copy = copy->left;
		} else if (orig->right != NULL && copy->right == NULL) {
			copy->right = copy_func(orig->right);

			if (copy->right == NULL) {
				msg_error("couldn't allocate memory for a node of the copy of the tree!");

				if (status != NULL)
					*status = FALSE;

				return dst;
			}

			copy->right->parent = copy;

			orig = orig->right;
			copy = copy->right;
		} else {
			orig = orig->parent;
			copy = copy->parent;
		}
	}

	return dst;
}

void tree_inorder(Tree *tree, UserFunc func, void *userdata)
{
	return_if_fail(tree != NULL);
	return_if_fail(func != NULL);

	if (tree->root == NULL)
		return;

	TreeNode *current = tree->root;

	while (current != NULL) {
		if (current->left == NULL) {
			func(current, userdata);
			current = current->right;
			continue;
		}

		TreeNode *pre = current->left;

		while (pre->right != NULL && pre->right != current)
			pre = pre->right;

		if (pre->right == current) {
			pre->right = NULL;
			func(current, userdata);
			current = current->right;
		} else {
			pre->right = current;
			current = current->left;
		}
	}
}

void tree_preorder(Tree *tree, UserFunc func, void *userdata)
{
	return_if_fail(tree != NULL);
	return_if_fail(func != NULL);

	if (tree->root == NULL)
		return;

	TreeNode *current = tree->root;

	while (current != NULL) {
		if (current->left == NULL) {
			func(current, userdata);
			current = current->right;
			continue;
		}

		TreeNode *pre = current->left;

		while (pre->right != NULL && pre->right != current)
			pre = pre->right;

		if (pre->right == current) {
			pre->right = NULL;
			current = current->right;
		} else {
			pre->right = current;
			func(current, userdata);
			current = current->left;
		}
	}
}

static void __tree_postorder_reverse(TreeNode *from, TreeNode *to)
{
	if (from == to)
		return;

	TreeNode *prev = from;
	TreeNode *node = from->right;

	while (prev != to) {
		TreeNode *next = node->right;
		node->right = prev;
		prev = node;
		node = next;
	}
}

void tree_postorder(Tree *tree, UserFunc func, void *userdata)
{
	return_if_fail(tree != NULL);
	return_if_fail(func != NULL);

	if (tree->root == NULL)
		return;

	TreeNode dummy;
	tree_node_init(&dummy);
	dummy.left = tree->root;

	TreeNode *current = &dummy;

	while (current != NULL) {
		if (current->left == NULL) {
			current = current->right;
			continue;
		}

		TreeNode *pre = current->left;

		while (pre->right != NULL && pre->right != current)
			pre = pre->right;

		if (pre->right == current) {
			TreeNode *node = pre;

			__tree_postorder_reverse(current->left, pre);

			while (node != current->left) {
				func(node, userdata);
				node = node->right;
			}

			func(node, userdata);
			__tree_postorder_reverse(pre, current->left);

			pre->right = NULL;
			current = current->right;
		} else {
			pre->right = current;
			current = current->left;
		}
	}
}

void tree_purge(Tree *tree, FreeFunc free_func)
{
	return_if_fail(tree != NULL);
	return_if_fail(free_func != NULL);

	if (tree->root == NULL)
		return;

	TreeNode *current = tree->root;

	while (current != NULL) {
		if (current->left != NULL)
			current = current->left;
		else if (current->right != NULL)
			current = current->right;
		else {
			TreeNode *parent = current->parent;

			if (parent != NULL) {
				if (parent->left == current)
					parent->left = NULL;
				else
					parent->right = NULL;
			}

			free_func(current);
			current = parent;
		}
	}

	tree->root = NULL;
}

TreeNode *tree_lookup(const Tree *tree, const TreeNode *node)
{
	return_val_if_fail(tree != NULL, NULL);
	return_val_if_fail(node != NULL, NULL);

	if (tree->root == NULL)
		return NULL;

	TreeNode *current = tree->root;

	while (current != NULL) {
		i32 cmp = tree->cmp_func(current, node);

		if (cmp > 0)
			current = current->left;
		else if (cmp < 0)
			current = current->right;
		else if (current != node)
			return current;
	}

	return NULL;
}

void tree_free(Tree *tree, FreeFunc free_func)
{
	return_if_fail(tree != NULL);
	return_if_fail(free_func != NULL);

	tree_purge(tree, free_func);
	free(tree);
}
