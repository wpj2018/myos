#ifndef __PMU_H
#define __PMU_H

#include "types.h"

void pmu_write_PMUSERENR(size_t val);
void pmu_read_PMUSERENR(size_t *val);

void pmu_write_PMCR(size_t val);
void pmu_read_PMCR(size_t *val);

void pmu_write_PMCNTENSET(size_t val);
void pmu_read_PMCNTENSET(size_t *val);

void pmu_write_PMCCNTR(size_t val);
void pmu_read_PMCCNTR(size_t *val);

void pmu_init(void);

#endif
