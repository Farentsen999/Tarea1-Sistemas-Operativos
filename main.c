#include <unistd.h>
#include <sys/wait.h>
#include "prompt.h"
#include "process.h"
#include "parser.h"
#include <string.h>
#include "config.h"
#include "exec_with_pipes.h"


int main(void) {
    char *arr[MAX_STRINGS]; // areglo de strings
	char *args[MAX_ARGS];
    char *input_buffer; // Para almacenar el puntero a la memoria asignada
    /*/
    char *inputFile;
    char *outputFile;
    /*/

    // Controla el bucle principal
    while (1) {
        input_buffer = leer_y_split(arr);
        if (arr[0] == NULL) {
            if (input_buffer != NULL) {
                free(input_buffer);
            }
            continue;
        }
        if (strcmp(arr[0], "exit") == 0) {
            if (input_buffer != NULL) {
                free(input_buffer);
            }
            break;
        }
        //else if (strcmp(arr[0], "miprof") == 0) {
                //ejecutar_miProof(args);
           // }
        else {
            commandParser(arr, args);
            ejecutar_comando(args);   
        }
        
        // Libera la memoria correctamente
        if (input_buffer != NULL) {
            free(input_buffer);
        }
    }
    return 0;
}
