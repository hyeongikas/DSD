#include "paging.h"
#include "pmm.h"
#include "screen.h"

#define PAGE_SIZE 4096

static uint64_t* pml4;

void paging_init() {
    // Allocate a page for PML4 (Page Map Level 4)
    pml4 = (uint64_t*)pmm_alloc();
    for (int i = 0; i < 512; i++) {
        pml4[i] = 0;
    }

    // Allocate a page for PDPT (Page Directory Pointer Table)
    uint64_t* pdpt = (uint64_t*)pmm_alloc();
    for (int i = 0; i < 512; i++) {
        pdpt[i] = 0;
    }
    pml4[0] = (uint64_t)pdpt | 0x03;

    // Allocate a page for PDT (Page Directory Table)
    uint64_t* pdt = (uint64_t*)pmm_alloc();
    for (int i = 0; i < 512; i++) {
        pdt[i] = 0;
    }
    pdpt[0] = (uint64_t)pdt | 0x03;

    // Allocate pages for PT (Page Table)
    for (int i = 0; i < 512; i++) {
        uint64_t* pt = (uint64_t*)pmm_alloc();
        for (int j = 0; j < 512; j++) {
            pt[j] = (i * 512 + j) * PAGE_SIZE | 0x03;
        }
        pdt[i] = (uint64_t)pt | 0x03;
    }

    // Load PML4 into CR3
    __asm__ __volatile__("mov %0, %%cr3" : : "r"(pml4));

    // Enable paging (set bit 31 in CR0)
    uint64_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("mov %0, %%cr0" : : "r"(cr0));

    screen_print("Paging initialized.\n");
}
