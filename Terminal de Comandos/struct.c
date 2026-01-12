#include <stdlib.h>
#include <stdlib.h>
#include <stddef.h>

#include "struct.h"

// Esta función inicializa la struct ArgArray
void initializeArgArray(ArgArray *argArray) {
    argArray->cadenas = NULL;
    argArray->count = 0;
    argArray->capacity = 0;

    argArray->pipeCommands = NULL;
    argArray->pipeCount = 0;

    argArray->inputFile = NULL;
    argArray->outputFile = NULL;
}

// Esta función libera la memoria usada por la struct ArgArray
void freeArgArray(ArgArray *argArray) {
    if (argArray->cadenas != NULL) {
    for (size_t i = 0; i < argArray->count; i++) {
        free(argArray->cadenas[i]);
    }
    free(argArray->cadenas);
    argArray->cadenas = NULL;
    }

    if (argArray->pipeCommands != NULL) {
        for (size_t i = 0; i < argArray->pipeCount; i++) {
            if (argArray->pipeCommands[i] != NULL) {
                free(argArray->pipeCommands[i]);
            }
        }
        free(argArray->pipeCommands);
        argArray->pipeCommands = NULL;
    }

    argArray->count = 0;
    argArray->capacity = 0;
    argArray->pipeCount = 0;
    argArray->inputFile = NULL;
    argArray->outputFile = NULL;
}