#ifndef __KERNEL_H
#define __KERNEL_H

#include <stddef.h>
#include "string.h"

#define PHY_KERNEL_BASE 0x60000000
#define PAGE_BITS 12
#define PAGE_SIZE (1UL << PAGE_BITS)

#define BYTES_PER_UCHAR 8

int printk(const char *fmt, ...);

#endif
