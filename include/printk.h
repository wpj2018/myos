#ifndef __PRINTK_H
#define __PRINTK_H

int printk(const char *fmt, ...);
#define PANIC(condition, msg) do 					\
{									\
	if (condition) {						\
		printk("ERROR:%s:%d, %s\n",  __func__, __LINE__, msg);	\
		asm("b .");						\
	}								\
} while (0)

#endif
