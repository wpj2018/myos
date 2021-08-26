INCLUDE ../defconfig

OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = VIRT_KERNEL_BASE;

	.text : {
		*(.text.head)
		*(.text)
	}
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
