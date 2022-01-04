#include "stdio.h"
#include "rbtree.h"
#include "test.h"

struct tnode {
	int key;
	struct rb_node rb_node;
};

int comp(struct rb_node *n1, struct rb_node *n2)
{
	struct tnode *t1 = container_of(n1, struct tnode, rb_node);
	struct tnode *t2 = container_of(n2, struct tnode, rb_node);
	return t1->key < t2->key;
}

void rb_show(struct rb_tree *tree)
{
	if (tree->root == NULL) {
		return;
	}
	struct rb_node *que[100];
	int front, tail;
	front = tail = 0;
	que[tail++] = tree->root;
	while (front < tail) {
		struct rb_node *cur = que[front++];
		struct tnode *t = container_of(cur, struct tnode, rb_node);
		printf("key = %d color = %s", t->key, cur->color ? "black":"red");
		if (cur->parent) {
			if (cur->parent->left == cur) {
				printf(" p = %d left", container_of(cur->parent, struct tnode, rb_node)->key);
			} else {
				printf(" p = %d right", container_of(cur->parent, struct tnode, rb_node)->key);
			}
		}
		printf("\n");
		if (cur->left) {
			que[tail++] = cur->left;
		}
		if (cur->right) {
			que[tail++] = cur->right;
		}
	}
}

void test_rbtree(void)
{
	printf("------------test rb_tree begin------------\n");

	struct rb_tree tree = {
		.root = NULL,
		.comp = comp,
	};

	struct tnode nodes[20] = {0};
	for (int i = 0; i < 8; i++) {
		nodes[i].key = i;
		printf("rb insert %d\n", nodes[i].key);
		rb_insert(&tree, &nodes[i].rb_node);
		rb_show(&tree);
	}
	for (int i = 0; i < 8; i++) {
		printf("rb delete %d\n", nodes[i].key);
		rb_delete(&tree, &nodes[i].rb_node);
		rb_show(&tree);
	}
	printf("------------test rb_tree end------------\n");
}
