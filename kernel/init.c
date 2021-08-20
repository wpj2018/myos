#include "bootmem.h"

const char logo[] = "hello myos !!!\n";
int start_kernel(void)
{
	printk("%s", &logo[0]);
	bootmem_init();
	void *ret = bootmem_alloc();
	bootmem_free((uintptr_t)ret);
	ret = bootmem_alloc();
	mm_init();
	for(;;);
	return 0;
}
