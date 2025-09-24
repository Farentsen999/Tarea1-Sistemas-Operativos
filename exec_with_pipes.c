#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "config.h"
#include <stdlib.h>

void close_pipes(int fd[][2], int pipes){
    for (int i = 0; i < pipes; i++) {
        close(fd[i][0]);
        close(fd[i][1]);        
    }
}

void wait_pids(pid_t pids[], int pipes){
    for (int i = 0; i < pipes + 1; i++) {
        waitpid(pids[i], NULL, 0);
    }

}

void exec_with_pipes(char **comandos[], int pipes){
    int fd[pipes][2];
    int procesos = pipes + 1;

    pid_t pids[procesos];

    for (int i = 0; i < pipes; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            close_pipes(fd, i);   
            exit(1);
        }
    }

    for (int i = 0; i < procesos; i++) { 

        pids[i] = fork();

        if (pids[i] == 0){
            
            if (i<pipes){
                dup2(fd[i][1], STDOUT_FILENO);
            }
            if (i>0){
                dup2(fd[i-1][0], STDIN_FILENO);
            }
            close_pipes(fd, pipes);
            execvp(comandos[i][0], comandos[i]);
        }
    }

    close_pipes(fd, pipes);
    wait_pids(pids, pipes);
}