#include "screen.h"
#include "pmm.h"
#include "heap.h"
#include "paging.h"
#include "fs.h"

extern void load_gdt();
extern void load_idt();
extern void isr_handler(uint64_t isr_number, uint64_t error_code);
extern void keyboard_isr_handler();

void kernel_main() {
    // Load GDT
    load_gdt();

    // Load IDT
    load_idt();

    // Initialize screen
    screen_clear();
    screen_print("Welcome to MyOS!\n");

    // Initialize memory manager
    pmm_init();
    heap_init();

    // Initialize paging
    paging_init();

    // Initialize file system
    fs_init();

    // Set up ISR handlers
    for (int i = 0; i < 32; i++) {
        isr_stub_table[i] = (uint64_t)isr_handler;
    }

    // Set up keyboard ISR handler
    isr_stub_table[33] = (uint64_t)keyboard_isr_handler;

    // Enable interrupts
    __asm__("sti");
}
