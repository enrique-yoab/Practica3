#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cola.h"

typedef struct Nodo
{
    char nom;
    int id;
    int instr;
    int space;
    struct Nodo *next;
} Nodo;

typedef struct Cola{
    Nodo *head;
    Nodo *tail;
} Cola;



void main(){

}