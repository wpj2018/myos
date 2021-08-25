#ifndef __TRAP_H
#define __TRAP_H

struct handler_vector {
	void (*not_used_hdl)(void);
	void (*und_ins_hdl)(void);
	void (*hyp_call_hdl)(void);
	void (*prefetch_abort_hdl)(void);
	void (*data_abort_hdl)(void);
	void (*hyp_trap_hdl)(void);
	void (*irq_hdl)(void);
	void (*fiq_hdl)(void);
};

void trap_init(void);
#endif
