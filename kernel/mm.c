#include "mm.h"

extern uintptr_t _end;
size_t *g_pt_base = (size_t *)PHY_PT_BASE;

void create_sec_mapping(uintptr_t pstart, uintptr_t pend, uintptr_t vstart)
{
	size_t idx;
	size_t mmu_flags = PMD_TYPE_SEC | PMD_SEC_B | PMD_SEC_C |
			   PMD_SEC_AP | DOMAIN_KERNEL_IDX;

	for (uintptr_t vaddr = vstart, paddr = pstart;
	     paddr < pend; vaddr += SEC_SZ, paddr += SEC_SZ)
	{
		idx = vaddr >> SEC_BITS;
		g_pt_base[idx] = ((paddr >> SEC_BITS) << SEC_BITS) | mmu_flags;
	}
}

void paging_init()
{
	create_sec_mapping(PHY_KERNEL_BASE, (uintptr_t)&_end, VIRT_KERNEL_BASE);

	create_sec_mapping(PHY_KERNEL_BASE, PHY_KERNEL_BASE + SEC_SZ,
			   PHY_KERNEL_BASE);

	create_sec_mapping(UART_BASE, UART_BASE + SEC_SZ, UART_BASE);
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
		: "r"(g_pt_base)
	);
	asm volatile (
		"mcr p15, 0, %0, c1, c0, 0"
		:
		: "r"(CR_M)
	);
}

void mm_init(void)
{
	paging_init();
	enable_mmu();
}
