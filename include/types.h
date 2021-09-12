#ifndef __TYPES_H
#define __TYPES_H

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

#define BYTES_PER_UCHAR		(8UL)

#endif
