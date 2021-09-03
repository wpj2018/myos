#ifndef __TASK_H
#define __TASK_H

#include "context.h"

struct task_struct {
	union {
		struct context *ctx;
		uchar *stack;
	};
	size_t ticks;
};

struct task_struct *get_current();
struct task_struct *task_create(void *func);
void task_init(void);

#define current (get_current())

#endif
