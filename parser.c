// Lista de las bibliotecaas locales
#include "parser.h"
#include "process.h"

// bibliotecas generales
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>   // para open
#include <unistd.h> 
#include <string.h>
#include <sys/wait.h>

void commandParser(char *inputTokens[], char *args[], char **inputFile, char **outputFile) {
    int argCount = 0;
    
    *inputFile = NULL;
    *outputFile = NULL;


    for (int i = 0; inputTokens[i] != NULL; i++) {
        if (strcmp(inputTokens[i], "<") == 0 && inputTokens[i + 1] != NULL) {
            *inputFile = inputTokens[++i];
        } 
        else if (strcmp(inputTokens[i], ">") == 0 && inputTokens[i + 1] != NULL) {
            *outputFile = inputTokens[++i];
        } 
        else if (strcmp(inputTokens[i], "|") == 0) {
            break; // pipes luego
        } 
        else {
            args[argCount++] = inputTokens[i];
            if (argCount >= MAX_ARGS - 1) break;
        }
    }
    args[argCount] = NULL; 
    if (*outputFile != NULL) {
        pid_t pid = fork();
        if (pid == 0) { // hijo
            int fd = open(*outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
            if (fd == -1) {
                perror("Error al crear el archivo"); 
                exit(1);
            }
            dup2(fd, STDOUT_FILENO); // redirigir stdout al archivo
            close(fd);
            ejecutar_comando(args, *inputFile, *outputFile);   // ejecutar comando, se puede llamar a la función?
            exit(1); // al momento de finalizar el proceso se hace "dup_free", y el resto se imprime en la terminal
        } else if (pid > 0) { // padre
            waitpid(pid, NULL, 0); 
        } else {
            perror("fork falló");
            exit(1);
        }  
    }
}