#ifndef __SYSCALL_H
#define __SYSCALL_H

#define printf(x)	asm("svc #0")

#endif
