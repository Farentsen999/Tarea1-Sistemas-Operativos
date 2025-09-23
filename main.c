#include <unistd.h>
#include <sys/wait.h>
#include "prompt.h"
#include "process.h"
#include "parser.h"
#include <string.h>
#include "config.h"


int main(void) {
    char *arr[MAX_STRINGS]; // areglo de strings
	char *args[MAX_ARGS];
    char *inputFile;
    char *outputFile;
// controla el bucle principal
    while (1) {
        leer_y_split(arr);
        if (arr[0] == NULL) {
            continue;
        }
        if (strcmp(arr[0], "exit") == 0) {
           break; // cambiar la condición para salir del ciclo
            continue;
        }

		commandParser(arr, args, &inputFile, &outputFile);

        // Si el comando no es "exit", se ejecuta
        ejecutar_comando(args, inputFile, outputFile);        
    }
}
