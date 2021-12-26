#include "buddy.h"
#include "mm.h"
#include "bootmem.h"

struct mem_bank g_init_banks[] = {
	{PHY_RAM_BASE, PHY_RAM_END},
};
struct mem_info g_mem_info;
struct page *g_mem_pages;
bool g_bootmem_is_work = true;

static void bootmem_add_bank(struct mem_bank *bank)
{
	size_t cnt = g_mem_info.nr_banks;
	if (cnt >= NR_BANKS) {
		printk("bootmem add bank err!!!");
		return;
	}
	size_t min_addr = PAGE_ALIGN(bank->start);
	size_t max_addr = PAGE_ALIGN(bank->end);
	if (!cnt || min_addr < g_mem_info.min_addr) {
		g_mem_info.min_addr = min_addr;
	}
	if (!cnt || max_addr > g_mem_info.max_addr) {
		g_mem_info.max_addr = max_addr;
	}
	g_mem_info.used_pages += (max_addr - min_addr) / PAGE_SIZE;
	g_mem_info.bitmap_bytes = (g_mem_info.max_addr - g_mem_info.min_addr) / PAGE_SIZE / BITS_PER_BYTE;
	g_mem_info.banks[cnt] = *bank;
	g_mem_info.nr_banks++;
}

static void bitmap_set(uintptr_t start, uintptr_t end, bool is_set)
{
	size_t start_idx = (PAGE_ALIGN(start) - g_mem_info.min_addr) >> PAGE_BITS;
	size_t end_idx = ((PAGE_ALIGN(end)) - g_mem_info.min_addr) >> PAGE_BITS;
	size_t idx;

	for (idx = start_idx; idx < end_idx; idx++) {
		if (is_set) {
			g_mem_info.bitmaps[idx >> 3] |= (1UL << (idx & 0x7));
		} else {
			g_mem_info.bitmaps[idx >> 3] &= ~(1UL << (idx & 0x7));
		}
		g_mem_info.used_pages += (is_set ? 1 : -1);
		g_mem_info.free_pages -= (is_set ? 1 : -1);
	}
}

void bootmem_init(struct boot_args *args)
{
	extern char _end[];
	size_t nr_banks;
	struct mem_bank *bank;
	struct mem_bank reserve_banks[] = {
		{PHY_RAM_BASE, __VA_PA__(_end)},
		{args->rootfs_start, args->rootfs_end},
		{}, // reserve for bitmap
		{}, // reserve for struct page vector
	};
	size_t pages_bytes;
	size_t i;

	memset(&g_mem_info, 0, sizeof(struct mem_info));
	nr_banks = sizeof(g_init_banks) / sizeof(struct mem_bank);
	for (i = 0; i < nr_banks; i++) {
		bank = &g_init_banks[i];
		bootmem_add_bank(bank);
	}

	// bitmap
	g_mem_info.bitmaps = (uchar *)PAGE_ALIGN(_end);
	memset((void*)g_mem_info.bitmaps, 0xff, g_mem_info.bitmap_bytes);
	for (i = 0; i < nr_banks; i++) {
		bank = &g_init_banks[i];
		bitmap_set(bank->start, bank->end, 0);
	}

	struct mem_bank bitmap_bank = {
		.start = __VA_PA__(g_mem_info.bitmaps),
		.end = __VA_PA__(g_mem_info.bitmaps) + g_mem_info.bitmap_bytes,
	};

	// struct pages vector
	g_mem_pages = (struct page *)__PA_VA__(bitmap_bank.end);
	pages_bytes = (g_mem_info.free_pages + g_mem_info.used_pages) * sizeof(struct page);
	struct mem_bank pages_bank = {
		.start = __VA_PA__(g_mem_pages),
		.end = __VA_PA__(g_mem_pages) + pages_bytes,
	};

	nr_banks = sizeof(reserve_banks) / sizeof(struct mem_bank);
	reserve_banks[nr_banks - 2] = bitmap_bank;
	reserve_banks[nr_banks - 1] = pages_bank;

	for (i = 0; i < nr_banks; i++) {
		bank = &reserve_banks[i];
		bitmap_set(bank->start, bank->end, 1);
	}
}

void *bootmem_alloc(void)
{
	uintptr_t paddr = 0;
	size_t i = 0;
	size_t bitmap_bytes = g_mem_info.bitmap_bytes;

	while (i < bitmap_bytes && (g_mem_info.bitmaps[i] & 0xff) == 0xff) {
		i++;
	}
	if (i != bitmap_bytes) {
		size_t idx = i * BITS_PER_BYTE + __builtin_ctz(~g_mem_info.bitmaps[i]);
		paddr = g_mem_info.min_addr + idx * PAGE_SIZE;
		bitmap_set(paddr, paddr + PAGE_SIZE, 1);
	}
	return (void *)paddr;
}

void bootmem_free(uintptr_t paddr)
{
	bitmap_set(paddr, paddr + PAGE_SIZE, 0);
}

void bootmem_stat(void)
{
	size_t total_pages = g_mem_info.free_pages + g_mem_info.used_pages;
	printk("===============mem stat begin==============\n");
	printk("|| total pages = %d\n", total_pages);
	printk("|| used  pages = %d\n", g_mem_info.used_pages);
	printk("|| free  pages = %d\n", g_mem_info.free_pages);
	printk("===============mem stat end================\n");
}

static void bootmem_free_page_to_buddy(struct page *page, size_t order)
{
	page->order = order;
	buddy_free_pages(page);
}

static void bootmem_free_meta_to_buddy(void)
{
	struct page *pages = phy_to_page(__VA_PA__(g_mem_info.bitmaps));
	size_t nr_pages = PAGE_ALIGN(g_mem_info.bitmap_bytes) >> PAGE_BITS;
	for (size_t i = 0; i < nr_pages; i++) {
		bootmem_free_page_to_buddy(&pages[i], 0);
	}
}

void bootmem_free_to_buddy(void)
{
	struct page *pages = phy_to_page(g_mem_info.min_addr);
	for (size_t i = 0; i < g_mem_info.bitmap_bytes; i++) {
		if (g_mem_info.bitmaps[i] == 0xff) {
			continue;
		}
		if (g_mem_info.bitmaps[i] == 0x0) {
			bootmem_free_page_to_buddy(&pages[i * 8], 3);
			continue;
		}
		for (size_t j = 0; j < 8; j++) {
			if (g_mem_info.bitmaps[i] & (1UL << j)) {
				continue;
			}
			bootmem_free_page_to_buddy(&pages[i * 8 + j], 0);
		}
	}
	bootmem_free_meta_to_buddy();
	g_bootmem_is_work = false;
}

bool bootmem_is_work(void)
{
	return g_bootmem_is_work;
}
