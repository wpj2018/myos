#include "stdio.h"
#include "syscall.h"
#include "string.h"

int printf(const char *format, ...)
{
	int ret;
	va_list args;
	char buf[MAX_BUF_LEN] = {0};

	va_start(args, format);
	ret = vsprintf(buf, format, args);
	va_end(args);
	asm volatile (
		"mov r0, %0\n"
		"svc %1\n"
		:
		:
		"r"(&buf[0]),
		"I"(__NR_syslog)
	);

	return 0;
}
