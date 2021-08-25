.section ".vector.init", #alloc, #execinstr
__vector_start:
	b .
	b .
	b hyp_call_stub
	b .
	b .
	b .
	b .
	b .

.macro handler_stub, name
\name:
	sub sp, sp, #4 * 18
	stmia sp, {r0-r14}
	mov r0, #(\name - handler_stub_start)
	b handler_start
.endm

handler_stub_start:

handler_stub stub0
handler_stub stub1
handler_stub hyp_call_stub

handler_start:
	adr lr, ret_to_usr
	ldr r1, =g_handler_vector
	add r1, r1, r0, lsr #2
	mov r0, sp
	ldr pc, [r1]

ret_to_usr:
	ldmia sp, {r0-r14}
	add sp, sp, #4 * 18
	mov pc, lr
