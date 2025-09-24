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
    /*/
    char *inputFile;
    char *outputFile;
    /*/

    // Controla el bucle principal
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
            commandParser(arr, args);
            ejecutar_comando(args);   
        }
    }
    return 0;
}
