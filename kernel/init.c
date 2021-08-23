#include "bootmem.h"
#include "mm.h"

#define __JUMP_TO_VM__ do {				\
	uintptr_t __off__ = __PA_VA__(0);		\
	asm("add pc, pc, %0"::"r"(__off__));		\
	asm("nop");					\
} while(0)

const char logo[] = "hello myos !!!\n";
int start_kernel(void)
{
	printk("%s", &logo[0]);
	bootmem_init();
	void *ret = bootmem_alloc();
	bootmem_free((uintptr_t)ret);
	ret = bootmem_alloc();
	mm_init();
	__JUMP_TO_VM__;
	while(1);
	return 0;
}
