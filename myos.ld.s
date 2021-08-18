OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x0;
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
	. = . + 0x400;
	_stack = .;
}
