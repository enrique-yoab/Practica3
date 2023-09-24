#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct proceso{    // esta es la estructura del proceso o los componentes que lo conforman
    char nombre[30];
    int identificador;
    int instruccion;
    int posicion;
    struct proceso *siguiente; 
} Proceso;
typedef struct cola{
    Proceso *inicio;
    Proceso *final;
}Cola;

void crearCola(Cola *cola){ //solo inicializa la cola de los procesos
    cola->final=NULL;
    cola->inicio=NULL;
}

Proceso *agregarProceso(Cola *cola,int id){
    Proceso *nuevo_proceso = (Proceso*)malloc(sizeof(Proceso));
    nuevo_proceso->posicion=id;
    nuevo_proceso->identificador=10 + rand()%(31-10); //numero de instrucciones dado por el programa entre 10 y 30
    nuevo_proceso->siguiente=NULL;

    printf("Ingrese el nombre del proceso %d: ",id);
    scanf("%29s",nuevo_proceso->nombre); //limitar la entrada a 29 caracteres
    if(cola!=NULL){
        cola->inicio= nuevo_proceso;
        cola->final = nuevo_proceso;
    }else{
        cola->final->siguiente=nuevo_proceso;
        cola->final=nuevo_proceso;
    }
}


int main(){
   srand(time(NULL));

}