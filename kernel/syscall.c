// syscall.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SYS_ADDUSER 1
#define SYS_REMOVEUSER 2
#define SYS_MODIFYUSER 3
#define SYS_AUTHENTICATE 4

void syscall_handler(int syscall_number, void *args) {
    switch (syscall_number) {
        case SYS_ADDUSER: {
            add_user_args_t *add_user_args = (add_user_args_t *)args;
            add_user(add_user_args->username, add_user_args->password);
            break;
        }
        case SYS_REMOVEUSER: {
            remove_user_args_t *remove_user_args = (remove_user_args_t *)args;
            remove_user(remove_user_args->username);
            break;
        }
        case SYS_MODIFYUSER: {
            modify_user_args_t *modify_user_args = (modify_user_args_t *)args;
            modify_user(modify_user_args->username, modify_user_args->new_password);
            break;
        }
        case SYS_AUTHENTICATE: {
            authenticate_user_args_t *authenticate_user_args = (authenticate_user_args_t *)args;
            int authenticated = authenticate_user(authenticate_user_args->username, authenticate_user_args->password);
            printf("Authentication result: %d\n", authenticated);
            break;
        }
        default:
            printf("Unknown syscall number: %d\n", syscall_number);
            break;
    }
}

typedef struct {
    char *username;
    char *password;
} add_user_args_t;

typedef struct {
    char *username;
} remove_user_args_t;

typedef struct {
    char *username;
    char *new_password;
} modify_user_args_t;

typedef struct {
    char *username;
    char *password;
} authenticate_user_args_t;

//ex

int main() {
    add_user_args_t add_user_args;
    add_user_args.username = "user1";
    add_user_args.password = "password1";
    syscall_handler(SYS_ADDUSER, &add_user_args);

    remove_user_args_t remove_user_args;
    remove_user_args.username = "user2";
    syscall_handler(SYS_REMOVEUSER, &remove_user_args);

    modify_user_args_t modify_user_args;
    modify_user_args.username = "user1";
    modify_user_args.new_password = "new_password1";
    syscall_handler(SYS_MODIFYUSER, &modify_user_args);

    authenticate_user_args_t authenticate_user_args;
    authenticate_user_args.username = "user1";
    authenticate_user_args.password = "new_password1";
    syscall_handler(SYS_AUTHENTICATE, &authenticate_user_args);

    return 0;
}