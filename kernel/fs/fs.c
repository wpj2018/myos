#include "ext4.h"
#include "mm.h"

void fs_init(void)
{
	ext4_init();
}

void vfs_read(char *filename, char *buf, size_t count)
{
	ext4_read(filename, buf, count);
}

void vfs_stat(char *filename, struct vfs_stat *stat)
{
	ext4_stat(filename, stat);
}
