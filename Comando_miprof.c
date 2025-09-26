#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/resource.h>
#include "parser.h"

int timeout_ocurrio = 0; //Estoy solo funcionaría para un proceso, y no se podría hacer multiples miprof

void timeout_handler(int sig) { //miprof ejec, miprof ejecsave txt
    (void)sig;
    timeout_ocurrio = 1;
}

void ejecutar_miprof(char **arr) {
    pid_t pid;
    char *archivoMiprof = NULL;
    int timelimit = -1;
    int cmd_index = 0;

    if (arr[1] == NULL) {
        printf("Uso: miprof <modo> [args...]\n");
        return;
    }

    char *modo = arr[1];

    if (strcmp(modo, "ejec") == 0) {
        cmd_index = 2;
    } else if (strcmp(modo, "ejecsave") == 0) {
        if (arr[2] == NULL || arr[3] == NULL) {
            printf("Uso: miprof ejecsave <archivo> <comando> [args...]\n");
            return;
        }
        archivoMiprof = arr[2];
        cmd_index = 3;
    } else if (strcmp(modo, "maxtiempo") == 0) {
        if (arr[2] == NULL || arr[3] == NULL) {
            printf("Uso: miprof maxtiempo <segundos> <comando> [args...]\n");
            return;
        }
        timelimit = atoi(arr[2]);
        if (timelimit <= 0) {
            printf("Tiempo inválido.\n");
            return;
        }
        cmd_index = 3;
    } else {
        printf("Modo desconocido: %s\n", modo);
        return;
    }

    // construir argv para exec
    char **comando = &arr[cmd_index];

    char **comandos[MAX_STRINGS];
    // Aqui se usa la funcion de parser.c para detectar pipes
    int pipes = detectorCantidadPipes(comando, comandos)

    pid = fork();
    if (pid < 0) {
        perror("fork fallo");
        return;
    }

    if (pid == 0) {
        // Proceso Hijo

        if (archivoMiprof != NULL) {
            int fd = open(archivoMiprof, O_WRONLY | O_CREAT | O_APPEND, 0664);
            if (fd == -1) {
                perror("Error al abrir archivo");
                exit(1);
            }

            // redirigir stdout del hijo al archivo para la salida del comando
            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("dup2");
                close(fd);
                exit(1);
            }
            close(fd);
        }

        execvp(comando[0], comando);
        perror("execvp fallo");
        exit(1);
    } else {
        // Proceso Padre
        struct rusage usage;
        int status;

        if (timelimit > 0) {
            signal(SIGALRM, timeout_handler);
            alarm(timelimit);
        }

        while (1) {
            pid_t w = wait4(pid, &status, WNOHANG, &usage);
            if (w == 0) {
                if (timelimit > 0 && timeout_ocurrio) {
                    printf("Proceso finalizado por timeout\n");
                    kill(pid, SIGTERM);
                    wait4(pid, &status, 0, &usage);
                    timeout_ocurrio = 0;
                    break;
                }
                usleep(100000);
            } else if (w == pid) {
                timeout_ocurrio = 0;
                break;
            } else {
                perror("wait4");
                break;
            }
        }

        if (timelimit > 0) alarm(0);

        if (archivoMiprof != NULL) {
            int fdm = open(archivoMiprof, O_WRONLY | O_CREAT | O_APPEND, 0664);
            if (fdm != -1) {
                dprintf(fdm,
                    "Tiempo de usuario: %ld.%06ld s\n"
                    "Tiempo de sistema: %ld.%06ld s\n"
                    "Memoria máxima: %ld KB\n"
                    "----------------------------------------\n",
                    usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
                    usage.ru_stime.tv_sec, usage.ru_stime.tv_usec,
                    usage.ru_maxrss);
                close(fdm);
                dprintf(fdm, "Comando ejecutado: ");
                for (int i = 0; comando[i] != NULL; ++i)
                    dprintf(fdm, "%s ", comando[i]);
                dprintf(fdm, "\n");
                dprintf(fdm,
                    "Tiempo de usuario: %ld.%06ld s\n"
                    "Tiempo de sistema: %ld.%06ld s\n"
                    "Memoria máxima: %ld KB\n"
                    "----------------------------------------\n",
                    usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
                    usage.ru_stime.tv_sec, usage.ru_stime.tv_usec,
                    usage.ru_maxrss);
                close(fdm);
            } else {
                perror("No se pudo abrir archivo para métricas");
            }
        } else {
            printf("Tiempo de usuario: %ld.%06ld s\n",
                usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
            printf("Tiempo de sistema: %ld.%06ld s\n",
                usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
            printf("Memoria máxima: %ld KB\n", usage.ru_maxrss);
        }
    }
}
