#include <stdio.h>
#include "struct.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "prompt.h"

//manejo de errores

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

ArgArray guardaArgumentos(char *input) {
    ArgArray arg_array;
    initializeArgArray(&arg_array);

    char *token = strtok(input, " ");

    while (token != NULL) {
        // Inicializar o expandir el arreglo si es necesario
        if (arg_array.count + 1 >= arg_array.capacity) {
            arg_array.capacity = (arg_array.capacity == 0) ? 10 : arg_array.capacity * 2;
            char **temp = (char**)realloc(arg_array.cadenas, arg_array.capacity * sizeof(char*));
            if (temp == NULL) {
                perror("Error de reasignacion de memoria");
                free(arg_array.cadenas);
                arg_array.cadenas = NULL;
                exit(EXIT_FAILURE);
            }
            arg_array.cadenas = temp;
        }
        
        arg_array.cadenas[arg_array.count++] = strdup(token);
        token = strtok(NULL, " ");
    }
    // Recorta el arreglo al tamano exacto
    // No salgas del programa si esto falla, es mejor mantener la memoria extra
    char **temp_realloc = (char**)realloc(arg_array.cadenas, (arg_array.count + 1) * sizeof(char*));
    if (temp_realloc != NULL) {
        arg_array.cadenas = temp_realloc;
        arg_array.capacity = arg_array.count + 1;
    }
    
    // Asegura que el ultimo puntero sea NULL
    if (arg_array.cadenas != NULL) {
        arg_array.cadenas[arg_array.count] = NULL;
    }
    return arg_array;
}

ArgArray leer_y_split() {
    printf("Ingresar Comando: ");
    fflush(stdout);
    
    char* input = lectura_infinita();

    if (input == NULL || strlen(input) == 0) {
        if (input != NULL) {
            free(input);
        }
        // Devuelve una estructura vacia en caso de error
        ArgArray empty = {NULL, 0, 0};
        return empty;
    }
    
    // Llama a la funcion guardaArgumentos y pasa la entrada
    ArgArray result = guardaArgumentos(input);
    
    // Libera la cadena de entrada una vez que se tokenizo
    //free(input);

    return result;
}


