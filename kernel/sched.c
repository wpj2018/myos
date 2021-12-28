#include "kernel.h"
#include "task.h"
#include "rbtree.h"
#include "sched.h"
#include "preempt.h"
#include "printk.h"

struct task_struct *sched_next()
{
	if (current->preempt_cnt > 0) {
		return current;
	}
	struct task_struct *task = NULL;
	struct rb_node *rb_node = rb_max_node(current->rb_tree);

	task = container_of(rb_node, struct task_struct, rb_node);

//	printk("task name = %s ticks = %d\n", task->name, task->ticks);
	if (task->ticks > 0) {
		task->ticks--;
		rb_update(current->rb_tree, rb_node);
	}

	return task;
}
