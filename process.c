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
        if(arr[0] == NULL) {
            _exit(0); // Si no hay comando, el hijo termina
        }
        else {
            execvp(arr[0], arr);  //cuando separa los espacios hacen que sean nulos y por eso no se puede crear archivos
            perror("Error, comando no encontrado\0"); 
                // ejecuta el comando ingresado
            _exit(127);
        }
    } else {
        wait(NULL); // Espera a que el hijo termine
    }
}
