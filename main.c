#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#include "miprof.h"
#include "prompt.h"
#include "process.h"
#include "parser.h"
#include "pipes.h"
#include "struct.h"

int main(void) {
    while (1) {
        // Se leen los comandos ingresados por el usuario y se almacenan en la struct ArgArray
        ArgArray cmd_data = leer_y_split();
        
        // Si no se ingreso ningun comando se continua con la siguiente iteracion
        if (cmd_data.cadenas == NULL || cmd_data.cadenas[0] == NULL) {
            continue;
        }

        // Comando para salir del shell
        if (strcmp(cmd_data.cadenas[0], "exit") == 0) {
            freeArgArray(&cmd_data);
           _exit(EXIT_SUCCESS);
        }

        // Comando para ejecutar miprof
        else if (strcmp(cmd_data.cadenas[0], "miprof") == 0) {
            ejecutar_miprof(cmd_data.cadenas);
        }

        // Procesa y ejecuta el comando
        else {
            // Parsea el comando para manejar pipes y redirecciones si hace falta
            commandParser(&cmd_data);

            // Ejecuta el comando, con o sin pipes
            if (cmd_data.pipeCount > 0) {
                exec_with_pipes(&cmd_data);
            } else {
                ejecutar_comando(&cmd_data);
            }
        }

        // Libera la memoria utilizada por ArgArray
        freeArgArray(&cmd_data);
    }
    return 0;
}