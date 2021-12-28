#ifndef __SLAB_H
#define __SLAB_H

#include "types.h"

#define SLAB_HEAD_SIZE		4

void *slab_alloc(size_t size);
void slab_free(void *kaddr, size_t size);
void slab_init(void);

#endif
