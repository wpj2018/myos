#include "bootmem.h"
#include "buddy.h"
#include "slab.h"

void mm_init(void)
{
	bootmem_stat();
	buddy_init();
	bootmem_free_to_buddy();
	buddy_stat();
	slab_init();
}
