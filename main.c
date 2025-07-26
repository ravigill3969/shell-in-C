#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "mylib.h"

int main() {
    setbuf(stdout, NULL);
    while (1) {
        printf("$ ");
        char input[1024];
        if (!fgets(input, sizeof(input), stdin)) break;

        input[strcspn(input, "\n")] = '\0';  // trim newline

        // taking out cmd name
        char *cmd = strtok(input, " ");

        // echo
        if (cmd && strcmp(cmd, "echo") == 0) {
            char *rest = strtok(NULL, "");
            if (rest) echo(rest);
        }

        // pwd
        if (cmd && strcmp(cmd, "pwd") == 0) {
            pwd();
        }

        // ls
        if (cmd && strcmp(cmd, "ls") == 0) {
            bool showHiddenFiles = 0;

            char *rest = strtok(NULL, "");

            if (rest && strcmp(rest, "-a") == 0) {
                showHiddenFiles = 1;
            }

            ls(showHiddenFiles);
        }

        // whoami
        // if (cmd && strcmp(cmd, "whoami") == 0) {
        //     whoami();
        // }

        if (cmd && strcmp(cmd, "cd") == 0) {
            char *dirOrFileName = "";

            char *rest = strtok(NULL, "");
            if (rest) {
                dirOrFileName = rest;
            }
            cd(dirOrFileName);
        }

        if (cmd && strcmp(cmd, "cat") == 0) {
            char *fileName;

            char *rest = strtok(NULL, "");
            if (rest) {
                fileName = rest;
                cat_file(fileName);
            } else {
                printf("cat: missing filename\n");
            }
        }

        if (cmd && strcmp(cmd, "exit") == 0) {
            if (cmd && strcmp(cmd, "exit") == 0) {
                char *code_str = strtok(NULL, "");
                int exit_code = 0;

                if (code_str) {
                    exit_code = atoi(code_str);
                }

                exit(exit_code);
            }
        }
    }

    return 0;
}
