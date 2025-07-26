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

        char *cmd = strtok(input, " ");
        if (cmd && strcmp(cmd, "echo") == 0) {
            char *rest = strtok(NULL, "");
            if (rest) echo(rest);
        }

        if (cmd && strcmp(cmd, "pwd") == 0) {
            pwd();
        }
        if (cmd && strcmp(cmd, "ls") == 0) {
            ls();
        }
    }

    return 0;
}
