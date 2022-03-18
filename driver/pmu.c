#include "module.h"
#include "pmu.h"
#include "string.h"

void pmu_write_PMUSERENR(size_t val)
{
	asm(
		"mcr p15, 0, %0, c9, c14, 0\n"
		:
		:"r"(val)
	);
}

void pmu_read_PMUSERENR(size_t *val)
{
	size_t tmp = 0;
	asm(
		"mrc p15, 0, %0, c9, c14, 0\n"
		:"=r"(tmp)
		:
	);
	*val = tmp;
}

void pmu_write_PMCR(size_t val)
{
	asm(
		"mcr p15, 0, %0, c9, c12, 0\n"
		:
		:"r"(val)
	);
}

void pmu_read_PMCR(size_t *val)
{
	size_t tmp = 0;
	asm(
		"mrc p15, 0, %0, c9, c12, 0\n"
		:"=r"(tmp)
		:
	);
	*val = tmp;
}

void pmu_write_PMCNTENSET(size_t val)
{
	asm(
		"mcr p15, 0, %0, c9, c12, 1\n"
		:
		:"r"(val)
	);
}

void pmu_read_PMCNTENSET(size_t *val)
{
	size_t tmp = 0;
	asm(
		"mrc p15, 0, %0, c9, c12, 1\n"
		:"=r"(tmp)
		:
	);
	*val = tmp;
}

void pmu_write_PMCCNTR(size_t val)
{
	asm(
		"mcr p15, 0, %0, c9, c13, 0\n"
		:
		:"r"(val)
	);
}

void pmu_read_PMCCNTR(size_t *val)
{
	size_t tmp = 0;
	asm(
		"mrc p15, 0, %0, c9, c13, 0\n"
		:"=r"(tmp)
		:
	);
	*val = tmp;
}

/* qemu cortex-a9 read pmccntr fail */
/* qemu cortex-a7 read pmccntr ok */
void pmu_init(void)
{
	size_t val = 0;
	pmu_read_PMCR(&val);
	pmu_write_PMCR(val | 1);

	pmu_read_PMCNTENSET(&val);
	pmu_write_PMCNTENSET(val | 1UL << 31);

	pmu_write_PMCCNTR(0);
	pmu_read_PMCCNTR(&val);

	printk("pmu ccntr val = %d\n", val);
}

__init_module__(pmu_init);
