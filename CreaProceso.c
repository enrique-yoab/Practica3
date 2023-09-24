#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct proceso{    // esta es la estructura del proceso o los componentes que lo conforman
    char nombre[30];
    int identificador;
    int instruccion;
    int instruccionRestante;
    int posicion;
    struct proceso *siguiente; 
} Proceso;
typedef struct cola{   //estructura de la cola, inicio o final
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
    nuevo_proceso->instruccionRestante = nuevo_proceso->instruccion;
    nuevo_proceso->siguiente=NULL;   //se inicializa el puntero siguiente en NULL

    printf("Ingrese el nombre del proceso %d: ",numProceso);
    scanf("%29s",nuevo_proceso->nombre); //limitar la entrada a 29 caracteres
    if(!cola->inicio){                 //entra al if si es el primer proceso en agregarse
        cola->inicio= nuevo_proceso;
        cola->final = nuevo_proceso;
    }else{                              //entre en else si ya hay un proceso inicial
        cola->final->siguiente=nuevo_proceso;
        cola->final=nuevo_proceso;
    }
}
Proceso *quitarProceso(Cola *cola){      //quita el proceso que se encuentra al inicio
    if(!cola->inicio){                 //si entra al if es que ya no hay ningun proceso
        return NULL;
    }
    Proceso *proceso_removido=cola->inicio;  //se crea un apuntador hacia el inicio de la cola
    cola->inicio=cola->inicio->siguiente;   //movemos el puntero de la cola de inicio, al siguiente proceso.
    if(!cola->inicio){             //entra al if si el proceso al que se movio  el puntero es el ultimo de la cola
        cola->final=NULL;
    }
    return proceso_removido;  //retorna el proceso que se quitara osea el que esta al inico de la cola
}
void visualizarCola(Cola *cola){
    Proceso *temp=cola->inicio;
    if(!temp){
        printf("La cola esta vacia\n");
        return;
    }
    printf("Proceso %d en la cola:\n",temp->posicion);
    while(temp){
        printf("ID: %d , Nombre: %s , Numero de instrucciones: %d\n",temp->identificador,temp->nombre,temp->instruccion);
        temp = temp->siguiente;
    }
}

Proceso *verProceso(Cola *cola){
    int aux;
    aux = (cola->inicio->instruccion) - (cola->inicio->instruccionRestante);
    printf("Proceso %s\n", cola->inicio->nombre);
    printf("ID: %d\n", cola->inicio->identificador);
    printf("Instrucciones Totales: %d\n", cola->inicio->instruccion);
    printf("Instrucciones ejecutadas: %d\n", aux);
}

Proceso *ejecutarProceso(Cola *cola){
    int aux;
    if (cola->inicio){
        printf("Cola vacia");
    }else{
        aux = cola->inicio->instruccionRestante - 5;
        cola->inicio->instruccionRestante = aux;
    }
    printf("Proceso %s se ha ejecutado \n", cola->inicio->nombre);
    printf("Instrucciones restantes: %d", cola->inicio->instruccionRestante);
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
    visualizarCola(&cola);
    while(cola.inicio){
        Proceso *proceso_actual=quitarProceso(&cola);
        printf("Proceso Ejecutado %d\n",proceso_actual->posicion);
        free(proceso_actual);
    }
    printf("terminado\n");
}