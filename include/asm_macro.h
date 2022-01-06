#ifndef __ASM_MACRO_H
#define __ASM_MACRO_H

#define ENTRY(name)		\
	.global name;		\
	name:			\

.macro save_usr
	sub sp, sp, #REG_NUM * 4
	stmib sp, {r1 - lr}^

	ldmia r0, {r0, r3 - r4}
	str r0, [sp, #4 * 0]

	add r1, sp, #REG_NUM * 4
	stmdb r1, {r3 - r4}
.endm

.macro save_svc
	sub sp, sp, #REG_NUM * 4
	stmib sp, {r1 - r12}

	ldmia r0, {r0, r3 - r4}
	str r0, [sp, #4 * 0]

	add r1, sp, #REG_NUM * 4
	mov r2, lr
	stmdb r1, {r1 - r4}
.endm

#endif
