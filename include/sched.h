#ifndef __SCHED_H
#define __SCHED_H

#include "task.h"

extern void __switch_to(struct context *prev, struct context *next);
void schedule(void);

#endif
