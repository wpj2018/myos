#ifndef __TASK_H
#define __TASK_H

#include "context.h"
#include "rbtree.h"

extern struct task_struct init_task;

union ctx_union {
	struct context ctx;
	uchar stack[TASK_STACK_SIZE];
};

struct task_struct {
	union {
		struct context *ctx;
		uchar *stack;
	};
	size_t *pgd;
	struct rb_tree *rb_tree;
	struct rb_node rb_node;
	size_t ticks;
	int preempt_cnt;
	char *name;
};

struct task_struct *get_current();
struct task_struct *task_create(char *name);
void run_task(struct task_struct *task, void *entry);
void task_init(void);

#define current (get_current())

#endif
