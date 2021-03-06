#ifndef __MM_H

#include "page.h"
#include "printk.h"
#include "bootmem.h"
#include "buddy.h"
#include "debug.h"

#define VIRT_END_MEM		(~0UL)
#define VIRT_HIGHMEM_BASE	(VIRT_END_MEM - 128 * SEC_SIZE + 1)

#define SEC_BITS		(20UL)
#define SEC_SIZE		(1UL << SEC_BITS)
#define SEC_MASK		(~(SEC_SIZE - 1))

#define PMD_TYPE_SEC		(2UL << 0)
#define PMD_SEC_B		(1UL << 2)
#define PMD_SEC_C		(1UL << 3)
#define PMD_SEC_AP_RW		(1UL << 10) //access read/write in EL1

#define PMD_TYPE_PT		(1UL << 0)
#define PT_TYPE_SMALL		(3UL << 0)
#define PT_B			(1UL << 2)
#define PT_C			(1UL << 3)
#define PT_AP_RW		(1UL << 4 | 1UL << 5)
#define PT_AP_RD		((1UL << 9) | (3UL << 4))

#define SCTLR_M			(1UL << 0)
#define SCTLR_A			(1UL << 1)
#define SCTLR_C			(1UL << 2)
#define SCTLR_V			(1UL << 13)

#define DOMAIN_KERN_ID		(0UL)
#define DOMAIN_USER_ID		(1UL)

#define DOMAIN_CLIENT		(1UL << 0)
#define DOMAIN_MANAGER		(3UL << 0)

#define __DOMAIN__(idx, flag)	(flag << (idx << 1))
#define DOMAIN_DACR		(__DOMAIN__(DOMAIN_KERN_ID, DOMAIN_CLIENT) |\
				 __DOMAIN__(DOMAIN_USER_ID, DOMAIN_CLIENT))

#define PMD_SEC_FLAGS		(PMD_TYPE_SEC | PMD_SEC_B | PMD_SEC_C | PMD_SEC_AP_RW)


void mm_init(struct boot_args *);
void mmu_init(void);
void *ioremap(uintptr_t phys_addr);
void *vmalloc(size_t size);
void *kalloc(size_t size);
void *kzalloc(size_t size);
void kfree(void *vaddr);
void map_kern_page(size_t *pgd, uintptr_t vaddr, uintptr_t paddr, size_t perm);
void map_user_page(size_t *pgd, uintptr_t vaddr, uintptr_t paddr, size_t perm);

#endif
