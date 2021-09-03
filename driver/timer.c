#include "module.h"
#include "mm.h"
#include "timer.h"

struct timer *g_timer;

void timer_init(void)
{
	printk("timer_init\n");
	g_timer = ioremap(TIMER_BASE);
	g_timer->load1 = 1024;
	g_timer->ctrl1 |= (1 << 6 | 1 << 7);
}

size_t timer_get_value1(void)
{
	return g_timer->value1;
}

size_t timer_get_ris1(void)
{
	return g_timer->ris1 & 0x1;
}

size_t timer_get_mis1(void)
{
	return g_timer->mis1 & 0x1;
}

void timer_clear_int1(void)
{
	g_timer->clr1 = 1;
}

__init_module__(timer_init);
