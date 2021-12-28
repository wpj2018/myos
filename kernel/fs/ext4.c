#include "kernel.h"
#include "string.h"
#include "mm.h"
#include "ext4.h"

size_t ext4_get_bsize_bits(struct ext4_super_block *sb)
{
	return 10 + sb->s_log_block_size;
}

size_t ext4_get_bsize(struct ext4_super_block *sb)
{
	return (1 << (10 + sb->s_log_block_size));
}

struct ext4_inode *ext4_get_itable(struct ext4_super_block *sb)
{
	size_t bsize = ext4_get_bsize(sb);
	uintptr_t base = (uintptr_t)sb - EXT4_SB_OFFSET;
	struct ext4_group_desc *gdt = (struct ext4_group_desc *)(base + bsize * 1);

	return (struct ext4_inode *)(base  + bsize * gdt->bg_inode_table_lo);
}

void *ext4_get_block_ptr(struct ext4_super_block *sb, size_t bno)
{
	size_t bsize = ext4_get_bsize(sb);
	uintptr_t base = (uintptr_t)sb - EXT4_SB_OFFSET;

	return  (void *)(base  + bsize * bno);
}

struct ext4_dentry *ext4_get_next_dentry(struct ext4_dentry *cur)
{
	struct ext4_dentry *next;

	next = (struct ext4_dentry *)((uintptr_t)cur + cur->rec_len);

	return next;
}

struct ext4_dentry *ext4_lookup_dentry(struct ext4_super_block *sb, void *bptr, char *name)
{
	struct ext4_dentry  *dentry = NULL;
	char buf[MAX_BUF_LEN];

	struct ext4_dentry *tmp = (struct ext4_dentry *)bptr;

	while (tmp->inode) {
		memcpy(buf, tmp->name, tmp->name_len);
		buf[tmp->name_len] = '\0';
		if (name && !strcmp(buf, name)) {
			dentry = tmp;
			break;
		}
		tmp = ext4_get_next_dentry(tmp);
	}

	return dentry;
}

struct ext4_node *ext4_bsearch(struct ext4_node *nodes, size_t size, size_t val)
{
	int l = 0;
	int r = size - 1;

	while (l <= r) {
		size_t mid = l + ((r - l) >> 1);
		size_t min_v = nodes[mid].leaf.ee_block;
		size_t max_v = min_v + nodes[mid].leaf.ee_len - 1;

		if (min_v > val) {
			r = mid - 1;
		} else if (max_v < val) {
			l = mid + 1;
		} else {
			return &nodes[mid];
		}
	}
	return NULL;
}

struct ext4_node *ext4_get_node(struct ext4_super_block *sb, struct ext4_extent_header *hdr, size_t logic_bno)
{
	struct ext4_node *res = NULL;
	struct ext4_node *nodes = (struct ext4_node *)&hdr[1];
	void *block = NULL;
	size_t start_bno = 0;
	size_t nblocks = 0;

	if (!hdr->eh_depth) {
		return ext4_bsearch(nodes, hdr->eh_entries, logic_bno);
	}

	for (size_t i = 0; i < hdr->eh_entries; i++) {
		start_bno = nodes[i].inter.ei_leaf_lo;
		block = ext4_get_block_ptr(sb, start_bno);
		res = ext4_get_node(sb, block, logic_bno);
		if (res) {
			break;
		}
	}
	return res;
}

size_t ext4_get_real_bno(struct ext4_super_block *sb, struct ext4_inode *inode, size_t logic_bno)
{
	size_t real_bno = -1;
	struct ext4_extent_header *hdr = (struct ext4_extent_header *)(&inode->i_block[0]);
	struct ext4_node *node = ext4_get_node(sb, hdr, logic_bno);

	if (node) {
		real_bno = node->leaf.ee_start_lo + logic_bno - node->leaf.ee_block;
	}

	return real_bno;
}

struct ext4_dentry *ext4_get_dentry(struct ext4_super_block *sb, struct ext4_inode *inode, char *name)
{
	struct ext4_dentry *dentry = NULL;
	void *bptr = NULL;
	size_t bsize_bits = ext4_get_bsize_bits(sb);
	size_t bsize = ext4_get_bsize(sb);

	size_t nblocks = (inode->i_size_lo + bsize - 1) >> bsize_bits;
	size_t real_bno = -1;

	for (size_t i = 0; i < nblocks; i++) {
		real_bno = ext4_get_real_bno(sb, inode, i);
		if (real_bno == -1) {
			continue;
		}

		bptr = ext4_get_block_ptr(sb, real_bno);
		dentry = ext4_lookup_dentry(sb, bptr, name);
		if (dentry) {
			break;
		}
	}

	return dentry;
}

void ext4_read_file(struct ext4_super_block *sb, struct ext4_inode *inode, void *buf, size_t pos, size_t count)
{
	size_t bsize_bits = ext4_get_bsize_bits(sb);
	size_t bsize = ext4_get_bsize(sb);

	size_t nblocks = (count + bsize - 1) >> bsize_bits;
	size_t real_bno = -1;

	void *bptr = NULL;
	size_t off = 0;
	size_t size = 0;

	for (size_t i = 0; i < nblocks; i++) {
		real_bno = ext4_get_real_bno(sb, inode, pos >> bsize_bits);
		if (real_bno == -1) {
			continue;
		}
		bptr = ext4_get_block_ptr(sb, real_bno);
		off = pos - (pos >> bsize_bits << bsize_bits); // off = pos % bsize;
		size = count < bsize - off ? count : bsize - off;
		memcpy(buf, bptr + off, size);

		buf += size;
		pos += size;
		count -= size;
		if (!count) {
			break;
		}
	}
}

void ext4_init(void)
{
	struct ext4_super_block *sb = (struct ext4_super_block *)(VIRT_RAMDISK_BASE + EXT4_SB_OFFSET);
	struct ext4_inode *itables = ext4_get_itable(sb);
	struct ext4_inode *root = &itables[EXT4_ROOT_INO - 1];
	struct ext4_dentry *dentry = ext4_get_dentry(sb, root, "verify.bin");
	struct ext4_inode *inode = &itables[dentry->inode - 1];
	char *buf = kalloc(PAGE_SIZE);

	for (size_t i = 0; i < inode->i_size_lo; i += 4096) {
		ext4_read_file(sb, inode, buf, i, 4096);
		size_t *tmp = (size_t *)&buf[0];
		for (size_t j = 0; j < 1024; j++) {
			if (*tmp != (i >> 2) + j) {
				printk("ext4 check failed!, i = %d j = %d v = %d\n", i, j, *tmp);
				return;
			}
			tmp++;
		}
	}
	printk("ext4 check success !!!\n");
}

void ext4_stat(char *filename, struct vfs_stat *stat)
{
	struct ext4_super_block *sb = (struct ext4_super_block *)(VIRT_RAMDISK_BASE + EXT4_SB_OFFSET);
	struct ext4_inode *itables = ext4_get_itable(sb);
	struct ext4_inode *root = &itables[EXT4_ROOT_INO - 1];
	struct ext4_dentry *dentry = ext4_get_dentry(sb, root, filename);
	struct ext4_inode *inode = &itables[dentry->inode - 1];

	stat->st_size = inode->i_size_lo;
}

void ext4_read(char *filename, char *buf, size_t count)
{
	struct ext4_super_block *sb = (struct ext4_super_block *)(VIRT_RAMDISK_BASE + EXT4_SB_OFFSET);
	struct ext4_inode *itables = ext4_get_itable(sb);
	struct ext4_inode *root = &itables[EXT4_ROOT_INO - 1];
	struct ext4_dentry *dentry = ext4_get_dentry(sb, root, filename);
	struct ext4_inode *inode = &itables[dentry->inode - 1];

	for (size_t i = 0; i < inode->i_size_lo && i < count; i += 4096) {
		ext4_read_file(sb, inode, &buf[i], i, 4096);
	}
}
