#include "bootmem.h"
#include "buddy.h"
#include "slab.h"
#include "mm.h"

void mm_init(struct boot_args *args)
{
	bootmem_init(args);
	bootmem_stat();
	buddy_init();
	bootmem_free_to_buddy();
	buddy_stat();
	slab_init();
	mmu_init();
}
