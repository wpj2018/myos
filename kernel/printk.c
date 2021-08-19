#include "string.h"

volatile char *g_uart = (volatile char *)0x10009000;
extern int vsprintf(char *, const char *fmt, va_list args);

void put_uart_char(const char c)
{
	*g_uart = c;
}

void put_uart(const char *str)
{
	const char *p = str;
	while (*p) {
		put_uart_char(*p++);
	}
}

int printk(const char *fmt, ...)
{
	int ret;
	va_list args;
	char buf[100] = {0};

	va_start(args, fmt);
	ret = vsprintf(buf, fmt, args);
	va_end(args);
	put_uart(buf);

	return ret;
}
