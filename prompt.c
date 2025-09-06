#include <stdio.h>
#include <unistd.h>

#define MAX_STRINGS 20
#define MAX_LENGTH 100

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

int main(void) {
    pid_t pid;
    char *arr[MAX_STRINGS]; // areglo de strings

    while (1) {
        leer_y_split(arr);

        pid = fork();
        if (pid == 0) {
            execvp(arr[0], arr);   // ejecuta el comando ingresado 
            _exit(127);           
        } else {
            wait(NULL);
        }
    }
}
