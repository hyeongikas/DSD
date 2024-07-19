#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

void heap_init();
void* kmalloc(uint64_t size);
void kfree(void* ptr);

#endif
