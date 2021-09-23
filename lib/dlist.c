#include "dlist.h"

void dlist_add(struct dlist_node *head, struct dlist_node *node)
{
	node->prev = head;
	node->next = head->next;
	head->next->prev = node;
	head->next = node;
}

void dlist_del(struct dlist_node *node)
{
	node->next->prev = node->prev;
	node->prev->next = node->next;
	node->next = node;
	node->prev = node;
}

void dlist_init(struct dlist_node *node)
{
	node->next = node;
	node->prev = node;
}
