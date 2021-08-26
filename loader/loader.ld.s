OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x60000000;

	.text : { *(.text) }
	.rel.plt : { *(.rel.plt) }
	.data : {
		__archive_start = .;
		*(.archive)
		__archive_end = .;
		*(.data)
	}
	.bss : { *(.bss) }
	_end = .;
}
