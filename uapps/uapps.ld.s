INCLUDE ../defconfig

OUTPUT_ARCH(arm)
ENTRY(main)
SECTIONS
{
	. = 0x30008000;

	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
