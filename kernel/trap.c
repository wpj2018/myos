#include "kernel.h"
#include "mm.h"
#include "trap.h"

void hyp_call_hdl(void)
{
	printk("------catch svc-------\n");
}

struct handler_vector g_handler_vector = {
	NULL,
	NULL,
	__PA_VA__(&hyp_call_hdl),
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

void trap_init(void)
{
	extern char __vector_start__[];
	extern char __vector_end__[];

	size_t vector_size =  __vector_end__ - __vector_start__;
	memset((void *)PHY_VECTOR_BASE, 0, PAGE_SIZE);
	memcpy((void *)PHY_VECTOR_BASE, __vector_start__, vector_size);
}
