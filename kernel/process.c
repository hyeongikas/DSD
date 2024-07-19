#include "process.h"
#include <string.h>

process_t processes[MAX_PROCESSES];
uint32_t current_pid = 0;

void process_init() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].state = 2; // Initialize all processes as waiting
    }
}

void process_create(void (*entry_point)()) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].state == 2) { // Find an available process slot
            processes[i].stack = malloc(PROCESS_STACK_SIZE);
            processes[i].rip = (uint64_t)entry_point;
            processes[i].rsp = (uint64_t)processes[i].stack + PROCESS_STACK_SIZE;
            processes[i].pid = current_pid++;
            processes[i].state = 0; // Mark as ready
            return;
        }
    }
    // Handle error: no available process slots
}

void process_switch() {
    // Implement context switching logic here
    // This will involve saving the current process's state and restoring the new process's state
}

void process_scheduler() {
    // Implement scheduling logic here
    // This will involve selecting the next process to run and calling process_switch()
}