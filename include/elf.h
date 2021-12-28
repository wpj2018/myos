#ifndef __ELF_H
#define __ELF_H

#include "types.h"

#define EI_NIDENT	16
#define PT_NULL		0
#define PT_LOAD		1
#define PT_DYNAMIC	2
#define PT_INTERP	3
#define PT_NOTE		4
#define PT_SHLIB	5
#define PT_PHDR		6
#define PT_TLS		7
#define PT_LOOS		0x60000000
#define PT_HIOS		0x6fffffff
#define PT_LOPROC	0x70000000
#define PT_HIPROC	0x7fffffff

typedef unsigned short elf32_half;
typedef unsigned int elf32_word;
typedef unsigned int elf32_off;
typedef unsigned long elf32_addr;

struct elf32_ehdr {
	unsigned char	e_ident[EI_NIDENT];
	elf32_half	e_type;
	elf32_half	e_machine;
	elf32_word	e_version;
	elf32_addr	e_entry;
	elf32_off	e_phoff;
	elf32_off	e_shoff;
	elf32_word	e_flags;
	elf32_half	e_ehsize;
	elf32_half	e_phentsize;
	elf32_half	e_phnum;
	elf32_half	e_shentsize;
	elf32_half	e_shnum;
	elf32_half	e_shstrndx;
};

struct elf32_phdr {
	elf32_word	p_type;
	elf32_off	p_offset;
	elf32_addr	p_vaddr;
	elf32_addr	p_paddr;
	elf32_word	p_filesz;
	elf32_word	p_memsz;
	elf32_word	p_flags;
	elf32_word	p_align;
};

void elf_parse_ehdr(void *addr, struct elf32_ehdr *ehdr);
void elf_parse_phdr(void *addr, struct elf32_phdr *phdr);
void elf_print_ehdr(struct elf32_ehdr *ehdr);
void elf_print_phdr(struct elf32_phdr *phdr, size_t idx);

#endif
