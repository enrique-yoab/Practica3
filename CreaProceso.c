#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct nodo{
    char nombre;
    int identificador;
    int instruccion;
    int posicion;
    struct nodo *siguiente; 
} Nodo;

Nodo *agregarProceso(Nodo *cola, char name,int id_proceso,int numInstr,int num){
    Nodo *temporal = calloc(sizeof(Nodo),1);
    Nodo *pivote;

    temporal->nombre=name;
    temporal->identificador=id_proceso;
    temporal->instruccion=numInstr;
    temporal->posicion=num;
    if(cola != NULL){
        pivote=cola;
        while(pivote->siguiente != NULL){
            pivote=pivote->siguiente;
        }
        pivote->siguiente = temporal;
        printf("Agregando proceso(%i) despues de proceso (%i) a la cola\n",temporal->posicion, pivote->posicion);
        return cola; 
    }else{
        printf("Agregando proceso(%i) a la cola\n",temporal->posicion);
        return temporal;
    }
}

int main(){
    Nodo *cola=NULL;
    int aux; char temp[20];
    srand(time(NULL));
    printf("Nombre del proceso = ");
    scanf("%s",&temp);
    cola=agregarProceso(cola,temp,15236,rand()%11+20,1);
    printf("\nel nombre es %s",cola->nombre);
    /**printf("Ingrese el numero de procesos a realizar\n");
    scanf("%d",&aux);
    for(int i=1;i<=aux;i++){
        printf("Nombre del proceso (%d)= ",i);
        scanf("%s",&temp)
        cola;
    }**/

}