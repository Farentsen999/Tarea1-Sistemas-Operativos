#include <unistd.h>
#include <sys/wait.h>
#include "prompt.h"

int main(void) {
    pid_t pid;
    char *arr[MAX_STRINGS]; // areglo de strings

    while (1) {
        leer_y_split(arr);

        pid = fork();
        if (pid == 0) {
            execvp(arr[0], arr);   // ejecuta el comando ingresado
            _exit(127);
        } else {
            wait(NULL);
        }
    }
}
