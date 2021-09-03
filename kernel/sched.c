#include "kernel.h"
#include "task.h"
#include "sched.h"

extern struct task_struct init_task;
extern struct task_struct g_task_list[3];
extern size_t g_task_cnt;

struct task_struct *sched_next()
{
	struct task_struct *task = NULL;
	size_t idx = 0;

	for (int i = 1; i < g_task_cnt; i++) {
		if (g_task_list[i].ticks > g_task_list[idx].ticks) {
			idx = i;
		}
	}
	printk("task id= %d ticks = %d\n", idx, g_task_list[idx].ticks);
	if (idx == 0) {
		for (size_t i = 1; i < g_task_cnt; i++) {
			g_task_list[i].ticks = 10;
		}
		task = &init_task;
	} else {
		task = &g_task_list[idx];
		task->ticks-- ;
	}
	return task;
}
