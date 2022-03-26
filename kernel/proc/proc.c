#include "string.h"
#include "elf.h"
#include "fs.h"
#include "mm.h"
#include "task.h"
#include "proc.h"

void *proc_map_stack(struct task_struct *task, size_t stack_size)
{
	uintptr_t vaddr = (uintptr_t)(0x80000000);
	uintptr_t kaddr = (uintptr_t)kzalloc(stack_size);
	map_user_page(task->ctx->pgd, vaddr, __VA_PA__(kaddr), PT_AP_RW);
	return (void *)(vaddr + stack_size);
}

void proc_map_phdr(struct task_struct *task, struct elf32_phdr *phdr, void *buf)
{
	size_t mem_sz = PAGE_ALIGN(phdr->p_memsz);
	size_t file_sz = phdr->p_filesz;
	uintptr_t vaddr = phdr->p_vaddr;
	struct page *pages = buddy_alloc_pages(mem_sz);
	size_t nr_pages = (1 << pages->order);

	memcpy((void *)page_to_virt(pages), buf + phdr->p_offset, file_sz);
	for (size_t i = 0; i < nr_pages; i++, vaddr += PAGE_SIZE) {
		map_user_page(task->ctx->pgd, vaddr, page_to_phy(&pages[i]), PT_AP_RW);
	}
}

void do_spawn(char *proc_name)
{
	struct vfs_stat stat;
	struct elf32_ehdr ehdr;
	struct elf32_phdr phdr;
	struct task_struct *task;
	void *buf;
	void *ptr;

	task = task_create(proc_name);

	vfs_stat(proc_name, &stat);
	buf = (void *)kzalloc(stat.st_size);
	BUG_ON(buf == NULL);

	vfs_read(proc_name, buf, stat.st_size);
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

	void *stack = proc_map_stack(task, TASK_STACK_SIZE);
	run_task(task, (void *)ehdr.e_entry, (void *)stack);
}

void run_init_process(char *init_name)
{
	do_spawn(init_name);
}
