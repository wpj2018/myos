#include "string.h"

int strcmp(const char *s, const char *t)
{
	while (*s && *t && (*s == *t)) {
		s++;
		t++;
	}
	return (*s != *t);
}

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

void swap(char *buf, size_t n)
{
	for (size_t i = 0; i < n / 2; i++) {
		char c = buf[i];
		buf[i] = buf[n - 1 - i];
		buf[n - 1 - i] = c;
	}
}

char *tostr(char *buf, size_t n)
{
	size_t cnt = 0;
	if (n == 0) {
		buf[cnt++] = '0';
	}
	while (n) {
		buf[cnt++] = n % 10 + '0';
		n /= 10;
	}
	swap(buf, cnt);
	buf[cnt] = '\0';
	return buf;
}

int vsprintf(char *str, const char *format, va_list args)
{
	char buf[MAX_BUF_LEN];
	const char *p = format;
	char *tmp;

	while (p && *p != '\0') {
		if (*p != '%') {
			*str++ = *p++;
			continue;
		}
		p++;
		if (!p) {
			break;
		}
		if (*p != 's' && *p != 'd') {
			/* TODO: */
			return -1;
		}
		if (*p == 'd') {
			tmp = tostr(&buf[0], va_arg(args, int));
		} else {
			tmp = va_arg(args, char *);
		}
		while (tmp && *tmp != '\0') {
			*str++ = *tmp++;
		}
		p++;
	}
	*str = '\0';
	return 0;
}
