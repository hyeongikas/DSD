#include "fs.h"
#include "pmm.h"
#include "paging.h"
#include "screen.h"

static fs_info_t* fs_info = NULL;

void fs_init() {
    // Allocate memory for file system info
    fs_info = (fs_info_t*)pmm_alloc();
    
    // Initialize FAT and Root Directory
    uint8_t* fat = (uint8_t*)pmm_alloc();
    uint8_t* root_dir = (uint8_t*)pmm_alloc();
    
    // Zero out FAT and Root Directory
    for (int i = 0; i < FS_SECTOR_SIZE; i++) {
        fat[i] = 0;
        root_dir[i] = 0;
    }
    
    // Initialize File System Info
    fs_info->fat_start = FS_ROOT_DIR_SECTORS;
    fs_info->data_start = fs_info->fat_start + FS_FAT_SECTORS;
    
    screen_print("File System initialized.\n");
}

uint32_t fs_open(const char* filename) {
    // Simple implementation: return a dummy index
    return 0;
}

void* fs_read(uint32_t file_index, uint32_t* size) {
    // Simple implementation: return NULL
    *size = 0;
    return NULL;
}
