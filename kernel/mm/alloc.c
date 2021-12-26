#include "bootmem.h"
#include "buddy.h"
#include "slab.h"
#include "mm.h"

/* TODO: vm management unimplement */
void *vmalloc(size_t size)
{
}

void *kalloc(size_t size)
{
	return slab_alloc(size);
}

void kfree(void *vaddr)
{
	struct page *page;

	page = pfn_to_page(__VA_PA__(vaddr));
	buddy_free_pages(page);
}

void mm_init(void)
{
	bootmem_stat();
	buddy_init();
	bootmem_free_to_buddy();
	buddy_stat();
	slab_init();
}
