#include <stdio.h>
#include <stdlib.h>
#include "crear_archivo.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void ejecutar_comando(char **args, char *inputFile, char *outputFile) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Error en fork()");
        exit(1);
    } else if (pid == 0) {
        if(inputFile != NULL) {
            int descriptorArchivoEntrada = open(inputFile, O_RDONLY);
            if (descriptorArchivoEntrada < 0) {
                perror("Error al abrir archivo de entrada");
                _exit(1);
            }
            dup2(descriptorArchivoEntrada, STDIN_FILENO);
            close(descriptorArchivoEntrada);
        }
<<<<<<< HEAD

        if(outputFile != NULL) {
            int descriptorArchivoSalida = open(outputFile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            if (escriptorArchivoSalida < 0) {
                perror("Error al abrir/crear archivo de salida");
                _exit(1);
            }
            dup2(descriptorArchivoSalida, STDOUT_FILENO);
            close(descriptorArchivoSalida);
=======
        else {
            execvp(arr[0], arr);  //cuando separa los espacios hacen que sean nulos y por eso no se puede crear archivos
            perror("Error, comando no encontrado\0"); 
                // ejecuta el comando ingresado
            _exit(127);
>>>>>>> 1927b45e973be8d20d2ae1c4d6bfcbed62ca45d2
        }
        execvp(args[0], args); 
        perror("Error, comando no encontrado");
        _exit(1);
    }
    else {
        wait(NULL);
    }
}
