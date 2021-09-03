#include "kernel.h"
#include "mm.h"
#include "arch.h"
#include "gic.h"
#include "timer.h"
#include "trap.h"

size_t irq_stack[4];
size_t abt_stack[4];
size_t und_stack[4];

void hyp_call_hdl(void)
{
	printk("------catch svc-------\n");
}

void data_abort_hdl(void)
{
	printk("------catch data abort-------\n");
}

void irq_hdl(void)
{
	printk("------catch irq-------\n");
	size_t ack_no = gicc_get_ack();
	timer_clear_int1();
	gicc_set_eoi(ack_no);
}

struct handler_vector g_handler_vector = {
	NULL,
	NULL,
	&hyp_call_hdl,
	NULL,
	&data_abort_hdl,
	NULL,
	&irq_hdl,
	NULL
};

void setup_stack(void)
{
	asm(
		"msr cpsr_c, %0\n"
		"mov sp, %1\n"
		"msr cpsr_c, %2\n"
		"mov sp, %3\n"
		"msr cpsr_c, %4\n"
		"mov sp, %5\n"
		"msr cpsr_c, %6\n"
		:
		:
		"I"(MODE_IRQ),
		"r"(&irq_stack[0]),
		"I"(MODE_ABT),
		"r"(&abt_stack[0]),
		"I"(MODE_UND),
		"r"(&und_stack[0]),
		"I"(MODE_SVC)
	);
}

void trap_init(void)
{
	extern char __vector_start[];
	extern char __vector_end[];

	size_t vector_size =  __vector_end - __vector_start;
	memset((void *)PHY_VECTOR_BASE, 0, PAGE_SIZE);
	memcpy((void *)PHY_VECTOR_BASE, __vector_start, vector_size);

	setup_stack();
}
