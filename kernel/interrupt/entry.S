#include "arch.h"
#include "kernel.h"
#include "asm_macro.h"

.section ".vector.init", #alloc, #execinstr
__vector_start:
	b .
	b .
	ldr pc, =svc_entry
	b .
	ldr pc, =dabt_entry
	b .
	ldr pc, =irq_entry
	b .

svc_entry:
	sub sp, sp, #REG_NUM * 4
	stmia sp, {r0 - lr}^
	mrs r0, spsr
	str lr, [sp, #4 * 15]
	str r0, [sp, #4 * 16]
	mov r0, sp
	bl svc_hdl
	add sp, sp, #REG_NUM * 4
	b ret_to_usr

.macro hdl_mode, mode, from
\mode\()_\from:
	save_\from
	mov r0, sp
	bl \mode\()_hdl
	add sp, sp, #REG_NUM * 4
	b ret_to_\from
.endm

.macro hdl_entry, mode, correction=4
	hdl_mode \mode usr
	hdl_mode \mode svc
\mode\()_entry:
	sub lr, lr, #\correction
	stmia sp, {r0, lr}
	mrs r0, spsr
	str r0, [sp, #4 * 2]

	mrs lr, cpsr
	bic lr, lr, #0x1f
	orr lr, lr, #(MODE_SVC | MASK_I | MASK_F)
	msr spsr, lr

	ldr lr, =\mode\()_usr
	and r0, r0, #0x1f
	teq r0, #MODE_SVC
	ldreq lr, =\mode\()_svc

	mov r0, sp
	movs pc, lr
.endm

hdl_entry irq, 4
hdl_entry dabt, 8
