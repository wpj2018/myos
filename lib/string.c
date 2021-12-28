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

char *to_hex(char *buf, size_t n)
{
	size_t cnt = 0;
	int c;

	do {
		c = n & 0x0f;
		if (c >= 10) {
			buf[cnt++] = c - 10 + 'A';
		} else {
			buf[cnt++] = c + '0';
		}
		n >>= 4;
	} while (n);

	buf[cnt++] = 'x';
	buf[cnt++] = '0';

	swap(buf, cnt);
	buf[cnt] = '\0';
	return buf;
}

char *to_decimal(char *buf, size_t n)
{
	size_t cnt = 0;

	do {
		buf[cnt++] = n % 10 + '0';
		n /= 10;
	} while (n);

	swap(buf, cnt);
	buf[cnt] = '\0';
	return buf;
}

char *tostr(char *buf, size_t n, size_t base)
{
	if (base == 16) {
		return to_hex(buf, n);
	}
	return to_decimal(buf, n);
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
		if (*p == 's') {
			tmp = va_arg(args, char *);
		} else {
			tmp = tostr(&buf[0], va_arg(args, int), *p == 'x' ? 16 : 10);
		}
		while (tmp && *tmp != '\0') {
			*str++ = *tmp++;
		}
		p++;
	}
	*str = '\0';
	return 0;
}
