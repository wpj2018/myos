#include "arch.h"

.global _start
.section ".text.head", #alloc, #execinstr
_start:
	adr sp, stack_stub
	bl early_mmu_init
	ldr sp, =__init_stack
	ldr pc, =start_kernel

.space 512
stack_stub:
