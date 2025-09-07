#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void ejecutar_comando(char **arr) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error en fork()");
        exit(1);
    } else if (pid == 0) {
        execvp(arr[0], arr);   // ejecuta el comando ingresado
        _exit(127);
    } else {
        wait(NULL); // Espera a que el hijo termine
    }
}