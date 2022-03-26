#include "string.h"
#include "buddy.h"
#include "slab.h"
#include "mm.h"

/* TODO: vm management unimplement */
void *vmalloc(size_t size)
{
}

void *kalloc(size_t size)
{
	void *ret = NULL;
	struct page *page;

	if (SLAB_HEAD_SIZE + size <= PAGE_SIZE) {
		ret = slab_alloc(SLAB_HEAD_SIZE + size);
		BUG_ON(ret == NULL);
		*(size_t *)ret = size;
		ret += SLAB_HEAD_SIZE;
	} else {
		page = buddy_alloc_pages(size);
		BUG_ON(page == NULL);
		ret = (void *)page_to_virt(page);
	}
	return ret;
}

void *kzalloc(size_t size)
{
	void *ret = kalloc(size);

	memset(ret, 0, size);
	return ret;
}

void kfree(void *vaddr)
{
	size_t size;
	struct page *page;

	page = virt_to_page(vaddr);
	if (page->in_slab) {
		size = *(size_t *)(vaddr - SLAB_HEAD_SIZE);
		slab_free(vaddr - SLAB_HEAD_SIZE, SLAB_HEAD_SIZE + size);
	} else {
		buddy_free_pages(page);
	}
}
