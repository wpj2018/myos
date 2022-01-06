#include "kernel.h"
#include "string.h"
#include "mm.h"
#include "arch.h"
#include "gic.h"
#include "timer.h"
#include "sched.h"
#include "trap.h"
#include "preempt.h"

size_t irq_stack[4];
size_t abt_stack[4];
size_t und_stack[4];

void svc_hdl(struct sys_regs *regs)
{
	char *s = (char *)regs->r0;
	printk("%s", s);
}

void dabt_hdl(struct sys_regs *regs)
{
	PANIC(1, "------catch data abort -------\n");
}

void irq_hdl(struct sys_regs *regs)
{
	size_t ack_no = gicc_get_ack();
	timer_clear_int1();
	gicc_set_eoi(ack_no);

	schedule();
}

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
	memset((void *)__PA_VA__(PHY_VECTOR_BASE), 0, PAGE_SIZE);
	memcpy((void *)__PA_VA__(PHY_VECTOR_BASE), __vector_start, vector_size);

	setup_stack();
}
