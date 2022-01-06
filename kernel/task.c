#include "kernel.h"
#include "string.h"
#include "mm.h"
#include "task.h"
#include "preempt.h"

extern void ret_to_usr(void);

struct context init_ctx __attribute__((__section__(".init.stack"))) = {
	.task = &init_task,
	.pgd = (size_t *)VIRT_PT_BASE,
};

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
	.root = &init_task.rb_node,
	.comp = task_comp,
};

struct task_struct init_task = {
	.name = "init",
	.ctx = &init_ctx,
	.rb_tree = &init_rbtree,
	.rb_node.color = RB_COLOR_BLACK,
	.rb_node.parent = NULL,
	.rb_node.left = NULL,
	.rb_node.right = NULL,
};

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

struct task_struct *task_create(char *name)
{
	struct task_struct *task = (struct task_struct *)kzalloc(sizeof(struct task_struct));
	struct context *ctx = (struct context *)kalloc(TASK_STACK_SIZE);

	ctx->pgd = kzalloc(4 * PAGE_SIZE);
	memcpy(ctx->pgd, init_ctx.pgd, 4 * PAGE_SIZE);
	memset(&ctx->regs, 0, sizeof(ctx->regs));
	ctx->task = task;

	task->ctx = ctx;
	task->ticks = 10;
	task->name = name;

	return task;
}

void run_task(struct task_struct *task, void *entry, void *stack)
{
	struct context *ctx = task->ctx;
	ctx->regs.sp = (uintptr_t)&ctx->stack[0];
	ctx->regs.lr = (uintptr_t)ret_to_usr;
	ctx->regs.spsr = MODE_SVC | MASK_I | MASK_F;

	struct sys_regs *uregs = (struct sys_regs *)(ctx->regs.sp - REG_NUM * 4);
	uregs->sp = (uintptr_t)stack;
	uregs->pc = (uintptr_t)entry;
	uregs->spsr = MODE_USR;

	preempt_disable();
	task->rb_tree = &init_rbtree;
	rb_insert(&init_rbtree, &task->rb_node);
	preempt_enable();
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
	struct task_struct *t1 = task_create("task1");
	struct task_struct *t2 = task_create("task2");
	run_task(t1, &task1, NULL);
	run_task(t2, &task2, NULL);
}
