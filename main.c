#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_visually(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        unsigned char ch = str[i];
        if (ch < 32 || ch == 127) {
            printf("^%c", ch ^ 64);
        } else {
            putchar(ch);
        }
    }
}

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

    char output[100] = "";
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

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    while (1) {
        printf("$ ");
        char input[100];
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0';

        char *cmd = strtok(input, " ");
        if (cmd && strcmp(cmd, "echo") == 0) {
            char *rest = strtok(NULL, "");
            echo(rest);
        }
    }

    return 0;
}
