#ifndef __LIST_H
#define __LIST_H

#include "kernel.h"

struct list_node {
	struct list_node *prev;
	struct list_node *next;
};

#endif
