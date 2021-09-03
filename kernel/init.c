#include "bootmem.h"
#include "mm.h"
#include "trap.h"
#include "module.h"
#include "gic.h"
#include "timer.h"

const char logo[] = "hello myos !!!\n";
int start_kernel(void)
{
	bootmem_init();
	paging_init();

	printk("%s", &logo[0]);
	trap_init();
	module_init();

	while(1) {
		printk("sleep\n");
	}
	return 0;
}
