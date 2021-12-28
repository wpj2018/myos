#include "string.h"
#include "kernel.h"
#include "elf.h"

void elf_parse_ehdr(void *addr, struct elf32_ehdr *ehdr)
{
	memcpy(ehdr, addr, sizeof(struct elf32_ehdr));
}

void elf_parse_phdr(void *addr, struct elf32_phdr *phdr)
{
	memcpy(phdr, addr, sizeof(struct elf32_ehdr));
}

void elf_print_ehdr(struct elf32_ehdr *ehdr)
{
	printk("=============ELF HEADER=============\n");
	printk("e_ident     : %s\n", &ehdr->e_ident[1]);
	printk("e_type      : %d\n", ehdr->e_type);
	printk("e_machine   : %d\n", ehdr->e_machine);
	printk("e_version   : %d\n", ehdr->e_version);
	printk("e_entry     : %x\n", ehdr->e_entry);
	printk("e_phoff     : %d\n", ehdr->e_phoff);
	printk("e_shoff     : %d\n", ehdr->e_shoff);
	printk("e_flags     : %d\n", ehdr->e_flags);
	printk("e_ehsize    : %d\n", ehdr->e_ehsize);
	printk("e_phentsize : %d\n", ehdr->e_phentsize);
	printk("e_phnum     : %d\n", ehdr->e_phnum);
	printk("e_shentsize : %d\n", ehdr->e_shentsize);
	printk("e_shnum     : %d\n", ehdr->e_shnum);
	printk("e_shstrndx  : %d\n", ehdr->e_shstrndx);
}

void elf_print_phdr(struct elf32_phdr *phdr, size_t idx)
{
	printk("=============ELF PROGRAM HEADER %d=============\n", idx);
	printk("p_type      : %d\n", phdr->p_type);
	printk("p_offset    : %d\n", phdr->p_offset);
	printk("p_vaddr     : %x\n", phdr->p_vaddr);
	printk("p_paddr     : %x\n", phdr->p_paddr);
	printk("p_filesz    : %d\n", phdr->p_filesz);
	printk("p_memsz     : %d\n", phdr->p_memsz);
	printk("p_flags     : %d\n", phdr->p_flags);
	printk("p_align     : %d\n", phdr->p_align);
}
