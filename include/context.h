#ifndef __CONTEXT_H
#define __CONTEXT_H

#include "arch.h"
#include "types.h"

struct task_struct;

struct sys_regs {
	union {
		struct {
			size_t r0;
			size_t r1;
			size_t r2;
			size_t r3;
			size_t r4;
			size_t r5;
			size_t r6;
			size_t r7;
			size_t r8;
			size_t r9;
			size_t r10;
			size_t r11;
			size_t r12;
			uintptr_t sp;
			uintptr_t lr;
			uintptr_t pc;
			size_t spsr;
		};
		size_t r[REG_NUM];
	};
};

struct context {
	struct sys_regs regs;
	struct task_struct *task;
};

#endif
