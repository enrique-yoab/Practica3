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

Proceso *agregarProceso(Cola *cola,int numProceso){   //agrega el proceso a la cola
    Proceso *nuevo_proceso = (Proceso*)malloc(sizeof(Proceso));
    nuevo_proceso->posicion=numProceso;
    nuevo_proceso->identificador=rand()%1000;
    nuevo_proceso->instruccion=10 + rand()%(31-10); //numero de instrucciones dado por el programa entre 10 y 30
    nuevo_proceso->siguiente=NULL;

    printf("Ingrese el nombre del proceso %d: ",numProceso);
    scanf("%29s",nuevo_proceso->nombre); //limitar la entrada a 29 caracteres
    if(cola->inicio!=NULL){
        cola->inicio= nuevo_proceso;
        cola->final = nuevo_proceso;
    }else{
        cola->final->siguiente=nuevo_proceso;
        cola->final=nuevo_proceso;
    }
}
Proceso *quitarProceso(Cola *cola){
    if(!cola->inicio){
        return NULL;
    }
    Proceso *proceso_removido=cola->inicio;
    cola->inicio=cola->inicio->siguiente;
    if(!cola->inicio){
        cola->final=NULL;
    }
    return proceso_removido;
}
void visualizarCola(Cola *cola){
    Proceso *temp=cola->inicio;
    if(!temp){
        printf("La cola esta vacia\n");
        return;
    }
    printf("Proceso %d en la cola:\n",temp->posicion);
    while(temp){
        printf("ID: %d , Nombre: %s , Numero de instrucciones: %d",temp->identificador,temp->nombre,temp->instruccion);
        temp = temp->siguiente;
    }
}

void main(){
    srand(time(NULL));
    Cola cola;
    crearCola(&cola); //se inicializa la cola de los procesos
    int numeroProc;
    printf("¿Cuantos procesos desea crear?: ");
    scanf("%d",&numeroProc);
    for(int i=1;i<=numeroProc;i++){
        agregarProceso(&cola,i);
    }
}