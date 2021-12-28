#ifndef __BOOTMEM_H
#define __BOOTMEM_H

#include "kernel.h"
#include "types.h"
#include "bootarg.h"

#define NR_BANKS 5

struct mem_bank {
	uintptr_t start;
	uintptr_t end;
};

struct mem_info {
	struct mem_bank banks[NR_BANKS];
	size_t nr_banks;
	uintptr_t min_addr;
	uintptr_t max_addr;
	uchar *bitmaps;
	size_t bitmap_bytes;
	size_t used_pages;
	size_t free_pages;
};

void bootmem_stat(void);
void bootmem_init(struct boot_args *args);
void *bootmem_alloc(void);
void bootmem_free(uintptr_t paddr);
void bootmem_free_to_buddy(void);
bool bootmem_is_work(void);

#endif
