#ifndef __KERNEL_H
#define __KERNEL_H

#include "string.h"

#define PHY_RAM_BASE		0x60000000
#define PHY_RAM_END		0xA0000000

#define PHY_UART_BASE		0x10009000
#define VIRT_UART_BASE		0xFFFF9000

#define TEXT_OFFSET		0x8000
#define PHY_KERNEL_BASE		(PHY_RAM_BASE + TEXT_OFFSET)
#define PHY_PT_BASE		(PHY_RAM_BASE + 0x4000)
#define VIRT_KERNEL_BASE	0xC0008000

#define __PA_VA__(x)		((uintptr_t)x - PHY_KERNEL_BASE + VIRT_KERNEL_BASE)
#define __VA_PA__(x)		((uintptr_t)x - VIRT_KERNEL_BASE + PHY_KERNEL_BASE)

#define PHY_VECTOR_BASE		PHY_RAM_BASE
#define VIRT_VECTOR_BASE	0xFFFF0000

#define PHY_RAMDISK_BASE	0x80100000
#define VIRT_RAMDISK_BASE	(__PA_VA__(PHY_RAMDISK_BASE))

int printk(const char *fmt, ...);
#define PANIC(condition, msg) do 					\
{									\
	if (condition) {						\
		printk("ERROR:%s:%d, %s\n",  __func__, __LINE__, msg);	\
		asm("b .");						\
	}								\
} while (0)

struct boot_args {
	uintptr_t rootfs_start;
	uintptr_t rootfs_end;
};

#endif
