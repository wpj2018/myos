#ifndef __STRING_H
#define __STRING_H

#include "types.h"

#define MAX_BUF_LEN 128

int strlen(const char *str);
char *strcpy(char *dst, const char *src);
int vsprintf(char *str, const char *format, va_list args);
void *memset(void *str, int c, size_t n);
void *memcpy(void *dst, void *src, size_t n);

#endif
