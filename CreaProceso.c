#include <stdio.h>
#include <unistd.h>

struct Nodo{
    char nombre;
    struct nodo *siguiente; 
}