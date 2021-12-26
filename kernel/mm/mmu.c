#include "page.h"
#include "bootmem.h"
#include "buddy.h"
#include "mm.h"

size_t *g_pt_base = (size_t *)__PA_VA__(PHY_PT_BASE);

static void clear_secs(uintptr_t vstart, size_t size)
{
	size_t s_idx = vstart >> SEC_BITS;
	size_t e_idx = s_idx + (size >> SEC_BITS);

	for (size_t idx = s_idx; idx < e_idx; idx++) {
		g_pt_base[idx] = 0;
	}
}

static void map_secs(uintptr_t vstart, uintptr_t pstart, size_t size)
{
	/*
	   g_pt_base cannot be used for it stored in data section and
	   mmu is not enabled, so we cannot access vaddr
	 */
	size_t *pt_base = (size_t *)PHY_PT_BASE;
	size_t s_idx = vstart >> SEC_BITS;
	size_t e_idx = s_idx + (size >> SEC_BITS);
	uintptr_t paddr = pstart & SEC_MASK;

	for (size_t idx = s_idx; idx < e_idx; idx++, paddr += SEC_SIZE) {
		pt_base[idx] = paddr | PMD_SEC_FLAGS;
	}
}

static void clear_page(uintptr_t vaddr)
{
	size_t l1_idx = vaddr >> SEC_BITS;
	size_t l2_idx = (vaddr >> PAGE_BITS) & 0xff;
	uintptr_t phy_page = 0;

	if (g_pt_base[l1_idx]) {
		phy_page= g_pt_base[l1_idx] & PAGE_MASK;
	}
	uintptr_t *page_table = (uintptr_t *)__PA_VA__(phy_page);
	if (page_table && page_table[l2_idx]) {
		page_table[l2_idx] = 0;
	}
}

static uintptr_t phy_zalloc_page(void)
{
	uintptr_t phy_page = 0;
	struct page *page;

	if (bootmem_is_work()) {
		phy_page = (uintptr_t)bootmem_alloc();
	} else {
		page = buddy_alloc_pages(PAGE_SIZE);
		PANIC(page == NULL, "buddy no mem\n");
		phy_page = (uintptr_t)page_to_phy(page);
	}
	PANIC(phy_page == 0, "phy_page is 0\n");
	memset((void *)__PA_VA__(phy_page), 0, PAGE_SIZE);

	return phy_page;
}

static void map_page(uintptr_t vaddr, uintptr_t paddr, size_t perm)
{
	size_t l1_idx = vaddr >> SEC_BITS;
	size_t l2_idx = (vaddr >> PAGE_BITS) & 0xff;
	size_t mmu_flags = PT_TYPE_SMALL | PT_B | PT_C | perm;
	uintptr_t phy_page = 0;

	if (!g_pt_base[l1_idx]) {
		phy_page = phy_zalloc_page();
		g_pt_base[l1_idx] = phy_page | PMD_TYPE_PT | DOMAIN_KERNEL_IDX;
	} else {
		phy_page = g_pt_base[l1_idx] & PAGE_MASK;
	}
	uintptr_t *page_table = (uintptr_t *)__PA_VA__(phy_page);
	if (!page_table[l2_idx]) {
		page_table[l2_idx] = (paddr & PAGE_MASK) | mmu_flags;
	}
}

static void enable_mmu(void)
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
		: "r"(SCTLR_M | SCTLR_A | SCTLR_C | SCTLR_V)
	);
}

void early_mmu_init(void)
{
	map_secs(PHY_RAM_BASE, PHY_RAM_BASE, SEC_SIZE);
	map_secs(__PA_VA__(PHY_RAM_BASE), PHY_RAM_BASE, PHY_RAM_END - PHY_RAM_BASE);

	enable_mmu();
}

void paging_init(void)
{
	clear_secs(0, VIRT_KERNEL_BASE);
	clear_secs(VIRT_HIGHMEM_BASE, VIRT_END_MEM - VIRT_HIGHMEM_BASE + 1);

	map_page(VIRT_UART_BASE, PHY_UART_BASE, PT_AP_RW);
	/* why set read-only does not work */
	map_page(VIRT_VECTOR_BASE, PHY_VECTOR_BASE, PT_AP_RD);
}

void *ioremap(uintptr_t phys_addr)
{
	map_page(phys_addr, phys_addr, PT_AP_RW);
	return (void*)phys_addr;
}
