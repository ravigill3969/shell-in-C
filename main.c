#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    while (1) {
        printf("$ ");
        char input[100];
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0';

        if (strcmp(input, "exit 0") != 0) {
            char *firstPart = strtok(input, " ");
            char *secondPart = strtok(NULL, "");

            if (strcmp(firstPart, "echo") == 0) {
                printf("%s\n", secondPart);
            } else if (strcmp(firstPart, "type") == 0) {
                if (secondPart == NULL) {
                    printf("type: missing argument\n");
                } else {
                    char cmd[256];
                    snprintf(cmd, sizeof(cmd), "command -v %s > /dev/null 2>&1",
                             secondPart);
                    if (system(cmd) != 0) {
                        printf("%s: not found\n", secondPart);
                    } else {
                        if (strcmp(secondPart, "echo") == 0 ||
                            strcmp(secondPart, "cd") == 0) {
                            printf("%s is a shell builtin\n", secondPart);
                        } else {
                            snprintf(cmd, sizeof(cmd), "command -v %s", secondPart);
                            FILE *fp = popen(cmd, "r");
                            if (fp != NULL) {
                                char path[1024];
                                if (fgets(path, sizeof(path), fp) != NULL) {
                                    path[strcspn(path, "\n")] = '\0';
                                    printf("%s is %s\n", secondPart, path);
                                }
                                fclose(fp);
                            }
                        }
                    }
                }
            } else {
                printf("%s: command not found \n", input);
            }
        } else {
            break;
        }
    }
    return 0;
}
