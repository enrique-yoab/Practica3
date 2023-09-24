#ifndef COLA_H
#define COLA_H

typedef struct Nodo
{
    char nom;
    int id;
    int instr;
    int space;
    struct Nodo *next;
} Nodo;

typedef struct Cola
{
    Nodo *head;
    Nodo *tail;
} Cola;
int generarID();
void iniciaCola(Cola *queue);
int empty(Cola *queue);
void push(Cola *queue, char nom);
char pop(Cola *queue);
void showHead(Cola *queue);

#include "cola.h"
#endif
