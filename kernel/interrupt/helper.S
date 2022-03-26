#include "arch.h"
#include "kernel.h"
#include "asm_macro.h"

ENTRY(ret_to_usr)
	sub sp, sp, #REG_NUM * 4
	ldr lr, [sp, #(REG_NUM - 2) * 4]
	ldr r0, [sp, #(REG_NUM - 1) * 4]
	msr spsr, r0

	ldmia sp, {r0 - lr}^
	add sp, sp, #REG_NUM * 4
	movs pc, lr

ENTRY(ret_to_svc)
	sub sp, sp, #REG_NUM * 4
	ldr r0, [sp, #(REG_NUM - 1) * 4]
	msr spsr, r0
	ldmia sp, {r0 - pc}^

ENTRY(__switch_to)
	stmia r0!, {r0 - lr}
	mrs r2, cpsr
	str r2, [r0, #4]

	// switch page table
	ldr r2, [r1, #REG_NUM * 4]
	sub r2, r2, #VA_PA_OFF
	mcr p15, 0, r2, c2, c0, 0
	// flush tlb
	mcr p15, 0, r0, c8, c6, 2

	ldr r2, [r1, #(REG_NUM - 1) * 4]
	msr spsr, r2
	ldmia r1, {r0 - sp, pc}^
