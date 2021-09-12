#include "bootmem.h"
#include "mm.h"

struct mem_info g_mem_info;

static void bootmem_add_bank(uintptr_t start, uintptr_t end, enum mem_bank_tag tag)
{
	size_t cnt = g_mem_info.nr_banks;
	if (cnt >= NR_BANKS) {
		printk("bootmem add bank err!!!");
		return;
	}
	if (start < g_mem_info.min_addr) {
		g_mem_info.min_addr = start;
	}
	if (end > g_mem_info.max_addr) {
		g_mem_info.max_addr = end;
	}
	g_mem_info.banks[cnt].start = start;
	g_mem_info.banks[cnt].end = end;
	g_mem_info.banks[cnt].tag = tag;
	g_mem_info.nr_banks++;
}

static void bitmap_set(uintptr_t start, uintptr_t end, bool is_set)
{
	size_t start_idx = (start - g_mem_info.min_addr) >> PAGE_BITS;
	size_t end_idx = (end - g_mem_info.min_addr) >> PAGE_BITS;
	for (size_t i = start_idx; i < end_idx; i++) {
		if (is_set) {
			g_mem_info.bitmaps[i >> 3] |= (1UL << (i & 0x7));
		} else {
			g_mem_info.bitmaps[i >> 3] &= ~(1UL << (i & 0x7));
		}
	}
}

void bootmem_init(struct boot_args *args)
{
	extern char _end[];
	uintptr_t phy_kernel_end = __VA_PA__((uintptr_t)_end);

	memset(&g_mem_info, 0, sizeof(struct mem_info));
	g_mem_info.min_addr = (uintptr_t)-1;

	bootmem_add_bank(PHY_RAM_BASE, PHY_RAM_END, MEM_BANK_FREE);
	bootmem_add_bank(PHY_RAM_BASE, phy_kernel_end, MEM_BANK_RESERVE);

	/* copy rootfs */
	memcpy((void *)VIRT_RAMDISK_BASE, (void *)__PA_VA__(args->rootfs_start), args->rootfs_size);
	bootmem_add_bank(PHY_RAMDISK_BASE, PHY_RAMDISK_BASE + args->rootfs_size, MEM_BANK_RESERVE);

	size_t nbits = (g_mem_info.max_addr - g_mem_info.min_addr) >> PAGE_BITS;
	g_mem_info.bitmaps = _end;
	g_mem_info.bitmap_bytes = nbits / BYTES_PER_UCHAR;
	bootmem_add_bank(phy_kernel_end , phy_kernel_end + g_mem_info.bitmap_bytes, MEM_BANK_RESERVE);

	memset((void*)g_mem_info.bitmaps, 0xff, g_mem_info.bitmap_bytes);
	for (size_t i = 0; i < g_mem_info.nr_banks; i++) {
		struct mem_bank *bank = &g_mem_info.banks[i];
		bitmap_set(bank->start, bank->end, bank->tag == MEM_BANK_FREE ? 0 : 1);
	}
}

void *bootmem_alloc(void)
{
	uintptr_t paddr = 0;
	size_t i = 0;
	while (i < g_mem_info.bitmap_bytes && (g_mem_info.bitmaps[i] & 0xff) == 0xff) {
		i++;
	}
	if (i != g_mem_info.bitmap_bytes) {
		size_t idx = i * BYTES_PER_UCHAR + __builtin_ctz(~g_mem_info.bitmaps[i]);
		paddr = g_mem_info.min_addr + idx * PAGE_SIZE;
	}
	bitmap_set(paddr, paddr + PAGE_SIZE, 1);
	return (void *)paddr;
}

void bootmem_free(uintptr_t paddr)
{
	bitmap_set(paddr, paddr + PAGE_SIZE, 0);
}
