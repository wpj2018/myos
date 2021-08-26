.global _start
_start:
	adr sp, _stack
	b main
.space 512
_stack:
