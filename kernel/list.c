#include "list.h"

void list_add(struct list_node *head, struct list_node *node)
{
	node->prev = head;
	node->next = head->next;
	head->next->prev = node;
	head->next = node;
}

void list_del(struct list_node *node)
{
	node->next->prev = node->prev;
	node->prev->next = node->next;
	node->next = null;
	node->prev = null;
}
