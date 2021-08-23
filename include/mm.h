#ifndef __MM_H

#include "kernel.h"

#define PMD_TYPE_SEC		(2UL << 0)
#define SEC_BITS		(20UL)
#define SEC_SZ			(1UL << SEC_BITS)
#define PMD_SEC_B		(1UL << 2)
#define PMD_SEC_C		(1UL << 3)
#define PMD_SEC_AP		(1UL << 10) //access read/write in EL1

#define CR_M			(1UL << 0)

#define DOMAIN_KERNEL_IDX	(0UL)
#define DOMAIN_USER_IDX		(1UL)

#define DOMAIN_CLIENT		(1UL << 0)
#define DOMAIN_MANAGER		(3UL << 0)

#define __DOMAIN__(idx, flag)	(flag << (idx << 1))

#define __PA_VA__(x)		(x - PHY_KERNEL_BASE + VIRT_KERNEL_BASE)
#define __VA_PA__(x)		(x - VIRT_KERNEL_BASE + PHY_KERNEL_BASE)

void mm_init(void);

#endif
