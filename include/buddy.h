#ifndef __BUDDY_H
#define __BUDDY_H

#include "dlist.h"
#include "page.h"

#define BUDDY_MAX_ORDER		11

struct free_area {
	struct dlist_node free_list;
	size_t nr_free;
};

void buddy_init(void);
struct page *buddy_alloc_pages(size_t size);
void buddy_free_pages(struct page *page);
void buddy_stat(void);

#endif
