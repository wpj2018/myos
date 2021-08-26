.global _start
.global __stack_chk_fail
.global __stack_chk_guard

.section ".text.head", #alloc, #execinstr
_start:
	adr sp, stack_stub
	bl early_mmu_init
	ldr sp, =_stack
	ldr pc, =start_kernel

.space 512
stack_stub:
__stack_chk_fail:
__stack_chk_guard:
