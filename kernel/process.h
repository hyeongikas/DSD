#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MAX_PROCESSES 10
#define PROCESS_STACK_SIZE 4096

typedef struct {
    uint64_t* stack;
    uint64_t rip;
    uint64_t rsp;
    uint32_t pid;
    uint8_t state; // 0: ready, 1: running, 2: waiting
} process_t;

void process_init();
void process_create(void (*entry_point)());
void process_switch();
void process_scheduler();

#endif
