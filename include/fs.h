#ifndef __FS_H
#define __FS_H

#define S_IXOTH		0x1
#define S_IWOTH		0x2
#define S_IROTH		0x4
#define S_IXGRP		0x8
#define S_IWGRP		0x10
#define S_IRGRP		0x20
#define S_IXUSR		0x40
#define S_IWUSR		0x80
#define S_IRUSR		0x100
#define S_ISVTX		0x200
#define S_ISGID		0x400
#define S_ISUID		0x800
#define S_IFIFO		0x1000
#define S_IFCHR		0x2000
#define S_IFDIR		0x4000
#define S_IFBLK		0x6000
#define S_IFREG		0x8000
#define S_IFLNK		0xA000
#define S_IFSOCK	0xC000

void fs_init(void);

#endif
