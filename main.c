#include <unistd.h>
#include <sys/wait.h>
#include "prompt.h"
#include "process.h"
#include "parser.h"
#include <string.h>
#include "config.h"
#include "exec_with_pipes.h"


int main(void) {
    char *arr[MAX_LENGTH];
    char *args[MAX_ARGS];
    char *inputFile;
    char *outputFile;
// controla el bucle principal
// habilitar para infinitos argumentos
    while (1) {
        leer_y_split(arr);
        if (arr[0] == NULL) {
            continue;
        }
        if (strcmp(arr[0], "exit") == 0) {
            break;
        }
        //else if (strcmp(arr[0], "miprof") == 0) {
                //ejecutar_miProof(args);
           // }
        else {
            commandParser(arr, args, &inputFile, &outputFile);
            ejecutar_comando(args, inputFile, outputFile);   
        }
    }
    return 0;
}
