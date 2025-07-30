#include "../utils/utils.h"

bool checkPipe(char *input) {
    bool hasPipe = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '|') {
            hasPipe = 1;
        }
    }

    return hasPipe;
}