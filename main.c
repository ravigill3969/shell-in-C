#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);
    while (1) {
        // Uncomment this block to pass the first stage
        printf("$ ");

        // Wait for user input
        char input[100];
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0';

        if (strcmp(input, "exit 0") != 0) {
            char *firstPart = strtok(input, " ");
            char *secondPart = strtok(NULL, "");

            // echo ----------------------------------------------

            if (strcmp(firstPart, "echo") == 0) {
                printf("%s\n", secondPart);
                // type ------------------------------------------

            } else if (strcmp(firstPart, "type") == 0) {
                char cmd[256];
                snprintf(cmd, sizeof(cmd), "command -v %s > /dev/null 2>&1",
                         secondPart);
                if (system(cmd) != 0) {
                    printf("%s: not found\n", secondPart);
                } else {
                    printf("%s is a shell builtin\n", secondPart);
                }

            } else {
                // error------------------------------------------
                printf("%s: command not found \n", input);
            }
        } else {
            break;
        }
    }
    return 0;
}
