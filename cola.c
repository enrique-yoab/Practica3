#include <stdio.h>
#include "cola.h"

bool empty(Cola *queue){
    return (queue->head == NULL);
}

void push(Cola *queue, char nom){
    int espacio [4] = {64, 128, 256, 512};
    //creacion y asignacion de memoria para el nuevo nodo
    struct Nodo *nuevo = (Nodo)malloc(sizeof(struct Nodo));
    //Asigna el ID del Proceso
    nuevo->nom = id;
    //Ultimo Nodo, no tiene siguiente
    nuevo->next = NULL;
    if (empty()){
        //Si esta vacia la cola head y tail apuntan a nuevo
        queue->head = nuevo;
        queue->tail = nuevo;
    }else{
        //Asignamos el nuevo al siguiente del tail
        queue->tail->next = nuevo;
        //Igualamos a tail al elemento nuevo
        queue->tail = nuevo;
    }
    queue->tail->nom = nom;
    queue->tail->id = id_process;
    queue->tail->instr = rand() % 21 + 10;
    queue->tail->space = espacio[rand() % 4];
       

}

char pop(Cola *queue){
    if (empty()){
        return -1;
    }else{
        char inf;
        //Auxiliar para recuperar informacion.
        struct Nodo *aux = queue->head;
        if (queue->head == tail){
            queue->head = NULL;
            queue->tail = NULL;
        }else{
            queue->head = queue->head->next;
        }
        inf = aux->nom;
        free (aux);
        return inf;
    }
}

