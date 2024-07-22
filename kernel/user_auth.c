#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>

#define PASSWD_FILE "passwd"
#define MAX_LINE_LENGTH 1024

void add_user(char *username, char *password) {
    FILE *fp = fopen(PASSWD_FILE, "a");
    if (fp == NULL) {
        perror("fopen");
        return;
    }

    char hashed_password[128];
    char *salt = crypt_r(password, "$6$" "salt", hashed_password);

    fprintf(fp, "%s:%s\n", username, salt);
    fclose(fp);
}

void remove_user(char *username) {
    FILE *fp = fopen(PASSWD_FILE, "r");
    if (fp == NULL) {
        perror("fopen");
        return;
    }

    FILE *temp_fp = fopen("temp_passwd", "w");
    if (temp_fp == NULL) {
        perror("fopen");
        fclose(fp);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        char *colon = strchr(line, ':');
        if (colon == NULL) {
            fputs(line, temp_fp);
            continue;
        }

        *colon = '\0';
        char *saved_username = line;

        if (strcmp(saved_username, username) != 0) {
            fputs(line, temp_fp);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    if (remove(PASSWD_FILE) != 0) {
        perror("remove");
    }

    if (rename("temp_passwd", PASSWD_FILE) != 0) {
        perror("rename");
    }
}

void modify_user(char *username, char *new_password) {
    remove_user(username);
    add_user(username, new_password);
}

int authenticate_user(char *username, char *password) {
    FILE *fp = fopen(PASSWD_FILE, "r");
    if (fp == NULL) {
        perror("fopen");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        char *colon = strchr(line, ':');
        if (colon == NULL) {
            continue;
        }

        *colon = '\0';
        char *saved_username = line;
        char *saved_password = colon + 1;

        if (strcmp(saved_username, username) == 0) {
            char hashed_password[128];
            char *salt = crypt_r(password, saved_password, hashed_password);

            if (strcmp(hashed_password, saved_password) == 0) {
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}