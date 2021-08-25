#include "string.h"

int strlen(const char *str)
{
	int len = 0;
	while (str && *str++ != '\0') {
		len++;
	}
	return len;
}

char *strcpy(char *dst, const char *src)
{
	char *res = dst;
	while (src && *src != '\0') {
		*dst++ = *src++;
	}
	return res;
}

void *memcpy(void *dst, void *src, size_t n)
{
	for (size_t i = 0; i < n; i++) {
		*((char*)dst + i) = *((char *)src + i);
	}
}

void *memset(void *str, int c, size_t n)
{
	for (size_t i = 0; i < n; i++) {
		*((char*)str + i) = (char)c;
	}
}

int vsprintf(char *str, const char *format, va_list args)
{
	const char *p = format;
	while (p && *p != '\0') {
		if (*p != '%') {
			*str++ = *p++;
			continue;
		}
		p++;
		if (p && *p != 's') {
			/* TODO: */
			return -1;
		}
		char *tmp = va_arg(args, char *);
		while (tmp && *tmp != '\0') {
			*str++ = *tmp++;
		}
		p++;
	}
	*str = '\0';
	return 0;
}
