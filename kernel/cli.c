#include "cli.h"
#include "fs.h"
#include "screen.h"
#include <string.h>

#define MAX_CMD_LEN 128

static void execute_command(const char* command) {
    // Simple command processing
    if (strcmp(command, "ls") == 0) {
        screen_print("Listing files:\n");
        // Simple implementation: list files in root directory
        for (int i = 0; i < FS_MAX_FILES; i++) {
            if (fs_info->files[i].size > 0) {
                char buf[32];
                snprintf(buf, sizeof(buf), "File %d\n", i);
                screen_print(buf);
            }
        }
    } else if (strncmp(command, "cat ", 4) == 0) {
        uint32_t file_index = fs_open(command + 4);
        uint32_t size;
        void* data = fs_read(file_index, &size);
        if (data) {
            screen_print((char*)data);
        } else {
            screen_print("Error reading file.\n");
        }
    } else {
        screen_print("Unknown command.\n");
    }
}

void cli_main() {
    char command[MAX_CMD_LEN];
    int cmd_pos = 0;

    screen_print("DSD CLI Interface\n");

    while (1) {
        screen_print("> ");
        while (cmd_pos < MAX_CMD_LEN - 1) {
            char c = getchar(); // Implement getchar() to read a character from keyboard
            if (c == '\n') {
                command[cmd_pos] = '\0';
                execute_command(command);
                cmd_pos = 0;
                break;
            }
            command[cmd_pos++] = c;
        }
    }
}
