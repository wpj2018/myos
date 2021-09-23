#ifndef __RBTREE_H
#define __RBTREE_H

#include "types.h"

#define RB_COLOR_RED		0
#define RB_COLOR_BLACK		1

struct rb_node {
	struct rb_node *left;
	struct rb_node *right;
	struct rb_node *parent;
	size_t color;
};
struct rb_tree {
	struct rb_node *root;
	int (*comp)(struct rb_node *n1, struct rb_node *n2);
};

void rb_insert(struct rb_tree *tree, struct rb_node *node);
void rb_delete(struct rb_tree *tree, struct rb_node *node);
void rb_update(struct rb_tree *tree, struct rb_node *node);
struct rb_node *rb_min_node(struct rb_tree *tree);
struct rb_node *rb_max_node(struct rb_tree *tree);

#endif
