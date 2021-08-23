#ifndef __KERNEL_H
#define __KERNEL_H

#include "string.h"

#define UART_BASE		0x10009000
#define PHY_KERNEL_BASE		0x60000000
#define PHY_PT_BASE		(PHY_KERNEL_BASE + 0x4000)

#define VIRT_KERNEL_BASE	0xC0000000

#define PAGE_BITS		(12UL)
#define PAGE_SIZE		(1UL << PAGE_BITS)

#define BYTES_PER_UCHAR		(8UL)

int printk(const char *fmt, ...);

#endif
