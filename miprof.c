#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/time.h>

// Variable global para manejar el PID del proceso hijo
pid_t child_pid = -1;

// Manejador de la señal SIGALRM para el timeout, esto termina el proceso hijo y el padre
void timeout_handler(int sig) {
    (void)sig; // Evita el warning de "unused parameter"
    if (child_pid > 0) {
        kill(child_pid, SIGTERM);
    }
}

// Función principal para ejecutar miprof
void ejecutar_miprof(char **arr) {
    // Variables para los modos y argumentos
    char *archivoMiprof = NULL;
    int timelimit = -1;
    int cmd_index = 0;
    
    if (arr[1] == NULL) {
        printf("Uso: miprof <modo> [args...]\n");
        return;
    }

    char *modo = arr[1];

    // Determinar el modo de operación
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

    // Construir argv para exec
    char **comando = &arr[cmd_index];
    if (comando[0] == NULL) {
        printf("Falta el comando a ejecutar.\n");
        return;
    }
    
    // Medir tiempo real
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    
    child_pid = fork();
    if (child_pid < 0) {
        perror("fork fallo");
        child_pid = -1;
        return;
    }

    if (child_pid == 0) {
        // Proceso Hijo

        // Redirigir salida si es modo ejecsave
        if (strcmp(modo, "ejecsave") == 0) {
            int fd = open(archivoMiprof, O_WRONLY | O_CREAT | O_APPEND, 0664);
            if (fd == -1) {
                perror("Error al abrir archivo");
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("dup2");
                close(fd);
                exit(EXIT_FAILURE);
            }
            close(fd);
        }

        // Ejecutar el comando
        execvp(comando[0], comando);
        perror("execvp fallo");
        exit(EXIT_FAILURE);
    } else {
        // Proceso Padre

        // Esperar al hijo y manejar timeout si es necesario
        int status;
        struct rusage usage;
        
        // Configurar alarma si hay limite de tiempo
        if (timelimit > 0) {
            signal(SIGALRM, timeout_handler);
            alarm(timelimit);
        }

        waitpid(child_pid, &status, 0);

        // Desactivar la alarma si el proceso termino antes del timeout
        if (timelimit > 0) {
            alarm(0); // Desactivar la alarma
        }
        
        // Resetear el PID del hijo
        gettimeofday(&end_time, NULL);
        
        // Si el proceso fue terminado por la señal de timeout
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGTERM) {
            printf("Proceso finalizado por timeout\n");
        }

        getrusage(RUSAGE_CHILDREN, &usage);

        // Calcular el tiempo real
        long real_time_sec = end_time.tv_sec - start_time.tv_sec;
        long real_time_usec = end_time.tv_usec - start_time.tv_usec;
        if (real_time_usec < 0) {
            real_time_sec--;
            real_time_usec += 1000000;
        }

        if (strcmp(modo, "ejecsave") == 0) {
            int fdm = open(archivoMiprof, O_WRONLY | O_CREAT | O_APPEND, 0664);
            if (fdm != -1) {
                dprintf(fdm, "\n----------------------------------------\n");
                dprintf(fdm, "Comando ejecutado: ");
                for (int i = 0; comando[i] != NULL; ++i)
                    dprintf(fdm, "%s ", comando[i]);
                dprintf(fdm, "\n");
                
                dprintf(fdm,
                        "Tiempo de usuario: %ld.%06ld s\n"
                        "Tiempo de sistema: %ld.%06ld s\n"
                        "Tiempo real: %ld.%06ld s\n"
                        "Memoria máxima: %ld KB\n",
                        usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
                        usage.ru_stime.tv_sec, usage.ru_stime.tv_usec,
                        real_time_sec, real_time_usec,
                        usage.ru_maxrss);
                close(fdm);
            } else {
                perror("No se pudo abrir archivo para métricas");
            }
        } else {
            // mostrar por pantalla
            printf("\n----------------------------------------\n");
            printf("Tiempo de usuario: %ld.%06ld s\n",
                usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
            printf("Tiempo de sistema: %ld.%06ld s\n",
                usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
            printf("Tiempo real: %ld.%06ld s\n",
                real_time_sec, real_time_usec);
            printf("Memoria máxima: %ld KB\n", usage.ru_maxrss);
        }
    }
}