#ifndef STRUCT_H
#define STRUCT_H
#include <stdlib.h>
#include <stddef.h>
typedef struct {
    char **cadenas;
    size_t count;
    size_t capacity;
    
    char ***pipeCommands;
    size_t pipeCount;

    char *inputFile;
    char *outputFile; 
} ArgArray;

void initializeArgArray(ArgArray *argArray);
void freeArgArray(ArgArray *argArray);
#endif