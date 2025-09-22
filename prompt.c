#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "prompt.h"


int leer_y_split(char *arr[] ) {
    char input[MAX_LENGTH];
    int count = 0;


    printf("Ingresar Comando: ");
    fflush(stdout); //asegura que el prompt se imprima antes de leer la entrada
    
    if(fgets(input, MAX_LENGTH, stdin) == NULL) {
        return -1;
    }
    input[strcspn(input, "\n")] = '\0'; // Elimina el salto de linea que añade fgets al final y lo cambia por \0

    if (strlen(input) == 0) {
        arr[0] = NULL;
        return 0;
    }

    char *token = strtok(input, " "); //divide la cadena en tokens separados por espacios
    while (token != NULL && count < MAX_STRINGS - 1) {
        arr[count] = token; //almacena cada token en el arreglo
        token = strtok(NULL, " "); //obtiene el siguiente token
        count++;
    }
    arr[count] = NULL; // Termina el array con NULL para execvp

    return count;
}
