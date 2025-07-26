#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// void print_visually(const char *str) {
//     for (int i = 0; str[i] != '\0'; i++) {
//         unsigned char ch = str[i];
//         if (ch < 32 || ch == 127) {
//             printf("^%c", ch ^ 64);
//         } else {
//             putchar(ch);
//         }
//     }
// }

int echo(char *input) {
    char *flag = strtok(input, " ");  // store flag or first word

    int no_newline = 0;
    int visuals = 0;

    if (flag[0] == '-') {
        if (flag[1] == 'n') {
            no_newline = 1;
        } else if (flag[1] == 'v') {
            visuals = 1;
        }
    }

    char *rest = strtok(NULL, "\0");

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
