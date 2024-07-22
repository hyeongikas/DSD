#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD_LENGTH 1024
#define MAX_ARGS 64
#define MAX_REDIRECTS 2
#define MAX_PIPES 16

typedef enum {
    NONE,
    OUTPUT_REDIRECT,
    APPEND_REDIRECT,
    INPUT_REDIRECT,
    HERE_DOC_REDIRECT
} redirect_type_t;

typedef struct {
    char *cmd;
    char *args[MAX_ARGS];
    int argc;
    redirect_type_t redirects[MAX_REDIRECTS];
    char *redirect_files[MAX_REDIRECTS];
    int pipe_fds[MAX_PIPES][2];
    int num_pipes;
    int background;
} command_t;

void parse_error(char *msg) {
    fprintf(stderr, "parse error: %s\n", msg);
    exit(1);
}

command_t parse_command(char *input) {
    command_t cmd;
    cmd.argc = 0;
    cmd.num_pipes = 0;
    cmd.background = 0;

    // Split the input string into commands separated by semicolons
    char *token = strtok(input, ";");
    while (token != NULL) {
        // Parse the command and its arguments
        char *arg = strtok(token, " \t\n");
        cmd.cmd = arg;

        while ((arg = strtok(NULL, " \t\n")) != NULL) {
            if (cmd.argc < MAX_ARGS) {
                cmd.args[cmd.argc] = arg;
                cmd.argc++;
            } else {
                parse_error("too many arguments");
            }
        }

        // Parse redirection operators
        for (int i = 0; i < MAX_REDIRECTS; i++) {
            if (strcmp(cmd.args[cmd.argc - 1], ">") == 0) {
                cmd.redirects[i] = OUTPUT_REDIRECT;
                cmd.redirect_files[i] = cmd.args[cmd.argc - 2];
                cmd.argc -= 2;
            } else if (strcmp(cmd.args[cmd.argc - 1], ">>") == 0) {
                cmd.redirects[i] = APPEND_REDIRECT;
                cmd.redirect_files[i] = cmd.args[cmd.argc - 2];
                cmd.argc -= 2;
            } else if (strcmp(cmd.args[cmd.argc - 1], "<") == 0) {
                cmd.redirects[i] = INPUT_REDIRECT;
                cmd.redirect_files[i] = cmd.args[cmd.argc - 2];
                cmd.argc -= 2;
            } else if (strcmp(cmd.args[cmd.argc - 1], "<<") == 0) {
                cmd.redirects[i] = HERE_DOC_REDIRECT;
                cmd.redirect_files[i] = cmd.args[cmd.argc - 2];
                cmd.argc -= 2;
            }
        }

        // Parse pipes
        for (int i = 0; i < MAX_PIPES; i++) {
            if (strcmp(cmd.args[cmd.argc - 1], "|") == 0) {
                pipe(cmd.pipe_fds[i]);
                cmd.num_pipes++;
                cmd.argc--;
            }
        }

        // Parse background job operator
        if (strcmp(cmd.args[cmd.argc - 1], "&") == 0) {
            cmd.background = 1;
            cmd.argc--;
        }

        token = strtok(NULL, ";");
    }

    return cmd;
}

void execute_command(command_t cmd) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process

        // Handle redirections
        for (int i = 0; i < MAX_REDIRECTS; i++) {
            if (cmd.redirects[i] == OUTPUT_REDIRECT) {
                int fd = open(cmd.redirect_files[i], O_WRONLY | O_CREAT, 0644);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            } else if (cmd.redirects[i] == APPEND_REDIRECT) {
                int fd = open(cmd.redirect_files[i], O_WRONLY | O_APPEND | O_CREAT, 0644);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            } else if (cmd.redirects[i] == INPUT_REDIRECT) {
                int fd = open(cmd.redirect_files[i], O_RDONLY);
                dup2(fd, STDIN_FILENO);
                close(fd);
            } else if (cmd.redirects[i] == HERE_DOC_REDIRECT) {
                int fd = open(cmd.redirect_files[i], O_RDWR | O_CREAT | O_EXCL, 0600);
                dup2(fd, STDIN_FILENO);
                close(fd);
                char *line = NULL;
                size_t n = 0;
                while (getline(&line, &n, stdin) != -1) {
                    write(STDOUT_FILENO, line, strlen(line));
                }
                free(line);
                exit(0);
            }
        }

        // Handle pipes
        if (cmd.num_pipes > 0) {
            for (int i = 0; i < cmd.num_pipes; i++) {
                dup2(cmd.pipe_fds[i][0], STDIN_FILENO);
                close(cmd.pipe_fds[i][0]);
                dup2(cmd.pipe_fds[i][1], STDOUT_FILENO);
                close(cmd.pipe_fds[i][1]);
            }
        }

        // Execute the command
        execvp(cmd.cmd, cmd.args);
        perror("execvp");
        exit(1);
    } else if (pid > 0) {
        // Parent process

        // Wait for background jobs to finish
        if (cmd.background == 0) {
            int status;
            waitpid(pid, &status, 0);
        }
    } else {
        // Fork failed
        perror("fork");
        exit(1);
    }
}

void builtin_cd(char *path) {
    if (chdir(path) != 0) {
        perror("chdir");
    }
}

void builtin_pwd() {
    char cwd[MAX_CMD_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd");
    }
}

void builtin_exit() {
    exit(0);
}

void parse_and_execute(char *input) {
    command_t cmd = parse_command(input);

    // Handle built-in commands
    if (strcmp(cmd.cmd, "cd") == 0) {
        builtin_cd(cmd.args[1]);
    } else if (strcmp(cmd.cmd, "pwd") == 0) {
        builtin_pwd();
    } else if (strcmp(cmd.cmd, "exit") == 0) {
        builtin_exit();
    } else {
        // Execute external commands
        execute_command(cmd);
    }
}

int main() {
    char input[MAX_CMD_LENGTH];
    printf("shell> ");

    while (fgets(input, MAX_CMD_LENGTH, stdin) != NULL) {
        input[strcspn(input, "\n")] = 0; // Remove the newline character

        parse_and_execute(input);

        printf("shell> ");
    }

    return 0;
}