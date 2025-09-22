#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void crearArchivo(char *inputTokens, int punto_partida) {
    // Reservar espacio para el nombre del archivo (con '\0')
    int lenNombre = strlen(inputTokens) - punto_partida;
    char *nombre_archivo = (char *)malloc((lenNombre + 1) * sizeof(char));
    if (nombre_archivo == NULL) {   
        printf("ERROR DE MEMORIA\n");
        return;
    }

    // Copiar desde inputTokens[punto_partida] hasta el final
    strcpy(nombre_archivo, inputTokens + punto_partida);

    // Ahora preparar el contenido que quieres escribir
    int largoEscritura = punto_partida;  // hasta antes del nombre
    char *escritura_archivo = (char *)malloc((largoEscritura + 1) * sizeof(char));
    if (escritura_archivo == NULL) {       
        printf("ERROR DE MEMORIA\n");
        free(nombre_archivo);
        return;
    }
    strncpy(escritura_archivo, inputTokens, largoEscritura);
    escritura_archivo[largoEscritura] = '\0';  // asegurar fin de string

    // Abrir archivo
    FILE *archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("ERROR AL ABRIR EL ARCHIVO\n");
        free(nombre_archivo);
        free(escritura_archivo);
        return;
    }

    // Escribir contenido
    fprintf(archivo, "%s", escritura_archivo);

    fclose(archivo);
    free(nombre_archivo);
    free(escritura_archivo);
}
