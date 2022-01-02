#include "arch.h"
#include "kernel.h"

.section ".vector.init", #alloc, #execinstr
__vector_start:
	b .
	b .
	b hyp_call_stub
	b .
	b data_abort_stub
	b .
	b irq_stub
	b .

.macro get_ctx, ctx
	mov \ctx, sp, lsr #12
	mov \ctx, \ctx, lsl #12
.endm

.macro handler_stub, name, correction=4
\name:
	sub lr, lr, #\correction
	stmdb sp, {r0-r1, lr-pc}
	mov r0, #(\name - handler_stub_start)
	b handler_start
.endm

handler_stub_start:

handler_stub stub0
handler_stub stub1
handler_stub hyp_call_stub, 0
handler_stub stub2
handler_stub data_abort_stub, 8
handler_stub stub3
handler_stub irq_stub

handler_idx:
	.word 0x0
/* [    ] <---(sp - 4 * 0) */
/* [spsr] <---(sp - 4 * 1) */
/* [ lr ] <---(sp - 4 * 2) */
/* [ r1 ] <---(sp - 4 * 3) */
/* [ r0 ] <---(sp - 4 * 4) */
handler_start:
	ldr r1, =handler_idx
	str r0, [r1]
	mrs r0, spsr
	str r0, [sp, #-4 * 1]

	mov r1, sp
	msr cpsr_c, #(MODE_SVC | MASK_I | MASK_F)

	get_ctx r0
	stmia r0!, {r0-r12}

	// r0 = ctx
	// r1 = except sp
	// r2 = sp_svc
	// r3 = lr_svc
	// r4 = lr_irq
	// r5 = spsr

	mov r2, sp
	mov r3, lr
	ldmdb r1!, {r4-r5}
	stmia r0, {r2-r5}

	// save org r0,r1
	ldmdb r1!, {r4-r5}
	get_ctx r0
	stmia r0, {r4-r5}
	// save ctx finish

	ldr r1, =handler_idx
	ldr r2, [r1]
	ldr r1, =g_handler_vector
	add r1, r1, r2, lsr #2

	mov fp, sp
	adr lr, ret_to_usr
	ldr pc, [r1]

ret_to_usr:
	// switch page table
	ldr r7, [r0, #4]
	sub r7, r7, #VA_PA_OFF
	mcr p15, 0, r7, c2, c0, 0

	ldr r7, [r0]
	ldr r0, [r7, #4 * (REG_NUM - 1)]
	msr spsr, r0
	ldmia r7, {r0-pc}^
