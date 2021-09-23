#include "bootmem.h"
#include "buddy.h"
#include "mm.h"

extern uintptr_t _end;
size_t *g_pt_base = (size_t *)__PA_VA__(PHY_PT_BASE);

void clear_secs(uintptr_t vstart, uintptr_t vend)
{
	size_t idx;
	size_t start_idx = vstart >> SEC_BITS;
	size_t end_idx = vend >> SEC_BITS;
	if (vend & SEC_MASK) {
		end_idx++;
	}

	for (idx = start_idx; idx < end_idx; idx++) {
		g_pt_base[idx] = 0;
	}
}

void map_secs(uintptr_t pstart, uintptr_t pend, uintptr_t vstart)
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
	     paddr < pend; vaddr += SEC_SIZE, paddr += SEC_SIZE)
	{
		idx = vaddr >> SEC_BITS;
		pt_base[idx] = (paddr & (~SEC_MASK)) | mmu_flags;
	}
}

void clear_page(uintptr_t vaddr)
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

void map_page(uintptr_t paddr, uintptr_t vaddr, size_t perm)
{
	size_t l1_idx = vaddr >> SEC_BITS;
	size_t l2_idx = (vaddr >> PAGE_BITS) & 0xff;
	size_t mmu_flags = PT_TYPE_SMALL | PT_B | PT_C | perm;
	uintptr_t phy_page = 0;

	if (!g_pt_base[l1_idx]) {
		phy_page = (uintptr_t)bootmem_alloc();
		g_pt_base[l1_idx] = phy_page | PMD_TYPE_PT | DOMAIN_KERNEL_IDX;
	} else {
		phy_page= g_pt_base[l1_idx] & PAGE_MASK;
	}
	uintptr_t *page_table = (uintptr_t *)__PA_VA__(phy_page);
	if (!page_table[l2_idx]) {
		page_table[l2_idx] = (paddr & PAGE_MASK) | mmu_flags;
	}
}

void paging_init(void)
{
	clear_secs(0, VIRT_KERNEL_BASE & (~SEC_MASK));
	clear_secs(HIGHMEM_BASE, END_MEM);

	map_page(PHY_UART_BASE, VIRT_UART_BASE, PT_AP_RW);
	/* why set read-only does not work */
	map_page(PHY_VECTOR_BASE, VIRT_VECTOR_BASE, PT_AP_RD);
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
		: "r"(SCTLR_M | SCTLR_A | SCTLR_C | SCTLR_V)
	);
}

void early_mmu_init(void)
{
	map_secs(PHY_RAM_BASE, PHY_RAM_BASE + SEC_SIZE, PHY_RAM_BASE);
	map_secs(PHY_RAM_BASE, PHY_RAM_END, __PA_VA__(PHY_RAM_BASE));

	enable_mmu();
}

void *ioremap(uintptr_t phys_addr)
{
	map_page(phys_addr, phys_addr, PT_AP_RW);
	return (void*)phys_addr;
}

/* TODO: vm management unimplement */
void *vmalloc(size_t size)
{
}

void *kalloc(size_t size)
{
	struct page *page;

	page = buddy_alloc_pages(size);
	if (page != NULL) {
		return (void *)__PA_VA__(page_to_phy(page));
	}
	return NULL;
}

void kfree(void *vaddr)
{
	struct page *page;

	page = pfn_to_page(__VA_PA__(vaddr));
	buddy_free_pages(page);
}

void mm_init(void)
{
	bootmem_stat();
	buddy_init();
	bootmem_free_to_buddy();
	buddy_stat();
}
