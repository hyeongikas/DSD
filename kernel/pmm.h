#ifndef PMM_H
#define PMM_H

#include <stdint.h>

void pmm_init();
void* pmm_alloc();
void pmm_free(void* ptr);

#endif
