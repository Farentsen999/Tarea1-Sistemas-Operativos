//Recordar listar casos limite, incluir << y >> y documentar codigo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void ejecutar_comando(char **args, char *inputFile, char *outputFile) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Error en fork()");
        exit(1);
    } else if (pid == 0) {
        //if(inputFile != NULL) {
            //int descriptorArchivoEntrada = open(inputFile, O_RDONLY);
            //if (descriptorArchivoEntrada < 0) {
                //perror("Error al abrir archivo de entrada");
                //_exit(1);
            //}
            //dup2(descriptorArchivoEntrada, STDIN_FILENO);
            //close(descriptorArchivoEntrada);
        //}

        //if(outputFile != NULL) {
           // int descriptorArchivoSalida = open(outputFile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
           // if (escriptorArchivoSalida < 0) {
               // perror("Error al abrir/crear archivo de salida");
               // _exit(1);
            //}
            //dup2(descriptorArchivoSalida, STDOUT_FILENO);
           // close(descriptorArchivoSalida);
        //}
        execvp(args[0], args); 
        perror("Error, comando no encontrado");
        _exit(1);
    }
    else {
        wait(NULL);
    }
}
