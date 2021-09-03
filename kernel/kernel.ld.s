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
		__module_start = .;
		*(.module.init)
		__module_end = .;

		__vector_start = .;
		*(.vector.init)
		__vector_end = .;
	}
	.data : {
		. = ALIGN(4096);
		*(.init.stack)
		__init_stack = .;
		*(.data)
	}
	.bss : { *(.bss) }
	_end = .;
}
