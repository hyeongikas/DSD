#include <stdint.h>
#include "isr.h"

// ISR handler function
void isr_handler(uint64_t isr_number, uint64_t error_code) {
    char *video_memory = (char *)0xB8000;
    video_memory[0] = 'I';
    video_memory[1] = 0x07; // White color
}
