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
	sub sp, sp, #4 * 16
	stmia sp, {r1-r12}

	// r1 = org r0
	// r2 = sp
	// r3 = lr_srv
	// r4 = lr_irq
	// r5 = spsr
	// r6 = idx
	add r2, sp, #4 * 16
	mov r3, lr
	ldmia r0, {r1, r4-r6}
	stmdb r2, {r2-r5}
	str r1, [sp, #-4]!
	// save context finish

	ldr r1, =g_handler_vector
	add r1, r1, r6, lsr #2

	mov r0, sp
	adr lr, ret_to_usr
	ldr pc, [r1]

ret_to_usr:
	ldr r0, [sp, #4 * 16]
	msr spsr, r0
	ldmia sp, {r0-pc}^
