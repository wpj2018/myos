#ifndef __ARCH_H
#define __ARCH_H

#define MASK_A		(1 << 8)
#define MASK_I		(1 << 7)
#define MASK_F		(1 << 6)

#define MODE_USER	0b10000
#define MODE_FIQ	0b10001
#define MODE_IRQ	0b10010
#define MODE_SVC	0b10011
#define MODE_MON	0b10110
#define MODE_ABT	0b10111
#define MODE_HYP	0b11010
#define MODE_UND	0b11011
#define MODE_SYS	0b11111

#define REG_NUM		17
#define TASK_STACK_SIZE	4096

#endif
