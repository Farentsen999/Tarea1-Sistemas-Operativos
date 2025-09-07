#include <unistd.h>
#include <sys/wait.h>
#include "prompt.h"
#include "process.h"

int main(void) {
    char *arr[MAX_STRINGS]; // areglo de strings

    while (1) {
        leer_y_split(arr);
        ejecutar_comando(arr);
    }
}
