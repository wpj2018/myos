#include "kernel.h"
#include "buddy.h"
#include "slab.h"

typedef unsigned short slot_t;

struct slot {
	slot_t units;
	slot_t offset;
};

struct dlist_node g_slab_page_list;

static slot_t slab_calc_units(size_t size)
{
	return (size + sizeof(struct slot) - 1) / sizeof(struct slot);
}

static struct slot *slab_next_slot(struct slot *slot)
{
	return &slot[slot->offset];
}

static bool slab_last_slot(struct page *page, struct slot *slot)
{
	return (uintptr_t)slot == page_to_virt(&page[1]);
}

static bool slab_can_merge(struct page *page, struct slot *prev, struct slot *next)
{
	if (slab_last_slot(page, next)) {
		return false;
	}
	return (prev->units == prev->offset);
}

static void slab_link_next(struct page *page, struct slot *cur, struct slot *next, bool need_merge)
{
	cur->offset = next - cur;
	if (need_merge && slab_can_merge(page, cur, next)) {
		cur->units += next->units;
		cur->offset += next->offset;
	}
}

static void slab_make_slots(struct page *page)
{
	struct slot *slots = (struct slot *)page_to_virt(page);
	slot_t units = slab_calc_units(PAGE_SIZE);

	slots[0].units = units;
	slab_link_next(page, &slots[0], &slots[units], 0);
	page->free_slots = units;
	page->free_list = (void *)slots;
}

static void slab_relink_slot(struct page *page, struct slot *cur, struct slot *prev, slot_t req_units)
{
	struct slot *next;

	if (cur->units == req_units) {
		next = slab_next_slot(cur);
	} else {
		next = &cur[req_units];
		next->units = cur->units - req_units;
		slab_link_next(page, next, slab_next_slot(cur), 0);
	}
	if (page->free_list == cur) {
		page->free_list = next;
	} else {
		slab_link_next(page, prev, next, 0);
	}
}

static void *slab_alloc_slot(struct page *page, slot_t req_units)
{
	struct slot *cur = page->free_list;
	struct slot *prev = page->free_list;

	for (; !slab_last_slot(page, cur); prev = cur, cur = slab_next_slot(cur)) {
		if (cur->units < req_units) {
			continue;
		}
		slab_relink_slot(page, cur, prev, req_units);
		page->free_slots -= req_units;
		return cur;
	}

	return NULL;
}

void *slab_alloc(size_t size)
{
	void *ret = NULL;
	struct page *page = NULL;
	slot_t req_units = slab_calc_units(size);

	for (struct dlist_node *dnode = g_slab_page_list.next; dnode != &g_slab_page_list; dnode = dnode->next) {
		page = container_of(dnode, struct page, dnode);
		if (page->free_slots < req_units) {
			continue;
		}
		ret = slab_alloc_slot(page, req_units);
		if (ret) {
			return ret;
		}
	}
	page = buddy_alloc_pages(size);
	if (page != NULL) {
		dlist_add(&g_slab_page_list, &page->dnode);
		slab_make_slots(page);
		ret = slab_alloc_slot(page, req_units);
	}
	return ret;
}

void slab_free(void *kaddr, size_t size)
{
	struct slot *slot = (struct slot *)kaddr;
	slot->units = slab_calc_units(size);
	struct page *page = virt_to_page(kaddr);
	struct slot *head = page->free_list;
	struct slot *next;

	while (!slab_last_slot(page, head)) {
		next = slab_next_slot(head);
		if (next > slot) {
			break;
		}
		head = next;
	}
	if (head > slot) {
		slab_link_next(page, slot, head, 1);
		page->free_list = slot;
	} else {
		slab_link_next(page, slot, next, 1);
		slab_link_next(page, head, slot, 1);
	}

	page->free_slots += slot->units;
}

void slab_init(void)
{
	dlist_init(&g_slab_page_list);
}
