#ifndef __TRAP_H
#define __TRAP_H

struct handler_vector {
	struct task_struct *(*not_used_hdl)();
	struct task_struct *(*und_ins_hdl)();
	struct task_struct *(*svc_hdl)();
	struct task_struct *(*prefetch_abort_hdl)();
	struct task_struct *(*dabt_hdl)();
	struct task_struct *(*hyp_trap_hdl)();
	struct task_struct *(*irq_hdl)();
	struct task_struct *(*fiq_hdl)();
};

void trap_init(void);
#endif
