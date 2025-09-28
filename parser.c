#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "parser.h"
#include "struct.h"

// Esta funcion parsea los comandos para manejar pipes y redirecciones
void commandParser(ArgArray *cmd) {
    // Variables para el conteo de argumentos y comandos del pipe
    int arg_count = 0;
    int current_pipe = 0;
    bool has_pipes = false;

    // Inicializa los campos de redireccionamiento y pipes
    for (size_t i = 0; cmd->cadenas[i] != NULL; i++) {
        if (strcmp(cmd->cadenas[i], "|") == 0) {
            cmd->pipeCount++;
            has_pipes = true;
        }
    }

    // Si hay pipes, se asigna memoria para los comandos del pipe
    if (has_pipes) {
        cmd->pipeCount++; // +1 para el último comando
        cmd->pipeCommands = (char***)malloc(cmd->pipeCount * sizeof(char**));
        if (cmd->pipeCommands == NULL) {
            perror("Error de asignacion de memoria para pipes");
            exit(EXIT_FAILURE);
        }
    }

    // Array temporal para almacenar los argumentos del comando actual
    char **current_args = (char**)malloc(MAX_ARGS * sizeof(char*));
    if (current_args == NULL) {
        perror("Error al asignar memoria para current_args");
        exit(EXIT_FAILURE);
    }

    // Recorre los comandos para identificar redirecciones y pipes
    for (size_t i = 0; cmd->cadenas[i] != NULL; i++) {
        //  Manejo de redirecciones de entrada
        if (strcmp(cmd->cadenas[i], "<") == 0) {
            if (cmd->cadenas[i + 1] != NULL) {
                cmd->inputFile = cmd->cadenas[++i];
            } else {
                perror("Error de sintaxis: se esperaba archivo de entrada");
                exit(EXIT_FAILURE);
            }
        // Manejo de redirecciones de salida
        } else if (strcmp(cmd->cadenas[i], ">") == 0) {
            if (cmd->cadenas[i + 1] != NULL) {
                cmd->outputFile = cmd->cadenas[++i];
            } else {
                perror("Error de sintaxis: se esperaba archivo de salida");
                exit(EXIT_FAILURE);
            }
            // Manejo de pipes
        } else if (strcmp(cmd->cadenas[i], "|") == 0) {
            if (has_pipes) {
                current_args[arg_count] = NULL;
                cmd->pipeCommands[current_pipe++] = current_args;
                
                current_args = (char**)malloc(MAX_ARGS * sizeof(char*));
                if (current_args == NULL) {
                    perror("Error al reasignar memoria para current_args");
                    exit(EXIT_FAILURE);
                }
                arg_count = 0;
            } else {
                break; // No debería haber pipes si no se detectó ninguno
            }
        } else {
            // Almacena el argumento actual
            current_args[arg_count++] = cmd->cadenas[i];
        }
    }
    // Finaliza el ultimo comando si no hay pipes
    current_args[arg_count] = NULL;
    
    // Almacena el ultimo comando si hay pipes
    if (has_pipes) {
        cmd->pipeCommands[current_pipe] = current_args;
    } else {
        free(current_args);
    }
}