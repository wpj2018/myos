#include "bootmem.h"
#include "mm.h"
#include "trap.h"

const char logo[] = "hello myos !!!\n";
int start_kernel(void)
{
	bootmem_init();
	paging_init();
	printk("%s", &logo[0]);
	trap_init();
	asm("svc 0");
	printk("return from svc !!!\n");
	while(1);
	return 0;
}
