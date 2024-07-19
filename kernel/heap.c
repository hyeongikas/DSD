#include "heap.h"
#include "pmm.h"
#include "screen.h"

typedef struct heap_block {
    uint64_t size;
    struct heap_block* next;
} heap_block_t;

static heap_block_t* free_list = NULL;

void heap_init() {
    free_list = (heap_block_t*)pmm_alloc();
    free_list->size = PMM_PAGE_SIZE - sizeof(heap_block_t);
    free_list->next = NULL;
    screen_print("Heap Manager initialized.\n");
}

void* kmalloc(uint64_t size) {
    heap_block_t* current = free_list;
    heap_block_t* prev = NULL;

    while (current) {
        if (current->size >= size) {
            if (current->size > size + sizeof(heap_block_t)) {
                heap_block_t* new_block = (heap_block_t*)((uint64_t)current + sizeof(heap_block_t) + size);
                new_block->size = current->size - size - sizeof(heap_block_t);
                new_block->next = current->next;
                current->size = size;
                current->next = new_block;
            }
            if (prev) {
                prev->next = current->next;
            } else {
                free_list = current->next;
            }
            return (void*)((uint64_t)current + sizeof(heap_block_t));
        }
        prev = current;
        current = current->next;
    }

    return NULL; // No suitable block found
}

void kfree(void* ptr) {
    heap_block_t* block = (heap_block_t*)((uint64_t)ptr - sizeof(heap_block_t));
    block->next = free_list;
    free_list = block;
}
