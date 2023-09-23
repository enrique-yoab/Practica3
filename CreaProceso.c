#include <stdio.h>
#include <unistd.h>
#include <time.h>

typedef struct nodo{
    char nombre;
    int identificador;
    int instruccion;
    struct nodo *siguiente; 
} Nodo;

Nodo *agregarValores (Nodo *cola, char name,int id_proceso,int numInstr){
    srand(time(NULL));
    Nodo *temporal = calloc(sizeof(Nodo),1);
    temporal->nombre=name;
    temporal->identificador=id_proceso;
    temporal->instruccion=rand()%11 + 20;
    if(cola != NULL){
        while(cola->siguiente != NULL){
            cola=cola->siguiente;
        }
    }else{
        return temporal;
    }
}

int main(){
    struct proceso *padre=NULL;
}