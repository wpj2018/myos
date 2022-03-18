#include "bootmem.h"
#include "mm.h"
#include "interrupt.h"
#include "module.h"
#include "task.h"
#include "fs.h"
#include "test.h"
#include "proc.h"

const char logo[] = "hello myos !!!\n";
int start_kernel(struct boot_args *args)
{
	bootmem_init((struct boot_args *)__PA_VA__(args));
	paging_init();
	mm_init();

	printk("%s", &logo[0]);

	fs_init();
	interrupt_init();
	module_init();
	//task_init();
	run_init_process("init.elf");

	while(1) {
	//	printk("sleep\n");
	}
	return 0;
}
