#ifndef __LIST_H
#define __LIST_H

#include "types.h"

struct dlist_node {
	struct dlist_node *prev;
	struct dlist_node *next;
};

#endif
