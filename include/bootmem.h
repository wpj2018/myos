#ifndef __BOOTMEM_H
#define __BOOTMEM_H

#include <stdint.h>
#include <stddef.h>
#include "kernel.h"

#define NR_BANKS 5

enum mem_bank_tag {
	MEM_BANK_FREE,
	MEM_BANK_RESERVE,
};

struct mem_bank {
	uintptr_t start;
	uintptr_t end;
	enum mem_bank_tag tag;
};

struct mem_info {
	struct mem_bank banks[NR_BANKS];
	size_t nr_banks;
	uintptr_t min_addr;
	uintptr_t max_addr;
	uchar *bitmaps;
	size_t bitmap_bytes;
};

void bootmem_init(void);
void *bootmem_alloc(void);
void bootmem_free(uintptr_t paddr);

#endif
