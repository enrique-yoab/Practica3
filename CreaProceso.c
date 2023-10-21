#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
typedef struct proceso{    // esta es la estructura del proceso o los componentes que lo conforman
    char nombre[30];
    int identificador;
    int instruccion;
    int instruccionRestante;
    int instruccionEjecutadas;
    int posicion;
    int espacio;
    struct proceso *siguiente; 
} Proceso;

typedef struct cola_Procesos{   //estructura de la cola, inicio y final
    char nombre[30];
    Proceso *inicio;
    Proceso *final;
}Cola;

void crearCola(Cola *cola, char *name){ //solo inicializa la cola de los procesos
    strcpy(cola->nombre,name);
    cola->final=NULL;
    cola->inicio=NULL;
}

Proceso *agregarProceso(Cola *cola,int numProceso,int *x,int y[]){   //agrega el proceso a la cola
    if(*x < 512){
        printf("Ya no hay memoria\n");
        return NULL;
    }
    Proceso *nuevo_proceso = (Proceso*)malloc(sizeof(Proceso));
    nuevo_proceso->posicion=numProceso;
    nuevo_proceso->identificador=rand()%1000;
    nuevo_proceso->instruccion=10 + rand()%(21); //numero de instrucciones dado por el programa entre 10 y 30
    nuevo_proceso->instruccionRestante = nuevo_proceso->instruccion;
    nuevo_proceso->siguiente=NULL;   //se inicializa el puntero siguiente en NULL para que no apunte a ningun otro nodo
    int aux1=rand()%4;   // valores entre 0 y 3
    nuevo_proceso->espacio=y[aux1];  //se le asigna la memoria al proceso
    *x=*x-y[aux1]; //se le resta le memoria del proceso a la memoria principal
    printf("Ingrese el nombre del proceso %d: ",numProceso);
    scanf("%29s",nuevo_proceso->nombre); //limitar la entrada a 29 caracteres
    if(!cola->inicio){                 //entra al if si es el primer proceso en agregarse
        cola->inicio= nuevo_proceso;
        cola->final = nuevo_proceso;
    }else{                              //entre en else si ya hay un proceso inicial
        cola->final->siguiente=nuevo_proceso;
        cola->final=nuevo_proceso;
    }
    printf("La memoria restante es %d\n",*x);
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

void mostrarCola(Cola *cola) {
    Proceso *temp = cola->inicio;
    if (!temp) {
        printf("La cola de %s esta vacia\n",cola->nombre);
        return;
    }
    printf("\n--------Cola de %s--------\n",cola->nombre);
    while (temp) {
        if(cola->inicio==temp){
            printf("El proceso %s (Proceso Activo) , ID: %d , Instrucciones pendientes: %d , Memoria: %d,\n",temp->nombre,temp->identificador,temp->instruccionRestante,temp->espacio);
        temp = temp->siguiente;
        }else{
        printf("El proceso %s (posicion %d) , ID: %d , Instrucciones pendientes: %d , Memoria: %d,\n",temp->nombre,temp->posicion,temp->identificador,temp->instruccionRestante,temp->espacio);
        temp = temp->siguiente;
        }
    }
}
void mostrarCola2(Cola *cola) {
    Proceso *temp = cola->inicio;
    if (!temp) {
        printf("\n--------La cola de %s esta vacia--------\n",cola->nombre);
        return;
    }

    printf("\n--------Cola de %s--------\n",cola->nombre);
    while (temp) {
        printf("El proceso %s (posicion %d) , ID: %d , Instrucciones pendientes: %d , Memoria: %d,\n",temp->nombre,temp->posicion,temp->identificador,temp->instruccionRestante,temp->espacio);
        temp = temp->siguiente;  
    }
}

void procesoActivo(Cola *cola){
    Proceso *aux=cola->inicio;
    if(!aux){
        printf("La cola de %s esta vacia\n",cola->nombre);
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
        printf("La cola de %s esta vacia\n",cola->nombre);
    }
    else{
        aux = cola->inicio->instruccionRestante - 5;
        cola->inicio->instruccionRestante = aux; //recuperacion del conteo de instrucciones restantes
        printf("\nProceso %s se ha ejecutado \n", cola->inicio->nombre); //Muestra el nombre del proceso ejecutadp
        printf("Instrucciones restantes: %d\n", cola->inicio->instruccionRestante); //Muestra las instrucciones restantes
        cola->inicio->instruccionEjecutadas=cola->inicio->instruccionEjecutadas + 5;
    }
}
void *procesoSiguiente(Cola *cola, Cola *ES, int valor){
    Proceso *aux = cola->inicio;
    if(!cola->inicio || !cola->inicio->siguiente){
        printf("Es el unico elemento de la cola o no hay elemento\n");
    }else{
        switch(valor){
            case 0:  //se agrega el proceso activo al final de la cola
                printf("Se movio al final de la cola de procesos\n");
                cola->inicio = cola->inicio->siguiente;
                aux->siguiente = NULL;   //desconecta el proceso de la cola

                cola->final->siguiente=aux;  //inserta el proceso al final de la cola
                cola->final=aux;       //se actualiza el puntero final;
                break;
            case 1:
                printf("Se movio a la cola de E/S\n");
                cola->inicio = cola->inicio->siguiente;
                aux->siguiente = NULL;   //desconecta el proceso de la cola

                ES->final->siguiente=aux;  //inserta el proceso al final de la cola
                ES->final=aux;       //se actualiza el puntero final;
                break;
            default:
                break;
        }
    }
}

void ejecutarProcesoES_IN(Cola *cola,Cola *tmp){ //se ejecutara el proceso inicial de la cola de E/S y de Interrupciones, agregandose en la cola de procesos, el proceso inicial
    int aux; //auxiliar para restar instrucciones
    if (!tmp->inicio){ //validacion de una cola vacia
        printf("La cola de %s esta vacia\n",tmp->nombre);
        return;
    }else{
        aux = tmp->inicio->instruccionRestante - 5; //el aux guardara el resultado de la diferencia de las instrucciones - 5
        tmp->inicio->instruccionRestante = aux; //recuperacion del conteo de instrucciones restantes
        printf("\nProceso %s se ha ejecutado \n", tmp->inicio->nombre); //Muestra el nombre del proceso ejecutadp
        printf("Instrucciones restantes: %d\n", tmp->inicio->instruccionRestante); //Muestra las instrucciones restantes
        tmp->inicio->instruccionEjecutadas=tmp->inicio->instruccionEjecutadas + 5;
        Proceso *puntero = tmp->inicio;
        tmp->inicio=tmp->inicio->siguiente;

        if(!cola->inicio){   //si la cola no tiene ningun proceso se agrega a la cola
            cola->inicio = puntero;
            cola->final = puntero;
            puntero->siguiente=NULL;
        }else{  //si no lo agregamos al final de la cola
            cola->final->siguiente = puntero;
            cola->final = puntero; //se actualiza el final de la cola con nodo que se movio
            puntero->siguiente=NULL;
        }
    }
}
void matarProceso(Cola *cola, Cola *tmp, int *x){ //se eliminara el proceso inicial de la cola de procesos y se agregara a la de eliminados
    if (!tmp->inicio){ //validacion de una cola vacia
        printf("La cola de %s esta vacia\n",tmp->nombre);
        return;
    }else{
        Proceso *puntero = tmp->inicio;
        tmp->inicio=tmp->inicio->siguiente;
        if(!cola->inicio){   //si la cola no tiene ningun proceso se agrega a la cola
            cola->inicio = puntero;
            cola->final = puntero;
            puntero->siguiente=NULL;
        }else{  //si no lo agregamos al final de la cola
            cola->final->siguiente = puntero;
            cola->final = puntero; //se actualiza el final de la cola con nodo que se movio
            puntero->siguiente=NULL;
        }
        *x = puntero->espacio + *x;  //se libera la memoria
        printf("La memoria total: %d",*x);
    }
}
int colaVacia(Cola *cola){
    if(cola->inicio!=NULL && cola->final!=NULL){
        printf("Hay procesos en la cola de PROCESOS\n");
        return 1;
    }else{
        printf("La cola esta vacia");
        return 0;
    }
}
 
void simulador(){
    Cola cola, E_S, interrupcion, eliminados, *colaActual;  //se crean varias colas una para los procesos, E/S y las interrupciones
    crearCola(&cola,"Procesos");  //se inicializa la cola de los procesos
    crearCola(&E_S,"E/S");  //se inicializa la cola E/S
    crearCola(&interrupcion,"Interrupciones");
    crearCola(&eliminados,"Procesos Eliminados");
    colaActual=&cola;
    printf("Bienvenido al simulador de Procesos\n\n");
    int numeroProc, x, s, z,i,j,k, espacioGlobal=2048, arreglo[] = {64,128,256,512}, contador=0,a;  //variable donde se guardara el numero de procesos

    do{ 
        printf("\nSe encuentra en la cola de %s\n",colaActual->nombre);
        printf("---------------------MENU-----------------------\n");
        printf("\nIngrese la opcion deseada\n");
        printf("0)Crear Proceso (proceso, E/S, Interrupciones)\n");
        printf("1)Ver proceso activo\n");
        printf("2)Ejecutar Proceso actual\n");
        printf("3)Pasar al proceso siguiente (puede colocar el proceso al final de cola, moverse a la cola E/S)\n");
        printf("4)Ver estado actual del sistema\n");
        printf("5)Ejecutar Entrada y Salida\n");
        printf("6)Ejecutar Intrrupcion\n");
        printf("7)Matar Proceso\n");
        printf("8)Imprimir lista de procesos Preaparados\n");
        printf("9)Imprimir lista de procesos de E/S\n");
        printf("10)imprimir la lista de interrupciones pendientes\n");
        printf("11)Salida\n");
        printf("---------------------Ejecucion de la opcion elegida--------------------\n");
        scanf("%d",&x);
        switch (x){
        case 0:
            printf("Que proceso desea crer\n1)Proceso X\n2)Entrada/Salida\n3)Interrupcion\nOpcion == ");
            scanf("%d",&a);
            if(a==1){
                printf("\n¿Cuantos procesos desea crear?: ");
                scanf("%d",&numeroProc);
                for(i=1;i<=numeroProc;i++){
                contador=contador+1;
                agregarProceso(colaActual,contador,&espacioGlobal,arreglo);
                }
            }else if(a==2 && colaActual==&cola){
                printf("\n¿Cuantos proceso de E/S desea tener?: ");
                scanf("%d",&z);
                for(j=1;j<=z;j++){
                contador=contador+1;
                agregarProceso(&E_S,contador,&espacioGlobal,arreglo);
                }
            }else if(a==3 && colaActual==&cola){
                printf("\n¿Cuantos proceso de interrupcion desea tener?: ");
                scanf("%d",&s);
                for(k=1;k<=s;k++){
                contador=contador+1;
                agregarProceso(&interrupcion,contador,&espacioGlobal,arreglo);
                }
            }
            break;
        case 1:
            procesoActivo(colaActual);
            break;
        case 2:
            ejecutarProceso(colaActual);
            break;
        case 3:
            int y=0+ rand()%(2);
            procesoSiguiente(&cola,&E_S,y);
            break;
        case 4:
            mostrarCola(&cola);
            mostrarCola2(&E_S);
            mostrarCola2(&interrupcion);
            mostrarCola2(&eliminados);
            break;
        case 5:
            ejecutarProcesoES_IN(&cola,&E_S);
            break;  
        case 6:
            ejecutarProcesoES_IN(&cola,&interrupcion);
            break;
        case 7:
            matarProceso(&eliminados,colaActual,&espacioGlobal);
        case 8:
            mostrarCola2(&cola);
        case 9:
            mostrarCola2(&E_S);
        case 10:
            mostrarCola2(&interrupcion);
            break;
        case 11:
            int A;
            char B[1];
            A=colaVacia(&cola);
            if(A==1){
                printf("Desea eliminar todos los procesos que quedan? \nSi(Y), No(N) = ");
                scanf("%s",B);
                if(B[0]=='Y' || B[0]=='S'){
                    while(cola.inicio){
                        Proceso *proceso_actual=quitarProceso(&cola);
                        printf("\nProceso Ejecutado %s\n",proceso_actual->nombre);
                        free(proceso_actual);
                    }
                    while(E_S.inicio){
                        Proceso *proceso_actual=quitarProceso(&E_S);
                        printf("Proceso Ejecutado %s\n",proceso_actual->nombre);
                        free(proceso_actual);
                    }
                    while(interrupcion.inicio){
                        Proceso *proceso_actual=quitarProceso(&interrupcion);
                        printf("Proceso Ejecutado %s\n",proceso_actual->nombre);
                        free(proceso_actual);
                    }
                    while(interrupcion.inicio){
                        Proceso *proceso_actual=quitarProceso(&eliminados);
                        printf("Proceso Ejecutado %s\n",proceso_actual->nombre);
                        free(proceso_actual);
                    }
                    x=12;
                }
            }
            break;
        default:
            break;
        }
    }while(x!= 12);

    printf("Hasta pronto :)\n");
}

void main(){
    srand(time(NULL));
    simulador();
    printf("terminado\n");
}
                                                                                                                                            