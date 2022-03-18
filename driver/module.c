#include "module.h"

extern init_call_t __module_start[];
extern init_call_t __module_end[];

void module_init(void)
{
	for (init_call_t *call = __module_start; call < __module_end; call++)
	{
		(*call)();
	}
}
