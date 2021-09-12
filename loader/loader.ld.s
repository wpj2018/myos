OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x80000000;

	.text : { *(.text) }
	.rel.plt : { *(.rel.plt) }
	.data : {
		__archive_start = .;
		*(.archive)
		__archive_end = .;

		. = ALIGN(1<<20);
		__rootfs_start = .;
		*(.rootfs)
		__rootfs_end = .;

		*(.data)
	}
	.bss : { *(.bss) }
	_end = .;
}
