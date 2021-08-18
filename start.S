.global _start
_start:
	ldr sp, =_stack
	bl start_kernel

