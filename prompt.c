#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "prompt.h"

char* lectura_infinita() {
    //se usa size_t en vez de int para aprovechar al maximo la capacidad de almacenamiento del computador
    size_t capacidad_inicial = 10*sizeof(char);
    size_t longitud_actual = 0;
    char *buffer = malloc(capacidad_inicial);
    if (buffer == NULL) {
        perror("Error, no se pudo asignar memoria");
        return NULL;
    }

    int c;
    //fgetc() lee caracter por caracter
    while ((c = fgetc(stdin)) != '\n'&& c != EOF) {
        //aqui se aumenta la capacidad del del buffer segun sea necesario
        if (longitud_actual + 1 >= capacidad_inicial) {
            capacidad_inicial *= 2;
            char *temp = realloc(buffer, capacidad_inicial);
            if (temp== NULL) {
                perror("Error, no se pudo asignar memoria");
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
        buffer[longitud_actual++] = c;
    }

    //se termina el almacenamiento en el buffer
    if (longitud_actual<capacidad_inicial) {
        char *temp = realloc(buffer, longitud_actual + 1);
        if (temp != NULL) {
            buffer = temp;
        }
    }
    buffer[longitud_actual] = '\0';
    return buffer;
}

char* leer_y_split(char *arr[]) {
    printf("Ingresar Comando: ");
    fflush(stdout); //asegura que el prompt se imprima antes de leer la entrada
    
    char* input = lectura_infinita();
    size_t count = 0;

    if (input == NULL) {
        return -1; // Error en la lectura
    }

    if (strlen(input) == 0) {
        arr[0] = NULL;
        free(input);
        return 0;
    }

    char *token = strtok(input, " "); //divide la cadena en tokens separados por espacios
    while (token != NULL && count < MAX_STRINGS - 1) {
        arr[count] = token; //almacena cada token en el arreglo
        token = strtok(NULL, " "); //obtiene el siguiente token
        count++;
    }

    arr[count] = NULL; // Termina el array con NULL para execvp
    
    return input; // Devuelve el puntero para poder hacer free() después
}
