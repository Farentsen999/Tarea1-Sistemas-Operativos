// Lista de las bibliotecaas locales
#include "parser.h"
#include "process.h"
#include "exec_with_pipes.h"
#include "config.h"

// bibliotecas generales
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <sys/wait.h>

void commandParser(char *inputTokens[], char *args[]) {
    int arg_count = 0;
    int pipes = 0;
    char *command_line[MAX_STRINGS][MAX_ARGS];
    char **comandos[MAX_STRINGS]; 

    for (int i = 0; inputTokens[i] != NULL; i++) {
        if (strcmp(inputTokens[i], "|") == 0) {
            command_line[pipes][arg_count] = NULL;
            comandos[pipes] = command_line[pipes]; 
            pipes++;
            arg_count = 0;
            continue;
        } else{
            command_line[pipes][arg_count++] = inputTokens[i];
        }
    }
    
    command_line[pipes][arg_count] = NULL;
    comandos[pipes] = command_line[pipes]; 

    if (pipes != 0)
    {
        exec_with_pipes(comandos, pipes);
    } else {            // SIN PIPES
        for (int i = 0; inputTokens[i] != NULL; i++) {
            args[i] = inputTokens[i];
        }
        args[arg_count] = NULL;
    }

    
    /*/
    *inputFile = NULL;
    *outputFile = NULL;
    /*/

        // PARA IMPLEMENTAR LUEGO
        /*/
        if (strcmp(inputTokens[i], "<") == 0 && inputTokens[i + 1] != NULL) {
            *inputFile = inputTokens[++i];
        } 
        else if (strcmp(inputTokens[i], ">") == 0 && inputTokens[i + 1] != NULL) {
            *outputFile = inputTokens[++i];
        } 
        else {
            args[argCount++] = inputTokens[i];
            if (argCount >= MAX_ARGS - 1) break;
        }
        /*/

    /*/
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
    /*/
}