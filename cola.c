include <stdio.h>
include <stdlib.h>
include <string.h>

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

bool empty(Cola *queue){
    return (queue->head == NULL);
}

void push(Cola *queue, char id){
    //creacion y asignacion de memoria para el nuevo nodo
    struct Nodo *nuevo = (Nodo)malloc(sizeof(struct Nodo));
    //Asigna el ID del Proceso
    nuevo->nom = id;
    //Ultimo Nodo, no tiene siguiente
    nuevo->next = NULL;
    if (empty()){
        queue->head = nuevo;
        queue->tail = nuevo;
    }else{
        //Asignamos el nuevo al siguiente del tail
        queue->tail->next = nuevo;
        //Igualamos a tail al elemento nuevo
        queue->tail = nuevo;
    }

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
