#include "mylib.h"

#include <dirent.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

struct linux_dirent64 {
    uint64_t d_ino;
    int64_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[];
};

// Buffer size for reading directory entries
#define BUF_SIZE 1024
// print current location
void pwd() {
    char buf[1024];
    syscall(SYS_getcwd, buf, sizeof(buf));
    syscall(SYS_write, 1, buf, strlen(buf));
    syscall(SYS_write, 1, "\n", 1);
}

// for echo -v flag
void print_visually(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        unsigned char ch = str[i];
        if (ch < 32 || ch == 127) {
            printf("^%c", ch ^ 64);  // control character
        } else {
            putchar(ch);
        }
    }
}

// echo in here
int echo(char *input) {
    int no_newline = 0;
    int visual = 0;

    char *token = strtok(input, " ");
    while (token && token[0] == '-') {
        if (strcmp(token, "-n") == 0)
            no_newline = 1;
        else if (strcmp(token, "-v") == 0)
            visual = 1;
        else
            break;

        token = strtok(NULL, " ");
    }

    char output[1024] = "";
    if (token) {
        strcat(output, token);
        char *next = strtok(NULL, "");
        if (next) {
            strcat(output, " ");
            strcat(output, next);
        }
    }

    if (visual)
        print_visually(output);
    else
        printf("%s", output);

    if (!no_newline) printf("\n");

    return 0;
}

//ls to list all the files or dir inside of current 
int ls() {
    int fd = syscall(SYS_open, ".", O_RDONLY | O_DIRECTORY);
    if (fd < 0) return 1;

    char buf[BUF_SIZE];
    int nread;

    while ((nread = syscall(SYS_getdents64, fd, buf, BUF_SIZE)) > 0) {
        int bpos = 0;
        while (bpos < nread) {
            struct linux_dirent64 *d = (struct linux_dirent64 *)(buf + bpos);

            // Avoid writing hidden files (optional)
            if (d->d_name[0] != '.') {
                syscall(SYS_write, 1, d->d_name, strlen(d->d_name));
                syscall(SYS_write, 1, "\n", 1);
            }

            bpos += d->d_reclen;
        }
    }

    syscall(SYS_close, fd);
    return 0;
}