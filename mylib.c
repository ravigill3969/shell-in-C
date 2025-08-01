#include "mylib.h"

#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
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
// print current location function
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

// echo function in here
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

// ls to list all the files or dir inside of current
int ls(bool showHiddenFiles) {
    int fd = syscall(SYS_open, ".", O_RDONLY | O_DIRECTORY);
    if (fd < 0) return 1;

    char buf[BUF_SIZE];
    int nread;

    while ((nread = syscall(SYS_getdents64, fd, buf, BUF_SIZE)) > 0) {
        int bpos = 0;
        while (bpos < nread) {
            struct linux_dirent64 *d = (struct linux_dirent64 *)(buf + bpos);

            char dot = '.';

            if (showHiddenFiles) {
                dot = ' ';
            }

            if (d->d_name[0] != dot) {
                syscall(SYS_write, 1, d->d_name, strlen(d->d_name));
                syscall(SYS_write, 1, "\n", 1);
            }

            bpos += d->d_reclen;
        }
    }

    syscall(SYS_close, fd);
    return 0;
}

// cd
int cd(const char *dirOrFileName) {
    // cd functionality here
    char buf[1024];

    if (syscall(SYS_getcwd, buf, sizeof(buf)) == -1) {
        return 1;
    }

    if (syscall(SYS_chdir, dirOrFileName) == -1) {
        return 1;
    }

    if (syscall(SYS_getcwd, buf, sizeof(buf)) == -1) {
        return 1;
    }

    return 0;
}

// cat-file
void cat_file(const char *filenames) {
    char buffer[1024];
    char input[1024];
    strncpy(input, filenames, sizeof(input) - 1);
    input[sizeof(input) - 1] = '\0';

    char *file = strtok(input, " ");
    while (file) {
        int fd = syscall(SYS_open, file, O_RDONLY);
        if (fd < 0) {
            perror(file);
            file = strtok(NULL, " ");
            continue;
        }

        ssize_t bytes;
        while ((bytes = syscall(SYS_read, fd, buffer, sizeof(buffer))) > 0) {
            syscall(SYS_write, STDOUT_FILENO, buffer, bytes);
        }

        syscall(SYS_close, fd);
        printf("\n\n");

        file = strtok(NULL, " ");
    }
}

int grep(const char *word, const char *filenames) {
    char files_copy[1024];
    strncpy(files_copy, filenames, sizeof(files_copy));
    files_copy[sizeof(files_copy) - 1] = '\0';

    char *file_name = strtok(files_copy, " ");

    while (file_name) {
        FILE *fptr = fopen(file_name, "r");
        if (!fptr) {
            perror(file_name);
            file_name = strtok(NULL, " ");
            continue;
        }

        char line[1024];
        while (fgets(line, sizeof(line), fptr)) {
            if (strstr(line, word)) {
                printf("%s: %s", file_name, line);
            }
        }

        fclose(fptr);
        file_name = strtok(NULL, " ");
    }

    return 0;
}


void clear() {
    // ANSI escape code to clear screen and move cursor to top-left
    printf("\033[2J\033[H");
    fflush(stdout);  // Ensure it flushes to terminal
}
