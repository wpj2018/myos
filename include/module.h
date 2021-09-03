#ifndef __MODULE_H
#define __MODULE_H

typedef void (*init_call_t)(void);

#define __init_module__(fn)						\
	static init_call_t __init_call_##fn \
		__attribute__((__section__(".module.init"))) = fn	\

void module_init(void);

#endif
