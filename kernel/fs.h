#ifndef FS_H
#define FS_H

#include <stdint.h>

#define FS_SECTOR_SIZE 512
#define FS_ROOT_DIR_SECTORS 1
#define FS_FAT_SECTORS 1
#define FS_MAX_FILES 128

typedef struct {
    uint32_t size;
    uint32_t first_cluster;
} file_entry_t;

typedef struct {
    file_entry_t files[FS_MAX_FILES];
    uint32_t fat_start;
    uint32_t data_start;
} fs_info_t;

void fs_init();
uint32_t fs_open(const char* filename);
void* fs_read(uint32_t file_index, uint32_t* size);

#endif
