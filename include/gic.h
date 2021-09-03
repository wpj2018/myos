#ifndef __GIC_H
#define __GIC_H

#define PHYS_GICD_BASE		0x1e001000
#define PHYS_GICC_BASE		0x1e000100

#define GICD_CTLR		0x000
#define GICD_TYPER		0x004
#define GICD_IIDR		0x008
#define GICD_RES0		0x00C
#define GICD_IGROUPR		0x080
#define GICD_ISENABLER		0x100
#define GICD_ICENABLER		0x180
#define GICD_ISPENDR		0x200
#define GICD_ICPENDR		0x280
#define GICD_ISACTIVER		0x300
#define GICD_ICACTIVER		0x380
#define GICD_IPRIORITYR		0x400
#define GICD_RES1		0x7FC
#define GICD_ITARGETSR		0x800
#define GICD_RES2		0xBFC
#define GICD_ICFGR		0xC00
#define GICD_RES3		0xD00
#define GICD_NSACR		0xE00
#define GICD_SGIR		0xF00
#define GICD_RES4		0xF04
#define GICD_CPENDSGIR		0xF10
#define GICD_SPENDSGIR		0xF20
#define GICD_RES5		0xF30
#define GICD_END		0x1000

struct gic_dist {
	volatile size_t ctrl;
	volatile size_t type;
	volatile size_t id;
	volatile uchar res0[GICD_IGROUPR - GICD_RES0];
	volatile size_t group[(GICD_ISENABLER - GICD_IGROUPR) >> 2];
	volatile size_t set_enable[(GICD_ICENABLER - GICD_ISENABLER) >> 2];
	volatile size_t clear_enable[(GICD_ISPENDR - GICD_ICENABLER) >> 2];
	volatile size_t set_pend[(GICD_ICPENDR - GICD_ISPENDR) >> 2];
	volatile size_t clear_pend[(GICD_ISACTIVER - GICD_ICPENDR) >> 2];
	volatile size_t set_active[(GICD_ICACTIVER - GICD_ISACTIVER) >> 2];
	volatile size_t clear_active[(GICD_IPRIORITYR - GICD_ICACTIVER) >> 2];
	volatile uchar priority[GICD_RES1 - GICD_IPRIORITYR];
	volatile uchar res1[GICD_ITARGETSR - GICD_RES1];
	volatile uchar target[GICD_RES2 - GICD_ITARGETSR];
	volatile uchar res2[GICD_ICFGR - GICD_RES2];
	volatile size_t cfg[(GICD_RES3 - GICD_ICFGR) >> 2];
	volatile uchar res3[GICD_NSACR - GICD_RES3];
	volatile size_t ns_acess_ctrl[(GICD_SGIR - GICD_NSACR) >> 2];
	volatile size_t sgi[(GICD_RES4 - GICD_SGIR) >> 2];
	volatile uchar res4[GICD_CPENDSGIR - GICD_RES4];
	volatile uchar sgi_clear_pend[GICD_SPENDSGIR - GICD_CPENDSGIR];
	volatile uchar sgi_set_pend[GICD_RES5 - GICD_SPENDSGIR];
	volatile uchar res5[GICD_END - GICD_RES5];
};

struct gic_cpu {
	volatile size_t ctrl;
	volatile size_t pm;
	volatile size_t bp;
	volatile size_t ack;
	volatile size_t eoi;
	volatile size_t res[];
};

size_t gicc_get_ack(void);
void gicc_set_eoi(size_t ack_no);

#endif
