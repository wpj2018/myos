#include "bootmem.h"
#include "mm.h"

extern uintptr_t _end;
size_t *g_pt_base = (size_t *)PHY_PT_BASE;

void create_sec_mapping(uintptr_t pstart, uintptr_t pend, uintptr_t vstart)
{
	size_t idx;
	size_t mmu_flags = PMD_TYPE_SEC | PMD_SEC_B | PMD_SEC_C |
			   PMD_SEC_AP_RW | DOMAIN_KERNEL_IDX;

	/*
	   g_pt_base cannot be used for it stored in data section and
	   mmu is not enabled, so we cannot access vaddr
	 */
	size_t *pt_base = (size_t *)PHY_PT_BASE;

	for (uintptr_t vaddr = vstart, paddr = pstart;
	     paddr < pend; vaddr += SEC_SZ, paddr += SEC_SZ)
	{
		idx = vaddr >> SEC_BITS;
		pt_base[idx] = ((paddr >> SEC_BITS) << SEC_BITS) | mmu_flags;
	}
}

void create_pt_mapping(uintptr_t paddr, uintptr_t vaddr, size_t perm)
{
	size_t l1_idx = vaddr >> SEC_BITS;
	size_t l2_idx = (vaddr >> PAGE_BITS) & 0xff;
	size_t mmu_flags = PT_TYPE_SMALL | PT_B | PT_C | perm;
	uintptr_t *page_table = NULL;

	if (!g_pt_base[l1_idx]) {
		page_table = (uintptr_t *)bootmem_alloc();
		g_pt_base[l1_idx] = (uintptr_t)page_table | PMD_TYPE_PT |
				    DOMAIN_KERNEL_IDX;
	} else {
		page_table = (uintptr_t *)((g_pt_base[l1_idx] >> PAGE_BITS) << PAGE_BITS);
	}
	if (!page_table[l2_idx]) {
		page_table[l2_idx] = ((paddr >> PAGE_BITS) << PAGE_BITS) |
				      mmu_flags;
	}
}

void paging_init()
{
	create_pt_mapping(UART_BASE, UART_BASE, PT_AP_RW);
	/* why set read-only does not work */
	create_pt_mapping(PHY_VECTOR_BASE, VIRT_VECTOR_BASE, PT_AP_RD);
}

void enable_mmu(void)
{
	asm volatile (
		"mcr p15, 0, %0, c3, c0, 0"
		:
		: "r"(__DOMAIN__(DOMAIN_KERNEL_IDX, DOMAIN_CLIENT))
	);
	asm volatile (
		"mcr p15, 0, %0, c2, c0, 0"
		:
		: "r"(PHY_PT_BASE)
	);
	asm volatile (
		"mcr p15, 0, %0, c1, c0, 0"
		:
		: "r"(CR_M | CR_V)
	);
}

void early_mmu_init(void)
{
	create_sec_mapping(PHY_KERNEL_BASE, PHY_KERNEL_BASE + SEC_SZ, VIRT_KERNEL_BASE);

	create_sec_mapping(PHY_KERNEL_BASE, PHY_KERNEL_BASE + SEC_SZ,
			   PHY_KERNEL_BASE);

	enable_mmu();
}

void *ioremap(uintptr_t phys_addr)
{
	create_pt_mapping(phys_addr, phys_addr, PT_AP_RW);
	return (void*)phys_addr;
}

/* TODO: vm management unimplement */
void *vmalloc(size_t size)
{
	void *paddr = bootmem_alloc();
	return paddr;
}

void mm_init(void)
{

}
