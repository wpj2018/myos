.global _start
_start:
	ldr sp, =_stack
	bl start_kernel

.global __stack_chk_fail
.global __stack_chk_guard
__stack_chk_fail:
__stack_chk_guard:
