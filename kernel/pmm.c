#include "pmm.h"
#include "screen.h"
#include <cstddef>

#define PMM_PAGE_SIZE 4096
#define PMM_MAX_PAGES 32768

static uint8_t pmm_bitmap[PMM_MAX_PAGES / 8] = {0};

static inline void pmm_set_bit(uint64_t frame) {
    pmm_bitmap[frame / 8] |= (1 << (frame % 8));
}

static inline void pmm_clear_bit(uint64_t frame) {
    pmm_bitmap[frame / 8] &= ~(1 << (frame % 8));
}

static inline uint8_t pmm_test_bit(uint64_t frame) {
    return pmm_bitmap[frame / 8] & (1 << (frame % 8));
}

void* pmm_alloc() {
    for (uint64_t i = 0; i < PMM_MAX_PAGES; i++) {
        if (!pmm_test_bit(i)) {
            pmm_set_bit(i);
            return (void*)(i * PMM_PAGE_SIZE);
        }
    }
    return NULL; // No free frames
}

void pmm_free(void* ptr) {
    uint64_t frame = (uint64_t)ptr / PMM_PAGE_SIZE;
    pmm_clear_bit(frame);
}

void pmm_init() {
    // Here we could initialize the bitmap based on the memory map
    // For simplicity, we assume all memory is free
    screen_print("Physical Memory Manager initialized.\n");
}
