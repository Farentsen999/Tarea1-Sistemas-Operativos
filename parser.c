#include <string.h>
#include "parser.h"

void commandParser(char *inputTokens[], char *args[], char **inputFile, char **outputFile) {
    int argCount = 0;
    int i = 0;

    *inputFile = NULL;
    *outputFile = NULL;

    while (inputTokens[i] != NULL) {
        if (strcmp(inputTokens[i], "<") == 0) {
            i++;
            if (inputTokens[i] != NULL) {
                *inputFile = inputTokens[i];
            }
        } else if (strcmp(inputTokens[i], ">") == 0) {
            i++;
            if (inputTokens[i] != NULL) {
                *outputFile = inputTokens[i];
            }
        } else if (strcmp(inputTokens[i], "|") == 0) {
            break;
        } else {
            args[argCount++] = inputTokens[i];
        }
        i++;
        if (argCount >= MAX_ARGS - 1) {
            break;
        }
    }
    args[argCount] = NULL;
}