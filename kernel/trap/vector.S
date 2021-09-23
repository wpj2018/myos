#include "arch.h"

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

.macro handler_stub, name
\name:
	sub lr, lr, #4
	stmia sp, {r0, lr}
	mov r0, #(\name - handler_stub_start)
	b handler_start
.endm

handler_stub_start:

handler_stub stub0
handler_stub stub1
handler_stub hyp_call_stub
handler_stub stub2
handler_stub data_abort_stub
handler_stub stub3
handler_stub irq_stub

handler_start:
	str r0, [sp, #3 * 4]
	mrs r0, spsr
	str r0, [sp, #8]

	mov r0, sp
	msr cpsr_c, #(MODE_SVC | MASK_I | MASK_F)

	push {r0}
	get_ctx r0
	stmib r0!, {r1-r12}
	pop {r7}

	// r0 = ctx
	// r2 = sp_svc
	// r3 = lr_svc
	// r4 = lr_irq
	// r5 = spsr
	// r6 = idx
	// r7 = execpt sp
	mov r2, sp
	mov r3, lr
	ldmib r7, {r4-r6}
	stmib r0, {r2-r5}

	// save org r0
	ldr r0, [r7]
	get_ctx r7
	str r0, [r7]
	// save ctx finish

	ldr r1, =g_handler_vector
	add r1, r1, r6, lsr #2

	adr lr, ret_to_usr
	ldr pc, [r1]

ret_to_usr:
	ldr r7, [r0]
	ldr r0, [r7, #4 * (REG_NUM - 1)]
	msr spsr, r0
	ldmia r7, {r0-pc}^
