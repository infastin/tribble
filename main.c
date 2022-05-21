#include "Hash.h"
#include "HashTable.h"
#include "HashTableIter.h"
#include "Heap.h"
#include "Messages.h"
#include "Rand.h"
#include "String.h"
#include "Tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	u32 a;
	u32 b;
	u32 dist;
} Edge;

i32 cmp_edges(const void *a, const void *b)
{
	const Edge *ea = a;
	const Edge *eb = b;

	if (ea->dist > eb->dist)
		return 1;

	if (ea->dist < eb->dist)
		return -1;

	return 0;
}

typedef struct {
	TrbTreeNode entry;
	u32 value;
} U32Node;

i32 u32_tree_cmp(const void *a, const void *b)
{
	const U32Node *na = trb_container_of(a, U32Node, entry);
	const U32Node *nb = trb_container_of(b, U32Node, entry);

	if (na->value > nb->value)
		return 1;

	if (na->value < nb->value)
		return -1;

	return 0;
}

TrbTreeNode *u32_node(u32 value)
{
	U32Node *node = trb_talloc(U32Node, 1);
	trb_return_val_if_fail(node != NULL, NULL);

	node->value = value;
	trb_tree_node_init(&node->entry);

	return &node->entry;
}

void u32_free(void *node)
{
	U32Node *un = trb_tree_node_entry(node, U32Node, entry);
	free(un);
}

void u32_tree_insert(TrbTree *tree, u32 value)
{
	trb_return_if_fail(tree != NULL);

	U32Node *node = trb_talloc(U32Node, 1);
	trb_return_if_fail(node != NULL);

	node->value = value;
	trb_tree_node_init(&node->entry);

	if (!trb_tree_insert(tree, &node->entry))
		free(node);
}

void u32_tree_print(void *node, void *data)
{
	TrbTreeNode *n = node;
	U32Node *un = trb_container_of(node, U32Node, entry);

	TrbString buffer;
	trb_string_init0(&buffer);

	trb_string_push_back_fmt(&buffer, "%u -- { ", un->value);

	if (n->parent != NULL) {
		U32Node *parent = trb_container_of(n->parent, U32Node, entry);
		trb_string_push_back_fmt(&buffer, "%u ", parent->value);
	} else {
		trb_string_push_back_fmt(&buffer, "nil ");
	}

	if (n->left != NULL) {
		U32Node *left = trb_container_of(n->left, U32Node, entry);
		trb_string_push_back_fmt(&buffer, "%u ", left->value);
	} else {
		trb_string_push_back_fmt(&buffer, "nil ");
	}

	if (n->right != NULL) {
		U32Node *right = trb_container_of(n->right, U32Node, entry);
		trb_string_push_back_fmt(&buffer, "%u ", right->value);
	} else {
		trb_string_push_back_fmt(&buffer, "nil ");
	}

	trb_string_push_back_c(&buffer, '}');
	printf("%s\n", buffer.data);
	trb_string_destroy(&buffer);
}

void my_free(char **ptr)
{
	free(*ptr);
}

int main(int argc, char *argv[])
{
	/* HashTable ht; */
	/* ht_init(&ht, 32, 8, 0xdeadbeef, jhash, (CmpFunc) strcmp); */
	/*  */
	/* ht_insert(&ht, get_arr(char, 32, "Mike Urasawa"), get_ptr(u64, 42)); */
	/* ht_insert(&ht, get_arr(char, 32, "Mario Franco"), get_ptr(u64, 12)); */
	/*  */
	/* HashTableIter iter; */
	/* ht_iter_init(&iter, &ht); */
	/*  */
	/* const char *name; */
	/* u64 *age; */
	/*  */
	/* while (ht_iter_next(&iter, (const void **) &name, (void **) &age)) { */
	/* 	printf("--------------------\n"); */
	/* 	printf(" Name: %s\n", name); */
	/* 	printf(" Age: %lu\n", *age); */
	/* 	printf("--------------------\n"); */
	/* } */
	/*  */
	/* ht_destroy(&ht, NULL, NULL); */

	/* HashTable ht; */
	/* ht_init(&ht, 30, 8, 0xdeadbeef, jhash, (CmpFunc) strcmp); */
	/*  */
	/* ht_insert(&ht, get_arr(char, 30, "Mike Urasawa"), get_ptr(u64, 42)); */
	/* ht_insert(&ht, get_arr(char, 32, "Mario Franco"), get_ptr(u64, 12)); */
	/*  */
	/* struct entry { */
	/* 	char key[30]; */
	/* 	u64 age; */
	/* }; */
	/*  */
	/* struct entry buf[ht.slots]; */
	/* usize len; */
	/*  */
	/* ht_remove_all(&ht, distance_of(struct entry, key, age), buf, &len); */
	/*  */
	/* for (usize i = 0; i < len; ++i) { */
	/* 	printf("--------------------\n"); */
	/* 	printf(" Name: %s\n", buf[i].key); */
	/* 	printf(" Age: %lu\n", buf[i].age); */
	/* 	printf("--------------------\n"); */
	/* } */
	/*  */
	/* ht_destroy(&ht, NULL, NULL); */

	/* String a; */
	/* string_init0(&a); */
	/* string_assign_fmt(&a, "%d!", 0b1010); */
	/*  */
	/* printf("%s\n", a.data); */
	/*  */
	/* string_push_back_fmt(&a, "%s!", "World"); */
	/*  */
	/* printf("%s\n", a.data); */
	/*  */
	/* char *buf = string_steal0(&a, NULL); */
	/*  */
	/* printf("%s\n", buf); */
	/*  */
	/* free(buf); */

	TrbTree tree;
	trb_tree_init(&tree, u32_tree_cmp);

	TrbTreeNode *n4 = u32_node(14);
	TrbTreeNode *n5 = u32_node(15);
	TrbTreeNode *n6 = u32_node(39);

	u32_tree_insert(&tree, 10);
	u32_tree_insert(&tree, 3);
	u32_tree_insert(&tree, 2);
	u32_tree_insert(&tree, 4);
	u32_tree_insert(&tree, 18);
	u32_tree_insert(&tree, 13);
	u32_tree_insert(&tree, 12);
	trb_tree_insert(&tree, n4);
	trb_tree_insert(&tree, n6);
	u32_tree_insert(&tree, 28);
	u32_tree_insert(&tree, 40);
	trb_tree_insert(&tree, n5);

	trb_tree_preorder(&tree, u32_tree_print, NULL);

	trb_tree_destroy(&tree, u32_free);

	return 0;
}
