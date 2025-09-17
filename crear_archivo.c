#include <stdio.h>
#include <string.h>

void crearArchivo(char *inputTokens, int punto_partida) {
    // Reservar espacio para el nombre del archivo (con '\0')
    int lenNombre = strlen(inputTokens) - punto_partida;
    char nombre_archivo[lenNombre + 1];

    // Copiar desde inputTokens[punto_partida] hasta el final
    strcpy(nombre_archivo, inputTokens + punto_partida);

    // Ahora preparar el contenido que quieres escribir
    int largoEscritura = punto_partida;  // hasta antes del nombre
    char escritura_archivo[largoEscritura + 1];
    strncpy(escritura_archivo, inputTokens, largoEscritura);
    escritura_archivo[largoEscritura] = '\0';  // asegurar fin de string

    // Abrir archivo
    FILE *archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("ERROR AL ABRIR EL ARCHIVO\n");
        return;
    }

    // Escribir contenido
    fprintf(archivo, "%s", escritura_archivo);

    fclose(archivo);
}
