#include "trb-tree.h"

#include "trb-macros.h"
#include "trb-math.h"
#include "trb-messages.h"
#include "trb-types.h"

#include <malloc.h>

typedef enum {
	LEFT,
	RIGHT
} dir_t;

#define height(n) ((n) == NULL ? 0 : (n)->height)
#define update_height(n) ((n)->height = trb_max(height((n)->left), height((n)->right)) + 1)
#define balance(n) ((isize) ((n) == NULL ? 0 : height((n)->right) - height((n)->left)))

TrbTree *trb_tree_init(TrbTree *self, TrbCmpFunc cmp_func)
{
	trb_return_val_if_fail(cmp_func != NULL, NULL);

	if (self == NULL) {
		self = trb_talloc(TrbTree, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the tree!");
			return NULL;
		}
	}

	self->root = NULL;
	self->cmp_func = cmp_func;
	self->with_data = FALSE;
	self->data = NULL;

	return self;
}

TrbTree *trb_tree_init_data(TrbTree *self, TrbCmpDataFunc cmpd_func, void *data)
{
	trb_return_val_if_fail(cmpd_func != NULL, NULL);

	if (self == NULL) {
		self = trb_talloc(TrbTree, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the tree!");
			return NULL;
		}
	}

	self->root = NULL;
	self->cmpd_func = cmpd_func;
	self->with_data = TRUE;
	self->data = data;

	return self;
}

static void __trb_tree_rotate(TrbTree *self, TrbTreeNode *n, dir_t dir)
{
	TrbTreeNode *c;
	TrbTreeNode *p = n->parent;

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
		self->root = c;
	}
}

static void __trb_tree_rebalance(TrbTree *self, TrbTreeNode *pivot, TrbTreeNode *node)
{
	TrbTreeNode *current = node;
	TrbTreeNode *pivot_parent = (pivot == NULL) ? NULL : pivot->parent;

	while (current != pivot_parent) {
		current->height = trb_max(height(current->left), height(current->right)) + 1;
		current = current->parent;
	}

	if (pivot == NULL || (balance(pivot) > -2 && balance(pivot) < 2))
		return;

	dir_t dir;         // the direction of the imbalance
	TrbTreeNode *bad;  // the child of the pivot node in the direction of the imbalance

	if (balance(pivot) == -2) {
		dir = LEFT;
		bad = pivot->left;
	} else {
		dir = RIGHT;
		bad = pivot->right;
	}

	if ((balance(bad) < 0) != dir) {
		__trb_tree_rotate(self, pivot, RIGHT - dir);
	} else {
		__trb_tree_rotate(self, bad, dir);
		update_height(bad);
		__trb_tree_rotate(self, pivot, RIGHT - dir);
	}

	update_height(pivot);
	update_height(pivot->parent);
}

bool trb_tree_insert(TrbTree *self, TrbTreeNode *node)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(node != NULL, FALSE);

	if (self->root == NULL) {
		self->root = node;
		return TRUE;
	}

	TrbTreeNode *current = self->root;
	TrbTreeNode *pivot = NULL;

	while (1) {
		i32 cmp;

		if (self->with_data)
			cmp = self->cmpd_func(current, node, self->data);
		else
			cmp = self->cmp_func(current, node);

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

		trb_msg_warn("found the same entry in the tree!");
		return FALSE;
	}

	node->parent = current;
	__trb_tree_rebalance(self, pivot, node->parent);

	return TRUE;
}

static TrbTreeNode *__trb_tree_remove_prepare(TrbTreeNode *node)
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

void trb_tree_remove(TrbTree *self, TrbTreeNode *node)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(node != NULL);

	TrbTreeNode *s = __trb_tree_remove_prepare(node);
	TrbTreeNode *current = s->parent;
	TrbTreeNode *pivot = NULL;

	while (current != NULL) {
		if (balance(current) != 0) {
			pivot = current;
			break;
		}

		current = current->parent;
	}

	if (pivot == node)
		pivot = s;

	TrbTreeNode *a;

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

	if (self->root == node)
		self->root = s;
	else if (node->parent->left == node)
		node->parent->left = s;
	else if (node->parent->right == node)
		node->parent->right = s;

	__trb_tree_rebalance(self, pivot, a);

	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	node->height = 1;
}

TrbTree *trb_tree_copy(const TrbTree *src, TrbTree *dst, TrbCopyFunc copy_func, bool *status)
{
	trb_return_val_if_fail(src != NULL, NULL);
	trb_return_val_if_fail(copy_func != NULL, NULL);

	if (dst == NULL) {
		dst = trb_talloc(TrbTree, 1);

		if (dst == NULL) {
			trb_msg_error("couldn't allocate memory for a copy of the tree!");
			return NULL;
		}
	}

	dst->root = NULL;

	dst->with_data = src->with_data;
	if (dst->with_data)
		dst->cmpd_func = src->cmpd_func;
	else
		dst->cmp_func = src->cmp_func;

	dst->data = src->data;

	if (status != NULL)
		*status = TRUE;

	if (src->root == NULL)
		return dst;

	dst->root = copy_func(src->root);

	if (dst->root == NULL) {
		trb_msg_error("couldn't allocate memory for a node of a copy of the tree!");

		if (status != NULL)
			*status = FALSE;

		return dst;
	}

	TrbTreeNode *copy = dst->root;
	TrbTreeNode *orig = src->root;

	while (orig != NULL) {
		if (orig->left != NULL && copy->left == NULL) {
			copy->left = copy_func(orig->left);

			if (copy->left == NULL) {
				trb_msg_error("couldn't allocate memory for a node of a copy of the tree!");

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
				trb_msg_error("couldn't allocate memory for a node of a copy of the tree!");

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

void trb_tree_inorder(TrbTree *self, TrbUserFunc func, void *userdata)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(func != NULL);

	if (self->root == NULL)
		return;

	TrbTreeNode *current = self->root;

	while (current != NULL) {
		if (current->left == NULL) {
			func(current, userdata);
			current = current->right;
			continue;
		}

		TrbTreeNode *pre = current->left;

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

void trb_tree_preorder(TrbTree *self, TrbUserFunc func, void *userdata)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(func != NULL);

	if (self->root == NULL)
		return;

	TrbTreeNode *current = self->root;

	while (current != NULL) {
		if (current->left == NULL) {
			func(current, userdata);
			current = current->right;
			continue;
		}

		TrbTreeNode *pre = current->left;

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

static void __trb_tree_postorder_reverse(TrbTreeNode *from, TrbTreeNode *to)
{
	if (from == to)
		return;

	TrbTreeNode *prev = from;
	TrbTreeNode *node = from->right;

	while (prev != to) {
		TrbTreeNode *next = node->right;
		node->right = prev;
		prev = node;
		node = next;
	}
}

void trb_tree_postorder(TrbTree *self, TrbUserFunc func, void *userdata)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(func != NULL);

	if (self->root == NULL)
		return;

	TrbTreeNode dummy;
	trb_tree_node_init(&dummy);
	dummy.left = self->root;

	TrbTreeNode *current = &dummy;

	while (current != NULL) {
		if (current->left == NULL) {
			current = current->right;
			continue;
		}

		TrbTreeNode *pre = current->left;

		while (pre->right != NULL && pre->right != current)
			pre = pre->right;

		if (pre->right == current) {
			TrbTreeNode *node = pre;

			__trb_tree_postorder_reverse(current->left, pre);

			while (node != current->left) {
				func(node, userdata);
				node = node->right;
			}

			func(node, userdata);
			__trb_tree_postorder_reverse(pre, current->left);

			pre->right = NULL;
			current = current->right;
		} else {
			pre->right = current;
			current = current->left;
		}
	}
}

void trb_tree_destroy(TrbTree *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(free_func != NULL);

	if (self->root == NULL)
		return;

	TrbTreeNode *current = self->root;

	while (current != NULL) {
		if (current->left != NULL)
			current = current->left;
		else if (current->right != NULL)
			current = current->right;
		else {
			TrbTreeNode *parent = current->parent;

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

	self->root = NULL;
}

TrbTreeNode *trb_tree_lookup(const TrbTree *self, const TrbTreeNode *node)
{
	trb_return_val_if_fail(self != NULL, NULL);

	if (self->root == NULL)
		return NULL;

	TrbTreeNode *current = self->root;

	while (current != NULL) {
		i32 cmp;

		if (self->with_data)
			cmp = self->cmpd_func(current, node, self->data);
		else
			cmp = self->cmp_func(current, node);

		if (cmp > 0)
			current = current->left;
		else if (cmp < 0)
			current = current->right;
		else if (current != node)
			return current;
	}

	return NULL;
}

void trb_tree_free(TrbTree *self, TrbFreeFunc free_func)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(free_func != NULL);

	trb_tree_destroy(self, free_func);
	free(self);
}
