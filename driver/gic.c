#include "module.h"
#include "mm.h"
#include "gic.h"

struct gic_dist *g_gic_dist = NULL;
struct gic_cpu *g_gic_cpu = NULL;

void gic_init(void)
{
	printk("gic_init\n");

	g_gic_dist = (struct gic_dist *)ioremap(PHYS_GICD_BASE);

	size_t it_line_num = (1 + g_gic_dist->type & 0x01f) * 32;
	for (size_t i = 0; i < it_line_num; i++) {
		g_gic_dist->set_enable[i / 32] |= (1 << (i % 32));
		g_gic_dist->clear_pend[i / 32] |= (1 << (i % 32));
		g_gic_dist->priority[i] = 0xA0;
	}
	g_gic_dist->ctrl = 0x1;

	g_gic_cpu = (struct gic_cpu *)ioremap(PHYS_GICC_BASE);
	g_gic_cpu->ctrl |= 0x1;
	g_gic_cpu->pm = 0xF0;
}

size_t gicc_get_ack(void)
{
	return g_gic_cpu->ack;
}

void gicc_set_eoi(size_t ack_no)
{
	g_gic_cpu->eoi = ack_no;
}

__init_module__(gic_init);
