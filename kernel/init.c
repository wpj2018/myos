#include "bootmem.h"
#include "mm.h"
#include "trap.h"
#include "module.h"
#include "task.h"
#include "fs.h"

const char logo[] = "hello myos !!!\n";
int start_kernel(void)
{
	bootmem_init();
	paging_init();

	printk("%s", &logo[0]);
	fs_init();
	while(1);
	trap_init();
	module_init();
	mm_init();

	task_init();


	while(1) {
		printk("sleep\n");
	}
	return 0;
}
