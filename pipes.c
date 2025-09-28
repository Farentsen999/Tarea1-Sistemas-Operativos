#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "redirections.h"

// Esta función cierra todos los descriptores de archivo de los pipes
void close_pipes(int fd[][2], int pipes){
    for (int i = 0; i < pipes; i++) {
        close(fd[i][0]);
        close(fd[i][1]);
    }
}

// Esta función espera a que todos los procesos hijos terminen
void wait_pids(pid_t pids[], int pipes){
    for (int i = 0; i < pipes + 1; i++) {
        waitpid(pids[i], NULL, 0);
    }
}

// Función para ejecutar comandos con pipes
void exec_with_pipes(ArgArray *cmd){
    // Se define el número de pipes y se crean los descriptores de archivo necesarios
    int pipes = cmd->pipeCount - 1;
    int fd[pipes][2];
    int procesos = pipes + 1;
    pid_t pids[procesos];

    // Se crean los pipes
    for (int i = 0; i < pipes; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Se crean los procesos hijos y se configuran los pipes y redirecciones
    for (int i = 0; i < procesos; i++) {
        pids[i] = fork();

        if (pids[i] == 0){
            // Proceso hijo

            // Redirige la entrada estándar al pipe anterior si no es el primer comando
            if (i > 0){
                dup2(fd[i-1][0], STDIN_FILENO);
            }
            // Redirige la salida estándar al siguiente pipe si no es el último comando
            if (i < pipes){
                dup2(fd[i][1], STDOUT_FILENO);
            }
            close_pipes(fd, pipes);

            // Redirección de entrada para el primer comando
            if (i == 0) {
                setup_redirections(cmd->inputFile, NULL);
            }
            // Redirección de salida para el último comando
            if (i == procesos - 1) {
                setup_redirections(NULL, cmd->outputFile);
            }

            // Ejecuta el comando
            execvp(cmd->pipeCommands[i][0], cmd->pipeCommands[i]);
            perror("Error en execvp");
            _exit(EXIT_FAILURE);
        }
    }

    // Proceso padre

    // Cierra todos los descriptores de archivo de los pipes y espera a que los procesos hijos terminen
    close_pipes(fd, pipes);
    wait_pids(pids, pipes);
}