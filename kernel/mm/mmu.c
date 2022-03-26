#include "page.h"
#include "bootmem.h"
#include "buddy.h"
#include "mm.h"

static void flush_tlb(void)
{
	asm("mcr p15, 0, r0, c8, c6, 2");
}

static void clear_secs(size_t *pgd, uintptr_t vstart, size_t size)
{
	size_t s_idx = vstart >> SEC_BITS;
	size_t e_idx = s_idx + (size >> SEC_BITS);

	for (size_t idx = s_idx; idx < e_idx; idx++) {
		pgd[idx] = 0;
	}
	flush_tlb();
}

static void map_secs(size_t *pgd, uintptr_t vstart, uintptr_t pstart, size_t size)
{
	size_t s_idx = vstart >> SEC_BITS;
	size_t e_idx = s_idx + (size >> SEC_BITS);
	uintptr_t paddr = pstart & SEC_MASK;

	for (size_t idx = s_idx; idx < e_idx; idx++, paddr += SEC_SIZE) {
		pgd[idx] = paddr | PMD_SEC_FLAGS;
	}
	flush_tlb();
}

static void clear_page(size_t *pgd, uintptr_t vaddr)
{
	size_t l1_idx = vaddr >> SEC_BITS;
	size_t l2_idx = (vaddr >> PAGE_BITS) & 0xff;
	uintptr_t phy_page = pgd[l1_idx] & PAGE_MASK;

	if (phy_page) {
		((uintptr_t *)__PA_VA__(phy_page))[l2_idx] = 0;
	}
	flush_tlb();
}

static void map_page(size_t *pgd, uintptr_t vaddr, uintptr_t paddr, size_t perm, size_t domain_id)
{
	size_t l1_idx = vaddr >> SEC_BITS;
	size_t l2_idx = (vaddr >> PAGE_BITS) & 0xff;
	size_t flags = PT_TYPE_SMALL | PT_B | PT_C | perm;
	uintptr_t phy_page = pgd[l1_idx] & PAGE_MASK;

	if (!phy_page) {
		void *va = kzalloc(PAGE_SIZE);
		BUG_ON(va == NULL);
		phy_page = __VA_PA__(va);
		pgd[l1_idx] = phy_page | PMD_TYPE_PT | (domain_id << 5);
	}

	uintptr_t *page_table = (uintptr_t *)__PA_VA__(phy_page);
	if (!page_table[l2_idx]) {
		page_table[l2_idx] = (paddr & PAGE_MASK) | flags;
	}
	flush_tlb();
}

static void enable_mmu(void)
{
	asm volatile (
		"mcr p15, 0, %0, c3, c0, 0"
		:
		: "r"(DOMAIN_DACR)
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
	map_secs((size_t *)PHY_PT_BASE, PHY_RAM_BASE, PHY_RAM_BASE, SEC_SIZE);
	map_secs((size_t *)PHY_PT_BASE,
		 VIRT_KERNEL_BASE - TEXT_OFFSET, PHY_RAM_BASE,
		 VIRT_HIGHMEM_BASE - VIRT_KERNEL_BASE + TEXT_OFFSET
	);
	map_secs((size_t *)PHY_PT_BASE, VIRT_UART_BASE, PHY_UART_BASE, SEC_SIZE);

	enable_mmu();
}

void mmu_init(void)
{
	clear_secs((size_t *)VIRT_PT_BASE, 0, VIRT_KERNEL_BASE);
	clear_secs((size_t *)VIRT_PT_BASE, VIRT_HIGHMEM_BASE, VIRT_END_MEM - VIRT_HIGHMEM_BASE + 1);

	map_kern_page((size_t *)VIRT_PT_BASE, VIRT_UART_BASE, PHY_UART_BASE, PT_AP_RW);
	/* why set read-only does not work */
	map_kern_page((size_t *)VIRT_PT_BASE, VIRT_VECTOR_BASE, PHY_VECTOR_BASE, PT_AP_RD);
}

void *ioremap(uintptr_t phys_addr)
{
	map_kern_page((size_t *)VIRT_PT_BASE, phys_addr, phys_addr, PT_AP_RW);
	return (void*)phys_addr;
}

void map_kern_page(size_t *pgd, uintptr_t vaddr, uintptr_t paddr, size_t perm)
{
	map_page(pgd, vaddr, paddr, perm, DOMAIN_KERN_ID);
}
void map_user_page(size_t *pgd, uintptr_t vaddr, uintptr_t paddr, size_t perm)
{
	map_page(pgd, vaddr, paddr, perm, DOMAIN_USER_ID);
}
