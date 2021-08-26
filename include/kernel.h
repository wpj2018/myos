#ifndef __KERNEL_H
#define __KERNEL_H

#include "string.h"

#define PHY_RAM_BASE		0x60000000
#define PHY_RAM_END		0xA0000000

#define UART_BASE		0x10009000

#define TEXT_OFFSET		0x8000
#define PHY_KERNEL_BASE		(PHY_RAM_BASE + TEXT_OFFSET)
#define PHY_PT_BASE		(PHY_RAM_BASE + 0x4000)

#define VIRT_KERNEL_BASE	0xC0008000
#define PHY_VECTOR_BASE		PHY_RAM_BASE
#define VIRT_VECTOR_BASE	0xFFFF0000

#define PAGE_BITS		(12UL)
#define PAGE_SIZE		(1UL << PAGE_BITS)

#define BYTES_PER_UCHAR		(8UL)

int printk(const char *fmt, ...);

#endif
