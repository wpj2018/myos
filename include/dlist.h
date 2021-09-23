#ifndef __LIST_H
#define __LIST_H

#include "types.h"

struct dlist_node {
	struct dlist_node *prev;
	struct dlist_node *next;
};

void dlist_add(struct dlist_node *head, struct dlist_node *node);
void dlist_del(struct dlist_node *node);
void dlist_init(struct dlist_node *node);

#endif
