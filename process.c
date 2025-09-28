#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#include "struct.h"
#include "redirections.h"

// Función para ejecutar un comando sin pipes
void ejecutar_comando(ArgArray *cmd) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Error en fork()");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Proceso hijo
        
        // Redirecciones si es necesario
        setup_redirections(cmd->inputFile, cmd->outputFile);

        // Ejecuta el comando
        execvp(cmd->cadenas[0], cmd->cadenas);
        perror("Error, comando no encontrado");
        _exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }
}