#include "string.h"
#include "rbtree.h"

size_t rb_color(struct rb_node *node)
{
	if (node != NULL && node->color == RB_COLOR_RED) {
		return node->color;
	}
	return RB_COLOR_BLACK;
}

struct rb_node *rb_brother(struct rb_node *node)
{
	struct rb_node *bro;
	struct rb_node *p = node->parent;
	if (node == p->left) {
		bro = p->right;
	} else {
		bro = p->left;
	}
	return bro;
}

/*
      |                         |
      x                         y
     / \                       / \
    a   y      --->           x   r
       / \                   / \
      b   r                 a   b
 */
void rb_left_rot(struct rb_tree *tree, struct rb_node *x)
{
	struct rb_node *y = x->right;
	x->right = y->left;
	if (y->left != NULL) {
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == NULL) {
		tree->root = y;
	} else if (x->parent->left == x) {
		x->parent->left = y;
	} else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

/*
      |                         |
      x                         y
     / \                       / \
    a   y      <---           x   r
       / \                   / \
      b   r                 a   b
 */
void rb_right_rot(struct rb_tree *tree, struct rb_node *y)
{
	struct rb_node *x = y->left;
	y->left = x->right;
	if (x->right != NULL) {
		x->right->parent = y;
	}
	x->parent = y->parent;
	if (y->parent == NULL) {
		tree->root = x;
	} else if (y->parent->left == y) {
		y->parent->left = x;
	} else {
		y->parent->right = x;
	}
	x->right = y;
	y->parent = x;
}

/* return root node after rotate */
struct rb_node *rb_rot(struct rb_tree *tree, struct rb_node *pp, struct rb_node *p, struct rb_node *node)
{
	if (pp->left == p) {
		if (p->right == node) {
			rb_left_rot(tree, p);
		}
		rb_right_rot(tree, pp);
	} else {
		if (p->left == node) {
			rb_right_rot(tree, p);
		}
		rb_left_rot(tree, pp);
	}
	return pp->parent;
}

void rb_rot_branch(struct rb_tree *tree, struct rb_node *node, bool left_rot)
{
	if (left_rot) {
		rb_left_rot(tree, node);
	} else {
		rb_right_rot(tree, node);
	}
}

void rb_insert_fixup(struct rb_tree *tree, struct rb_node *node)
{
	struct rb_node *p;
	struct rb_node *u; // uncle
	struct rb_node *r;

	while ((p = node->parent) && p->color == RB_COLOR_RED)
	{
		p->parent->color = RB_COLOR_RED;

		if (p == p->parent->left) {
			u = p->parent->right;
		} else {
			u = p->parent->left;
		}
		if (u && u->color == RB_COLOR_RED) {
			u->color = RB_COLOR_BLACK;
			p->color = RB_COLOR_BLACK;
			node = p->parent;
			continue;
		}
		r = rb_rot(tree, p->parent, p, node);
		r->color = RB_COLOR_BLACK;
		break;
	}
	tree->root->color= RB_COLOR_BLACK;
}

void rb_insert(struct rb_tree *tree, struct rb_node *node)
{
	memset(node, 0, sizeof(struct rb_node));
	if (tree->root == NULL) {
		node->color = RB_COLOR_BLACK;
		tree->root = node;
		return;
	}
	struct rb_node *p = tree->root;
	struct rb_node *parent;
	while (p != NULL) {
		parent = p;
		if (tree->comp(node, p)) { // node < p
			p = p->left;
		} else {
			p = p->right;
		}
	}
	node->parent = parent;
	if (tree->comp(node, parent)) {
		parent->left = node;
	} else {
		parent->right = node;
	}
	rb_insert_fixup(tree, node);
}

struct rb_node *rb_min_node(struct rb_tree *tree)
{
	struct rb_node *p = tree->root;
	while (p->left) {
		p = p->left;
	}
	return p;
}

struct rb_node *rb_max_node(struct rb_tree *tree)
{
	struct rb_node *p = tree->root;
	while (p->right) {
		p = p->right;
	}
	return p;
}

void rb_make_orphan(struct rb_node *node)
{
	if (node == NULL || node->parent == NULL) {
		return;
	}
	struct rb_node *p = node->parent;
	if (p->left == node) {
		p->left = NULL;
	} else {
		p->right = NULL;
	}
	memset(node, 0, sizeof(struct rb_node));
}

struct rb_node *rb_substitue(struct rb_tree *tree, struct rb_node *node, struct rb_node *sub)
{
	rb_make_orphan(sub);

	struct rb_node *l = node->left;
	struct rb_node *r = node->right;
	struct rb_node *p = node->parent;

	if (sub != NULL) {
		*sub = *node;
	}

	if (l != NULL) {
		l->parent = sub;
	}
	if (r != NULL) {
		r->parent = sub;
	}
	if (p == NULL) {
		tree->root = sub;
	} else if (p->left == node) {
		p->left = sub;
	} else {
		p->right = sub;
	}

	memset(node, 0, sizeof(struct rb_node));
}

struct rb_node *rb_successor(struct rb_node *node)
{
	struct rb_node *ret = node->right;
	while (ret->left) {
		ret = ret->left;
	}
	return ret;
}

void rb_delete_fixup(struct rb_tree *tree, struct rb_node *node)
{
	struct rb_node *p;
	struct rb_node *bro;
	struct rb_node *bl; //brother left child
	struct rb_node *br; //brother right child
	bool branch; //left branch
	while (node != tree->root && rb_color(node) == RB_COLOR_BLACK) {
		p = node->parent;
		branch = (p->left == node);
		bro = rb_brother(node);
		if (rb_color(bro) == RB_COLOR_RED) {
			bro->color = RB_COLOR_BLACK;
			p->color = RB_COLOR_RED;
			rb_rot_branch(tree, p, branch);
			p = node->parent;
			bro = rb_brother(node);
		}
		bl = bro->left;
		br = bro->right;

		if (rb_color(bl) == RB_COLOR_BLACK && rb_color(br) == RB_COLOR_BLACK) {
			bro->color = RB_COLOR_RED;
			node = p;
			continue;
		}
		if (rb_color(br) == RB_COLOR_BLACK) {
			bl->color = RB_COLOR_BLACK;
			bro->color = RB_COLOR_RED;
			rb_rot_branch(tree, bro, !branch);
			p = node->parent;
			bro = rb_brother(node);
		}
		bl = bro->left;
		br = bro->right;

		bro->color = p->color;
		p->color = RB_COLOR_BLACK;
		br->color = RB_COLOR_BLACK;

		rb_rot_branch(tree, p, branch);

		node = tree->root;
	}
	node->color = RB_COLOR_BLACK;
}

void rb_delete(struct rb_tree *tree, struct rb_node *node)
{
	struct rb_node *sub; // substitute node
	struct rb_node *child;
	struct rb_node *p;

	if (node->left == NULL || node->right == NULL) { // at most one child node
		sub = node;
	} else {
		sub = rb_successor(node);
	}
	if (sub->left != NULL) {
		child = sub->left;
	} else {
		child = sub->right;
	}

	if (child == NULL && rb_color(sub) == RB_COLOR_BLACK) {
		rb_delete_fixup(tree, sub);
	}

	rb_substitue(tree, sub, child);
	if (sub != node) {
		rb_substitue(tree, node, sub);
	}
}

void rb_update(struct rb_tree *tree, struct rb_node *node)
{
	rb_delete(tree, node);
	rb_insert(tree, node);
}
