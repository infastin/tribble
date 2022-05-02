#include "Tree.h"

#include "Macros.h"
#include "Messages.h"
#include "Types.h"

#include <malloc.h>

typedef enum {
	BLACK = 0,
	RED
} tn_color;

typedef enum {
	LEFT,
	RIGHT
} r_dir;

#define color(n) (((n) == NULL) ? BLACK : (n)->color)

#define uncle(n) ({                              \
	TreeNode *__p = (n)->parent;                 \
	TreeNode *__g = __p->parent;                 \
	(__p == __g->left) ? __g->right : __g->left; \
})

#define sibling(n) ({                            \
	TreeNode *__p = (n)->parent;                 \
	((n) == __p->left) ? __p->right : __p->left; \
})

Tree *tree_init(Tree *tree, CmpFunc cmp_func, CopyFunc copy_func)
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
	tree->cmpf = cmp_func;
	tree->cpyf = copy_func;

	return tree;
}

Tree *tree_copy(Tree *dst, const Tree *src, int32_t *err)
{
	return_val_if_fail(src != NULL, NULL);
	return_val_if_fail(src->cpyf != NULL, NULL);

	if (dst == NULL) {
		dst = talloc(Tree, 1);

		if (dst == NULL) {
			msg_error("couldn't allocate memory for the copy of the tree!");
			return NULL;
		}
	}

	dst->cmpf = src->cmpf;
	dst->cpyf = src->cpyf;
	dst->root = NULL;

	if (err != NULL)
		*err = 0;

	if (src->root == NULL)
		return dst;

	dst->root = dst->cpyf(src->root);

	if (dst->root == NULL) {
		msg_error("couldn't allocate memory for a node of the copy of the tree!");

		if (err != NULL)
			*err = -1;

		return dst;
	}

	TreeNode *copy = dst->root;
	TreeNode *orig = src->root;

	while (orig != NULL) {
		if (orig->left != NULL && copy->left == NULL) {
			copy->left = dst->cpyf(orig->left);

			if (copy->left == NULL) {
				msg_error("couldn't allocate memory for a node of the copy of the tree!");

				if (err != NULL)
					*err = -1;

				return dst;
			}

			copy->left->parent = copy;

			orig = orig->left;
			copy = copy->left;
		} else if (orig->right != NULL && copy->right == NULL) {
			copy->right = dst->cpyf(orig->right);

			if (copy->right == NULL) {
				msg_error("couldn't allocate memory for a node of the copy of the tree!");

				if (err != NULL)
					*err = -1;

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

void tree_traverse(Tree *tree, UserFunc func, void *userdata)
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

static void __tree_rotate(Tree *tree, TreeNode *p, r_dir dir)
{
	TreeNode *n;
	TreeNode *g = p->parent;

	if (dir == LEFT) {
		n = p->right;

		p->right = n->left;
		n->left = p;
		n->parent = g;
		p->parent = n;

		if (p->right != NULL)
			p->right->parent = p;
	} else {
		n = p->left;

		p->left = n->right;
		n->right = p;
		n->parent = g;
		p->parent = n;

		if (p->left != NULL)
			p->left->parent = p;
	}

	if (g != NULL) {
		if (p == g->right)
			g->right = n;
		else
			g->left = n;
	} else
		tree->root = n;
}

static void __tree_recolor(Tree *tree, TreeNode *node)
{
	TreeNode *p, *g, *u, *n;
	n = node;

	while (1) {
		if (n->color == RED && tree->root == n)
			n->color = BLACK;

		if ((p = n->parent) == NULL)
			return;

		if (p->color == RED && tree->root == p)
			p->color = BLACK;

		if (p->color == BLACK)
			return;

		g = p->parent;
		u = uncle(node);

		if (p->color == RED && color(u) == RED) {
			p->color = BLACK;
			u->color = BLACK;
			g->color = RED;

			n = g;
		} else if (color(u) == BLACK) {
			if (n == p->right && p == g->left) {
				__tree_rotate(tree, p, LEFT);
				n = p;
				p = g->left;
			} else if (n == p->left && p == g->right) {
				__tree_rotate(tree, p, RIGHT);
				n = p;
				p = g->right;
			}

			if (n == p->right && p == g->right)
				__tree_rotate(tree, g, LEFT);
			else
				__tree_rotate(tree, g, RIGHT);

			p->color = BLACK;
			g->color = RED;

			return;
		}
	}
}

bool tree_insert(Tree *tree, TreeNode *node)
{
	return_val_if_fail(tree != NULL, FALSE);
	return_val_if_fail(node != NULL, FALSE);

	if (tree->root == NULL) {
		tree->root = node;
		tree->root->color = BLACK;
		return TRUE;
	}

	TreeNode *current = tree->root;

	while (1) {
		int32_t cmp = tree->cmpf(current, node);

		if (cmp > 0) {
			if (current->left == NULL) {
				current->left = node;
				break;
			}

			current = current->left;
		} else if (cmp < 0) {
			if (current->right == NULL) {
				current->right = node;
				break;
			}

			current = current->right;
		} else {
			msg_warn("found the same entry in the tree!");
			return FALSE;
		}
	}

	node->parent = current;
	__tree_recolor(tree, node);

	return TRUE;
}

static void __tree_swap_case1(TreeNode *a, TreeNode *b)
{
	TreeNode *old_a_left = a->left;
	TreeNode *old_a_right = a->right;

	TreeNode *old_b_parent = b->parent;
	TreeNode *old_b_left = b->left;
	TreeNode *old_b_right = b->right;

	b->parent = a;
	a->parent = old_b_parent;

	if (old_b_parent != NULL) {
		if (old_b_parent->left == b)
			old_b_parent->left = a;
		else
			old_b_parent->right = a;
	}

	b->left = old_a_left;
	b->right = old_a_left;

	if (old_a_left != NULL)
		old_a_left->parent = b;

	if (old_a_right != NULL)
		old_a_right->parent = b;

	if (old_b_left == a) {
		a->left = b;
		a->right = old_b_right;

		if (old_b_right != NULL)
			old_b_right->parent = a;
	} else {
		a->right = b;
		a->left = old_b_left;

		if (old_b_left != NULL)
			old_b_left->parent = a;
	}
}

static void __tree_swap_case2(TreeNode *a, TreeNode *b)
{
	TreeNode *old_a_parent = a->parent;
	TreeNode *old_a_left = a->left;
	TreeNode *old_a_right = a->right;

	TreeNode *old_b_parent = b->parent;
	TreeNode *old_b_left = b->left;
	TreeNode *old_b_right = b->right;

	a->parent = old_b_parent;

	if (old_b_parent != NULL) {
		if (old_b_parent->left == b)
			old_b_parent->left = a;
		else
			old_b_parent->right = a;
	}

	b->parent = old_a_parent;

	if (old_a_parent != NULL) {
		if (old_a_parent->left == a)
			old_a_parent->left = b;
		else
			old_a_parent->right = b;
	}

	a->left = old_b_left;
	a->right = old_b_right;

	if (old_b_left != NULL)
		old_b_left->parent = a;

	if (old_b_right != NULL)
		old_b_right->parent = a;

	b->left = old_a_left;
	b->right = old_a_right;

	if (old_a_left != NULL)
		old_a_left->parent = b;

	if (old_a_right != NULL)
		old_a_right->parent = b;
}

static void __tree_swap(TreeNode *a, TreeNode *b)
{
	if (a->parent == b)
		__tree_swap_case1(a, b);
	else if (b->parent == a)
		__tree_swap_case1(b, a);
	else
		__tree_swap_case2(a, b);

	tn_color tmp = a->color;

	a->color = b->color;
	b->color = tmp;
}

void tree_swap(Tree *tree, TreeNode *a, TreeNode *b)
{
	return_if_fail(tree != NULL);
	return_if_fail(a != NULL);
	return_if_fail(b != NULL);

	if (a == b)
		return;

	__tree_swap(a, b);

	if (tree->root == a)
		tree->root = b;
	else if (tree->root == b)
		tree->root = a;
}

static TreeNode *__tree_prepare_remove(Tree *tree, TreeNode *node)
{
	if (node->left != NULL && node->right != NULL) {
		TreeNode *s = node->right;
		for (; s->left != NULL; s = s->left)
			continue;

		__tree_swap(node, s);

		if (tree->root == node)
			tree->root = s;
	}

	if (node->left != NULL)
		return node->left;

	return node->right;
}

static void __tree_replace_child(Tree *tree, TreeNode *n, TreeNode *c)
{
	if (c != NULL)
		c->parent = n->parent;

	if (n->parent != NULL) {
		if (n == n->parent->left)
			n->parent->left = c;
		else
			n->parent->right = c;
	} else
		tree->root = c;
}

static void __tree_remove(Tree *tree, TreeNode *node)
{
	TreeNode *n, *p, *s;
	n = node;

	/* case 1 */
	while ((p = n->parent) != NULL) {
		/* case 2 */
		s = sibling(n);

		if (s->color == RED) {
			n->parent->color = RED;
			s->color = BLACK;

			if (n == n->parent->left)
				__tree_rotate(tree, p, LEFT);
			else
				__tree_rotate(tree, p, RIGHT);

			s = sibling(n);
		}

		/* case 3 */
		if (p->color == RED && s->color == BLACK && color(s->left) == BLACK && color(s->right) == BLACK) {
			s->color = RED;
			n = p;
			continue;
		} else if (n == n->parent->left && s->color == BLACK && color(s->left) == RED && color(s->right) == BLACK) {
			/* case 4 */
			s->color = RED;
			p->color = BLACK;
		} else {
			/* case 5 */
			if (n == n->parent->left && s->color == BLACK && color(s->left) == RED && color(s->right) == BLACK) {
				s->color = RED;
				s->left->color = BLACK;
				__tree_rotate(tree, s, RIGHT);
				s = sibling(n);
			} else if (n == n->parent->right && s->color == BLACK && color(s->left) == BLACK && color(s->right) == RED) {
				s->color = RED;
				s->right->color = BLACK;
				__tree_rotate(tree, s, LEFT);
				s = sibling(n);
			}

			/* case 6 */
			s->color = p->color;
			p->color = BLACK;

			if (n == p->left) {
				s->right->color = BLACK;
				__tree_rotate(tree, p, LEFT);
			} else {
				s->left->color = BLACK;
				__tree_rotate(tree, p, RIGHT);
			}
		}
	}
}

void tree_remove(Tree *tree, TreeNode *node)
{
	return_if_fail(tree != NULL);
	return_if_fail(node != NULL);

	TreeNode *n, *c;

	n = node;
	c = __tree_prepare_remove(tree, node);

	if (n->color == BLACK) {
		n->color = color(c);
		__tree_remove(tree, n);
	}

	__tree_replace_child(tree, n, c);
	if (n->parent == NULL && c != NULL)
		c->color = BLACK;

	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
}

TreeNode *tree_lookup(const Tree *tree, const TreeNode *node)
{
	return_val_if_fail(tree != NULL, NULL);
	return_val_if_fail(node != NULL, NULL);

	if (tree->root == NULL)
		return NULL;

	TreeNode *current = tree->root;

	while (current != NULL) {
		int32_t cmp = tree->cmpf(current, node);

		if (cmp > 0)
			current = current->left;
		else if (cmp < 0)
			current = current->right;
		else if (current != node)
			return current;
	}

	return NULL;
}
