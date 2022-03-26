#ifndef __DEBUG_H
#define __DEBUG_H
#include "printk.h"

#define BUG_ON(condition)						\
do {									\
	if (condition) {						\
		printk("ERROR:%s:%d\n",  __func__, __LINE__);		\
		asm("b .");						\
	}								\
} while (0)

#endif
