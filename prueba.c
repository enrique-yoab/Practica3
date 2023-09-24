#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cola.h"

int main(){
    Cola procesos;
    iniciaCola(&procesos);
    push(&procesos, 'A');
    push(&procesos,'B');
    push(&procesos, 'C');
    push(&procesos, 'D');
    showHead(&procesos);
    printf("Eliminado: %c \n", pop(&procesos));
    printf("Eliminado: %c \n", pop(&procesos));
    showHead(&procesos);
    return 0;
}