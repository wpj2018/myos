#ifndef __TIMER_H
#define __TIMER_H

#define TIMER_BASE 0x100e4000

struct timer {
	volatile size_t load1;
	volatile size_t value1;
	volatile size_t ctrl1;
	volatile size_t clr1;
	volatile size_t ris1;
	volatile size_t mis1;
	volatile size_t res[];
};

void timer_clear_int1(void);
size_t timer_get_value1(void);
size_t timer_get_ris1(void);
size_t timer_get_mis1(void);

#endif
