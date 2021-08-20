.global _start
.global __stack_chk_fail
.global __stack_chk_guard
_start:
	ldr sp, =_stack
	bl start_kernel

__stack_chk_fail:
__stack_chk_guard:
