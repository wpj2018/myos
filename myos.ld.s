INCLUDE defconfig

OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = PHY_KERNEL_BASE + TEXT_OFFSET;

	.text : { *(.text) }
	.init : {
		__vector_start__ = .;
		*(.vector.init)
		__vector_end__ = .;
	}
	.data : { *(.data) }
	.bss : { *(.bss) }
	. = . + 4096;
	_stack = .;
	. = ALIGN(4096);
	_end = .;
}
