#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

#include "mylib.h"
#include "utils/utils.h"

int forkClone(char *cmd);

int main() {
    setbuf(stdout, NULL);
    while (1) {
        printf("$ ");
        char input[1024];
        if (!fgets(input, sizeof(input), stdin)) break;

        input[strcspn(input, "\n")] = '\0';

        bool hasPipe = checkPipe(input);

        // fork
        if (hasPipe) {
            forkClone(input);
            continue;
        }

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

        // cd
        if (cmd && strcmp(cmd, "cd") == 0) {
            char *dirOrFileName = "";

            char *rest = strtok(NULL, "");
            if (rest) {
                dirOrFileName = rest;
            }
            cd(dirOrFileName);
        }

        // cat
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

        // exit
        if (cmd && strcmp(cmd, "exit") == 0) {
            char *code_str = strtok(NULL, "");
            int exit_code = 0;

            if (code_str) {
                exit_code = atoi(code_str);
            }

            exit(exit_code);
        }

        // grep
        if (cmd && strcmp(cmd, "grep") == 0) {
            char *word = strtok(NULL, " ");
            char *file_token = strtok(NULL, "");

            if (word == NULL || file_token == NULL) {
                printf("Usage: grep <word> <file1> [file2 ...]\n");
            } else {
                grep(word, file_token);
            }
        }

        // clear
        if (cmd && strcmp(cmd, "clear") == 0) {
            clear();
        }
    }

    return 0;
}

int forkClone(char *cmd) {
    char *leftCmd = strtok(cmd, "|");
    char *rightCmd = strtok(NULL, "");

    if (!leftCmd || !rightCmd) {
        printf("Invalid pipe command\n");
        return 1;
    }

    // Strip leading/trailing spaces
    while (*leftCmd == ' ') leftCmd++;
    while (*rightCmd == ' ') rightCmd++;

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe failed");
        return 1;
    }

    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid1 == 0) {
        // child → runs left command → write to pipe
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        char *args[10];
        int i = 0;
        char *token = strtok(leftCmd, " ");
        while (token && i < 9) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        execvp(args[0], args);
        perror("execvp leftCmd failed");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid2 == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        // Tokenize right command
        char *args[10];
        int i = 0;
        char *token = strtok(rightCmd, " ");
        while (token && i < 9) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        execvp(args[0], args);
        perror("execvp rightCmd failed");
        exit(1);
    }

    // Parent: Close both ends and wait
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);

    return 0;
}
