INCLUDE defconfig

OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = PHY_KERNEL_BASE + TEXT_OFFSET;

	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
	. = . + 4096;
	_stack = .;
	. = ALIGN(4096);
	_end = .;
}
