#include "arch.h"

.global _start
.section ".text.head", #alloc, #execinstr
_start:
	adr sp, stack_stub
	push {r0}
	bl early_mmu_init
	pop {r0}
	ldr sp, =__init_stack
	ldr pc, =start_kernel

.space 512
stack_stub:
