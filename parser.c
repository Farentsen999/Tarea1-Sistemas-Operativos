#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "parser.h"
#include "struct.h"

// Esta funcion parsea los comandos para manejar pipes y redirecciones..
void commandParser(ArgArray *cmd) {
    // Variables para el conteo de argumentos y comandos del pipe
    int arg_count = 0;
    int current_pipe = 0;
    bool has_pipes = false;
    
    // El array 'cadenas' contiene la entrada cruda: ["ls", "-l", ">", "salida.txt", NULL]

    // Paso 1: Conteo inicial de pipes para asignación de memoria
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
    } else {
        // Inicializa el array pipeCommands incluso si no ha pipes
        cmd->pipeCount = 1;
        cmd->pipeCommands = (char***)malloc(sizeof(char**));
        if (cmd->pipeCommands == NULL) {
            perror("Error de asignacion de memoria para comando simple");
            exit(EXIT_FAILURE);
        }
    }

    // Array temporal para almacenar los argumentos del comando actual (solo argumentos válidos)
    char **current_args = (char**)malloc(MAX_ARGS * sizeof(char*));
    if (current_args == NULL) {
        perror("Error al asignar memoria para current_args");
        exit(EXIT_FAILURE);
    }
    
    // Recorre los comandos para identificar y consumir redirecciones y pipes
    for (size_t i = 0; cmd->cadenas[i] != NULL; i++) {
        
        // Manejo de redirecciones de entrada
        if (strcmp(cmd->cadenas[i], "<") == 0) {
            if (cmd->cadenas[i + 1] != NULL) {
                // Guarda el nombre del archivo en la estructura
                cmd->inputFile = cmd->cadenas[++i]; 
            } else {
                fprintf(stderr, "Error de sintaxis: se esperaba archivo de entrada después de '<'.\n");
                exit(EXIT_FAILURE);
            }
        
        // Manejo de redirecciones de salida
        } else if (strcmp(cmd->cadenas[i], ">") == 0) {
            if (cmd->cadenas[i + 1] != NULL) {
                // Guarda el nombre del archivo en la estructura
                cmd->outputFile = cmd->cadenas[++i];
            } else {
                fprintf(stderr, "Error de sintaxis: se esperaba archivo de salida después de '>'.\n");
                exit(EXIT_FAILURE);
            }
        
        // Manejo de pipes
        } else if (strcmp(cmd->cadenas[i], "|") == 0) {
            
            // Finaliza el comando actual
            current_args[arg_count] = NULL;
            // Almacena el array de argumentos del comando completado
            cmd->pipeCommands[current_pipe++] = current_args;
            
            // Prepara un nuevo array para el siguiente comando
            current_args = (char**)malloc(MAX_ARGS * sizeof(char*));
            if (current_args == NULL) {
                perror("Error al reasignar memoria para current_args");
                exit(EXIT_FAILURE);
            }
            arg_count = 0;

        } else {
            // Almacena el argumento actual, si no es un operador de redireccion o nombre de archivo)
            current_args[arg_count++] = cmd->cadenas[i];
            
            if (arg_count >= MAX_ARGS) {
                fprintf(stderr, "Error: Se excedió el número máximo de argumentos (%d) en un comando.\n", MAX_ARGS);
                exit(EXIT_FAILURE);
            }
        }
    }
    
    // Almacena el ultimo comando
    current_args[arg_count] = NULL;
    
    if (current_pipe < cmd->pipeCount) {
        // Almacena el ultimo comando
        cmd->pipeCommands[current_pipe] = current_args;
    } else {
        // Esto solo ocurre si la entrada estaba vacía o mal formada al final
        free(current_args); // Liberamos el array no usado si el comando anterior terminó con un pipe
    }
}
