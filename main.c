#include <unistd.h>
#include <sys/wait.h>
#include "prompt.h"
#include "process.h"
#include "parser.h"
#include <string.h>


int main(void) {
    char *arr[MAX_STRINGS]; // areglo de strings
	char *args[MAX_ARGS];
    char *inputFile;
    char *outputFile;
    int running = 1; // controla el bucle principal
    while (running) {
        leer_y_split(arr);

        if (arr[0] == NULL) {
            continue;
        }

        if (strcmp(arr[0], "exit") == 0) {
            running = 0; // cambiar la condición para salir del ciclo
            continue;
        }

		commandParser(arr, args, &inputFile, &outputFile);

        // Si el comando no es "exit", se ejecuta
        ejecutar_comando(args);
    }
}
