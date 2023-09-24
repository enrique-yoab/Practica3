#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct proceso{    // esta es la estructura del proceso o los componentes que lo conforman
    char nombre[30];
    int identificador;
    int instruccion;
    int instruccionRestante;
    int instruccionEjecutadas;
    int posicion;
    struct proceso *siguiente; 
} Proceso;
typedef struct cola_Procesos{   //estructura de la cola, inicio y final
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
        cola->final=NULL;   //se pone en NULL el puntero del final de la cola
    }
    return proceso_removido;  //retorna el proceso que se quitara osea el que esta al inico de la cola
}
void visualizarCola(Cola *cola, int numeroPr){
    Proceso *temp=cola->inicio;
    if(!temp){
        printf("La cola esta vacia\n");
        return;
    }
    printf("Proceso %d en la cola\n",temp->posicion);
    while(temp->posicion!=numeroPr){
        temp = temp->siguiente;
    }
    printf("El proceso %d, con ID: %d , Nombre: %s , Numero de instrucciones: %d\n",temp->posicion,temp->identificador,temp->nombre,temp->instruccion);
}

void mostrarCola(Cola *cola) {
    Proceso *temp = cola->inicio;
    if (!temp) {
        printf("La cola está vacía.\n");
        return;
    }

    printf("Procesos %d en la cola:\n",temp->posicion);
    while (temp) {
        printf("El proceso %d, con ID: %d , Nombre: %s , Numero de instrucciones: %d\n",temp->posicion,temp->identificador,temp->nombre,temp->instruccion);
        temp = temp->siguiente;
    }
}

void procesoActivo(Cola *cola){
    Proceso *aux=cola->inicio;
    if(!aux){
        printf("La cola esta vacia\n");
        return;
    }
    printf("El proceso activo es %s\n",aux->nombre);
    printf("ID: %d\n",aux->identificador);
    printf("Instrucciones Totales: %d\n",aux->instruccion);
    printf("Instrucciones Ejecutadas: %d\n",aux->instruccionEjecutadas);
}


void ejecutarProceso(Cola *cola){ //se ejecuta el proceso actual, el que esta al inicio de la cola
    int aux; //auxiliar para restar instrucciones
    if (!cola->inicio){ //validacion de una cola vacia
        printf("\nCola vacia\n");
    }
    else{
        aux = cola->inicio->instruccionRestante - 5;
        cola->inicio->instruccionRestante = aux; //recuperacion del conteo de instrucciones restantes
        printf("\nProceso %s se ha ejecutado \n", cola->inicio->nombre); //Muestra el nombre del proceso ejecutadp
        printf("Instrucciones restantes: %d\n", cola->inicio->instruccionRestante); //Muestra las instrucciones restantes
        cola->inicio->instruccionEjecutadas=cola->inicio->instruccionEjecutadas + 5;
    }
}
void *procesoSiguiente(Cola *cola, int valor){
    Proceso *aux = cola->inicio;
    if(!cola->inicio || !cola->inicio->siguiente){
        printf("Solo hay un elemento asi que es el primero y ultimo elemento de la cola\n");
    }else{
        switch(valor){
            case 1:  //se agrega el proceso activo al final de la cola
                cola->inicio = cola->inicio->siguiente;
                aux->siguiente = NULL;   //desconecta el proceso de la cola

                cola->final->siguiente=aux;  //inserta el proceso al final de la cola
                cola->final=aux;       //se actualiza el puntero final;
                break;
            case 2:
                break;
            default:
                break;
        }
    }
}

void simulador(){
    Cola cola;
    crearCola(&cola);  //se inicializa la cola de los procesos
    printf("Bienvenido al simulador de Procesos\n\n");
    int numeroProc, procesoSel, x;  //variable donde se guardara el numero de procesos
    printf("¿Cuantos procesos desea crear?: ");
    scanf("%d",&numeroProc);
    for(int i=1;i<=numeroProc;i++){
        agregarProceso(&cola,i);
    }
    
    do{ 
        printf("\nIngrese la opcion deseada\n");
        printf("1)Ver proceso activo\n");
        printf("2)Ejecutar Proceso actual\n");
        printf("3)Pasar al proceso siguiente (puede colocar el proceso al final de cola, ir a la cola de E/S)\n");
        printf("4)vizualizar la cola\n");
        scanf("%d",&x);
        switch (x){
        case 1:
            procesoActivo(&cola);
            break;
        case 2:
            ejecutarProceso(&cola);
            break;
        case 3:
            int y=1 + rand()%(4-1);
            procesoSiguiente(&cola,1);
            break;
        case 4:
            mostrarCola(&cola);
            break;
        default:
            printf("***Opcion no valida***\n Ingrese una de las que aparece en el menu\n");
            break;
        }
    }while(x!=5);

    while(cola.inicio){
        Proceso *proceso_actual=quitarProceso(&cola);
        printf("Proceso Ejecutado %d\n",proceso_actual->posicion);
        free(proceso_actual);
    }
}

void main(){
    srand(time(NULL));
    simulador();
    printf("terminado\n");
}
