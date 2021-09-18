#include "kernel.h"
#include "mm.h"
#include "task.h"

uchar init_stack[TASK_STACK_SIZE] __attribute__((__section__(".init.stack")));

int task_comp(struct rb_node *n1, struct rb_node *n2)
{
	struct task_struct *t1 = container_of(n1, struct task_struct, rb_node);
	struct task_struct *t2 = container_of(n2, struct task_struct, rb_node);
	if (t1->ticks != t2->ticks) {
		return t1->ticks < t2->ticks;
	}
	return (uintptr_t)n1 > (uintptr_t)n2;
}

struct rb_tree init_rbtree = {
	.root = NULL,
	.comp = task_comp,
};

struct task_struct init_task = {
	.stack = &init_stack[0],
	.rb_tree = &init_rbtree,
};

size_t g_task_cnt = 1;
struct task_struct g_task_list[3];

void task1(void)
{
	while (1) {
		printk("run task1\n");
	}
}

void task2(void)
{
	while (1) {
		printk("run task2\n");
	}
}

struct task_struct *task_create(char *name, void *func)
{
	struct task_struct *task = &g_task_list[g_task_cnt];
	struct context *ctx = (struct context *)vmalloc(TASK_STACK_SIZE);

	task->rb_tree = &init_rbtree;
	task->ctx = ctx;
	task->ticks = 10;
	task->name = name;

	rb_insert(task->rb_tree, &task->rb_node);

	ctx->regs.pc = (uintptr_t)func;
	ctx->task = task;
	ctx->regs.sp = (uintptr_t)(&task->stack[TASK_STACK_SIZE]);

	g_task_cnt++;

	return task;
}

struct task_struct *get_current(void)
{
	struct task_struct *task;
	register uintptr_t sp asm ("sp");

	task = ((struct context *)(sp & ~(TASK_STACK_SIZE - 1)))->task;

	return task;
}

void task_init(void)
{
	init_task.ctx->task = &init_task;
	rb_insert(init_task.rb_tree, &init_task.rb_node);

	task_create("task1", &task1);
	task_create("task2", &task2);
}
