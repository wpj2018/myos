#include "kernel.h"
#include "string.h"
#include "bootarg.h"
#include "mm.h"

void main(void)
{
	extern char __archive_start[];
	extern char __archive_end[];
	size_t archive_size = __archive_end - __archive_start;
	memcpy((void *)(PHY_KERNEL_BASE), (void *)__archive_start, archive_size);

	extern char __rootfs_start[];
	extern char __rootfs_end[];

	struct boot_args args = {
		.rootfs_start = (uintptr_t)__rootfs_start,
		.rootfs_end = (uintptr_t)__rootfs_end,
	};

	asm("mov r0, %0"::"r"(&args));
	asm("mov pc, %0"::"r"(PHY_KERNEL_BASE));
}
