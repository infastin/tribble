#include "Deque.h"
#include "Hash.h"
#include "HashTable.h"
#include "HashTableIter.h"
#include "Heap.h"
#include "Math.h"
#include "Messages.h"
#include "Rand.h"
#include "Slice.h"
#include "String.h"
#include "Tree.h"
#include "Utils.h"

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

	/* TrbTree tree; */
	/* trb_tree_init(&tree, u32_tree_cmp); */
	/*  */
	/* TrbTreeNode *n4 = u32_node(14); */
	/* TrbTreeNode *n5 = u32_node(15); */
	/* TrbTreeNode *n6 = u32_node(39); */
	/*  */
	/* u32_tree_insert(&tree, 10); */
	/* u32_tree_insert(&tree, 3); */
	/* u32_tree_insert(&tree, 2); */
	/* u32_tree_insert(&tree, 4); */
	/* u32_tree_insert(&tree, 18); */
	/* u32_tree_insert(&tree, 13); */
	/* u32_tree_insert(&tree, 12); */
	/* trb_tree_insert(&tree, n4); */
	/* trb_tree_insert(&tree, n6); */
	/* u32_tree_insert(&tree, 28); */
	/* u32_tree_insert(&tree, 40); */
	/* trb_tree_insert(&tree, n5); */
	/*  */
	/* trb_tree_preorder(&tree, u32_tree_print, NULL); */
	/*  */
	/* trb_tree_destroy(&tree, u32_free); */

	TrbPcg64 prg;
	trb_pcg64_init(&prg, 0xdeadbeef);

	TrbDeque deque;
	trb_deque_init(&deque, TRUE, 4);

	u32 numbers[16] = { 0 };

	printf("push_back 13:\n");
	for (u32 i = 0; i < 13; ++i) {
		u32 val = trb_pcg64_next_u32(&prg) % 100;
		printf("%u ", val);
		trb_deque_push_back(&deque, &val);
	}

	printf("\n");

	printf("print:\n");
	for (u32 i = 0; i < deque.len; ++i) {
		u32 val = trb_deque_get(&deque, u32, i);
		printf("%u ", val);
	}

	printf("\n");

	TrbSlice deque_slice;
	trb_deque_slice(&deque, &deque_slice, 0, deque.len);
	trb_heapsort(&deque_slice, (TrbCmpFunc) trb_u32cmp);
	trb_reverse(&deque_slice);

	for (u32 i = 0; i < deque.len; ++i) {
		u32 val = trb_deque_get(&deque, u32, i);
		printf("%u ", val);
	}

	/* printf("pop_front 10:\n"); */
	/* for (u32 i = 0; i < 10; ++i) { */
	/* 	u32 val; */
	/* 	trb_deque_pop_front(&deque, &val); */
	/* 	printf("%u ", val); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("print:\n"); */
	/* for (u32 i = 0; i < deque.len; ++i) { */
	/* 	u32 val = trb_deque_get(&deque, u32, i); */
	/* 	printf("%u ", val); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("pop_back_many 3:\n"); */
	/* trb_deque_pop_back_many(&deque, 3, numbers); */
	/* for (u32 i = 0; i < 3; ++i) { */
	/* 	printf("%u ", numbers[i]); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("print:\n"); */
	/* for (u32 i = 0; i < deque.len; ++i) { */
	/* 	u32 val = trb_deque_get(&deque, u32, i); */
	/* 	printf("%u ", val); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("push_front_many 7:\n"); */
	/* for (u32 i = 0; i < 7; ++i) { */
	/* 	numbers[i] = trb_pcg64_next_u32(&prg) % 100; */
	/* 	printf("%u ", numbers[i]); */
	/* } */
	/*  */
	/* trb_deque_push_front_many(&deque, numbers, 7); */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("print:\n"); */
	/* for (u32 i = 0; i < deque.len; ++i) { */
	/* 	u32 val = trb_deque_get(&deque, u32, i); */
	/* 	printf("%u ", val); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("insert_many 8 2:\n"); */
	/* for (u32 i = 0; i < 8; ++i) { */
	/* 	numbers[i] = trb_pcg64_next_u32(&prg) % 100; */
	/* 	printf("%u ", numbers[i]); */
	/* } */
	/* trb_deque_insert_many(&deque, 2, numbers, 8); */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("print:\n"); */
	/* for (u32 i = 0; i < deque.len; ++i) { */
	/* 	u32 val = trb_deque_get(&deque, u32, i); */
	/* 	printf("%u ", val); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("remove_range 5 8:\n"); */
	/* trb_deque_remove_range(&deque, 5, 8, numbers); */
	/*  */
	/* for (u32 i = 0; i < 8; ++i) { */
	/* 	printf("%u ", numbers[i]); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("print:\n"); */
	/* for (u32 i = 0; i < deque.len; ++i) { */
	/* 	u32 val = trb_deque_get(&deque, u32, i); */
	/* 	printf("%u ", val); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("push_back 4:\n"); */
	/* for (u32 i = 0; i < 4; ++i) { */
	/* 	u32 val = trb_pcg64_next_u32(&prg) % 100; */
	/* 	printf("%u ", val); */
	/* 	trb_deque_push_back(&deque, &val); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("print:\n"); */
	/* for (u32 i = 0; i < deque.len; ++i) { */
	/* 	u32 val = trb_deque_get(&deque, u32, i); */
	/* 	printf("%u ", val); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("remove_range 2 3:\n"); */
	/* trb_deque_remove_range(&deque, 2, 3, numbers); */
	/*  */
	/* for (u32 i = 0; i < 3; ++i) { */
	/* 	printf("%u ", numbers[i]); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("print:\n"); */
	/* for (u32 i = 0; i < deque.len; ++i) { */
	/* 	u32 val = trb_deque_get(&deque, u32, i); */
	/* 	printf("%u ", val); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("search 71\n"); */
	/* usize index; */
	/* trb_deque_search(&deque, trb_get_ptr(u32, 78), (TrbCmpFunc) trb_u32cmp, &index); */
	/* printf("%zu\n", index); */
	/*  */
	/* printf("remove_all:\n"); */
	/* trb_deque_remove_all(&deque, numbers); */
	/*  */
	/* for (u32 i = 0; i < 16; ++i) { */
	/* 	printf("%u ", numbers[i]); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("print:\n"); */
	/* for (u32 i = 0; i < deque.len; ++i) { */
	/* 	u32 val = trb_deque_get(&deque, u32, i); */
	/* 	printf("%u ", val); */
	/* } */
	/*  */
	/* printf("\n"); */
	/*  */
	/* printf("%zu\n", deque.buckets.len); */

	trb_deque_destroy(&deque, NULL);

	return 0;
}
