#include <stdio.h>
#include <unistd.h>
#include "prompt.h"


int leer_y_split(char *arr[]) {
    static char input[MAX_LENGTH];
    int count = 0;
    char *p = input;

    printf("Ingrese un string: ");
    scanf(" %[^\n]", input);

    while (*p != '\0') {
        while (*p == ' ') p++;
        if (*p == '\0') break;

        arr[count++] = p;

        while (*p != ' ' && *p != '\0') p++;
        if (*p == ' ') {
            *p = '\0';
            p++;
        }
        if (count >= MAX_STRINGS - 1) break; 
    }
    arr[count] = NULL; // para el execvp
    return count;
}
