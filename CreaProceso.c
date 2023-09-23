#include <stdio.h>
#include <unistd.h>

struct proceso{
    char nombre[];
    int identificador;
    int instruccion;
    struct proceso *siguiente; 
}

struct proceso *agregar (struct proceso *cola, char *name,int id_proceso,int numInstr){
    if(cola != NULL){

    }else{

    }
}

int main(){
    struct proceso *padre=NULL;
}