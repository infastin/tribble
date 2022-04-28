#include "Macros.h"
#include "Messages.h"
#include "Tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _IntTreeNode IntTreeNode;

struct _IntTreeNode {
	int value;
	TreeNode entry;
};

int int_tree_cmp(const void *a, const void *b)
{
	const IntTreeNode *ia = tree_node_entry(a, IntTreeNode, entry);
	const IntTreeNode *ib = tree_node_entry(b, IntTreeNode, entry);

	return ia->value - ib->value;
}

Tree *int_tree_init(Tree *tree)
{
	return tree_init(tree, int_tree_cmp, NULL);
}

void int_tree_free_node(void *_node)
{
	IntTreeNode *node = tree_node_entry(_node, IntTreeNode, entry);
	free(node);
}

void int_tree_purge(Tree *tree)
{
	tree_purge(tree, int_tree_free_node);
}

void int_tree_free(Tree *tree)
{
	int_tree_purge(tree);
	free(tree);
}

void int_tree_insert(Tree *tree, int value)
{
	return_if_fail(tree != NULL);

	IntTreeNode *node = talloc(IntTreeNode, 1);
	return_if_fail(node != NULL);

	node->value = value;
	tree_node_init(&node->entry);

	int err = tree_insert(tree, &node->entry);

	if (err != 0)
		free(node);
}

IntTreeNode *int_tree_lookup(const Tree *tree, int value)
{
	IntTreeNode node = { 0 };
	node.value = value;

	TreeNode *entry = tree_lookup(tree, &node.entry);

	if (entry == NULL)
		return NULL;
	else
		return tree_node_entry(entry, IntTreeNode, entry);
}

void int_tree_str(void *_node, void *data)
{
	IntTreeNode *node = tree_node_entry(_node, IntTreeNode, entry);
	printf("%d ", node->value);
}

void int_tree_print(Tree *tree)
{
	return_if_fail(tree != NULL);
	tree_traverse(tree, int_tree_str, NULL);
}

int main(int argc, char *argv[])
{
	srand(time(0));

	Tree tree;
	int_tree_init(&tree);

	for (int i = 0; i < 100; ++i) {
		int_tree_insert(&tree, rand() % 100);
	}

	int_tree_insert(&tree, 42);

	int_tree_print(&tree);

	int_tree_purge(&tree);

	return 0;
}
