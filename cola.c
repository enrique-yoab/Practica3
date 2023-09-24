#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cola.h"
#include "utilidades.h"

int generarID(){
    int num;
    srand(time(NULL));
    num = rand() % 900 + 100;
    return num;
}

void iniciaCola(Cola *queue){
    queue->head = NULL;
    queue->tail = NULL;
}

int empty(Cola *queue){
    if (queue->head == NULL){
        return 1;
    }else{ 
        return 0;
    }
}

void push(Cola *queue, char nom){
    srand(time(NULL));
    Nodo *nuevo;
    int espacio [4] = {64, 128, 256, 512};

    //creacion y asignacion de memoria para el nuevo nodo
    nuevo = (Nodo*)malloc(sizeof(Nodo));
    
    //Asigna el nombre del Proceso
    nuevo->nom = nom;
    
    //Ultimo Nodo, no tiene siguiente
    nuevo->next = NULL;

    if (empty(queue)){
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
    printf("Insertado: %c\n", queue->tail->nom);
    
    queue->tail->id = generarID();
    printf("ID: %d\n", queue->tail->id);
    
    queue->tail->instr = numRandom(10, 30);
    printf("Inst: %d\n", queue->tail->instr);

    queue->tail->space = espacio[numRandom(0, 4)];
    printf("Espacio: %d\n\n", queue->tail->space);
}

char pop(Cola *queue){
    if (empty(queue)){
        return -1;
    }else{
        char inf;
        //Auxiliar para recuperar informacion.
        Nodo *aux = queue->head;
        if (queue->head == queue->tail){
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

void showHead(Cola *queue){
    printf("Funcion mostrar\n\n");
    if (empty(queue)){
        printf("La cola esta vacia\n\n");
    }else {
        printf("Nombre: %c\n", queue->head->nom);
        printf("ID: %d\n", queue->head->id);
        printf("Instrucciones: %d\n", queue->head->instr);
        printf("Espacio: %d\n\n", queue->head->space);
    }    
}
