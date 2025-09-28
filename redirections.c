#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "redirections.h"

// Función para configurar las redirecciones de entrada y salida
void setup_redirections(const char *input_file, const char *output_file) {
    if (input_file != NULL) {
        int fd_in = open(input_file, O_RDONLY);
        if (fd_in < 0) {
            perror("Error al abrir archivo de entrada");
            _exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }
    if (output_file != NULL) {
        int fd_out = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        if (fd_out < 0) {
            perror("Error al abrir/crear archivo de salida");
            _exit(EXIT_FAILURE);
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
}