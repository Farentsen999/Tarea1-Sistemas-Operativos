#include <string.h>
#include "parser.h"

void commandParser(char *inputTokens[], char *args[]) {
    int argCount = 0;
    int i = 0;

    while (inputTokens[i] != NULL) {
        args[argCount++] = inputTokens[i++];
        if (argCount >= MAX_ARGS - 1) {
            break;
        }
    }
    args[argCount] = NULL;
}