#ifndef __TYPES_H
#define __TYPES_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stddef.h>

typedef unsigned int size_t;
typedef unsigned char uchar;
typedef int bool;

typedef unsigned long long __u64;
typedef unsigned int __u32;
typedef unsigned short __u16;
typedef unsigned char __u8;

typedef __u16  __le16;
typedef __u32  __le32;
typedef __u64  __le64;

#define NULL ((void *)0)
#define null ((void *)0)
#define false 0
#define true  1

#define BITS_PER_BYTE		(8UL)

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})


#endif
