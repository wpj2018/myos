#include "string.h"
#include "elf.h"
#include "fs.h"
#include "mm.h"
#include "task.h"
#include "proc.h"

void proc_map_phdr(struct task_struct *task, struct elf32_phdr *phdr, void *buf)
{
	size_t mem_sz = PAGE_ALIGN(phdr->p_memsz);
	size_t file_sz = phdr->p_filesz;
	uintptr_t vaddr = phdr->p_vaddr;
	struct page *pages = buddy_alloc_pages(mem_sz);
	size_t nr_pages = (1 << pages->order);

	memcpy((void *)page_to_virt(pages), buf + phdr->p_offset, file_sz);
	for (size_t i = 0; i < nr_pages; i++, vaddr += PAGE_SIZE) {
		map_page(task->pgd, vaddr, page_to_phy(&pages[i]), PT_AP_RW);
	}
}

void run_init_process(char *init_name)
{
	struct vfs_stat stat;
	struct elf32_ehdr ehdr;
	struct elf32_phdr phdr;
	struct task_struct *task;
	void *buf;
	void *ptr;

	task = task_create("init");

	vfs_stat(init_name, &stat);
	buf = (void *)kzalloc(stat.st_size);
	PANIC(buf == NULL, "kzalloc failed");

	vfs_read(init_name, buf, stat.st_size);
	elf_parse_ehdr(buf, &ehdr);
	elf_print_ehdr(&ehdr);

	ptr = buf + ehdr.e_phoff;
	for (size_t i = 0; i < ehdr.e_phnum; i++) {
		elf_parse_phdr(ptr, &phdr);
		elf_print_phdr(&phdr, i);
		if (phdr.p_type == PT_LOAD) {
			proc_map_phdr(task, &phdr, buf);
		}
		ptr += sizeof(struct elf32_phdr);
	}
	kfree(buf);
	run_task(task, (void *)ehdr.e_entry);
}
