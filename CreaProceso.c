#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
/***Aqui inician las estructuras de procesos, memoria y paginas ***/
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
} Cola;
typedef struct memory{ //estructura del bloque de memoria
  char nombreProceso[30];
  int espacio;
  int registroBase,registroLimite;
  Proceso inicioProceso;
  struct memory *next;
}Bloque;

typedef struct ColaMemory{ //estructura de la memoria en si
    char nombreUnidad[30];
    int espacioGlobal; //memoria global
    Bloque *first;
    Bloque *last;
}memoria;

typedef struct page{ //estructura de la pagina
    char nombreProc[30];//almacenara el nombre del proceso
    int REGBA, REGLIM;//almacenara el registro base y limite de cada pagina que es de 16 direcciones
    int numPag;//tendra un numero de pagina
    struct page *otraPagina;
} Pagina;

typedef struct TablePages{
    //TABLA DE PAGINAS
    char nombreCola[30];//almacenara el nombre de la cola de procesos, E/S, Interrupciones ó Eliminados
    Pagina *inicio;
    Pagina *final;
    struct TablePages *siguienteTabla;
}Tabla;
/***Hasta aqui son las estructuras de los procesos, memoria y paginas  ***/

void crearCola(Cola *cola, char *name){ //solo inicializa la cola de los procesos
    strcpy(cola->nombre,name);
    cola->final=NULL;
    cola->inicio=NULL;
}

void crearMemoria(memoria *Block, char *name){ //se crea la cola de memoria
    strcpy(Block->nombreUnidad,name);
    Block->first=NULL;
    Block->last=NULL;
    Block->espacioGlobal=1064;
}
void crearTabla(Tabla *T1, char *name){//se crea la Tabla que tendra las paginas de cada proceso
    strcpy(T1->nombreCola,name);
    T1->inicio=NULL;
    T1->final=NULL;
}

void *agregarHuecos(memoria *U1,int *base, int *limite){ //llenara la memoria con procesos huecos 
// con tamaños de 16 unidades para poder fragmentar la memoria
    memoria *tmp=U1;
    int aux=1;
    do{
        if(tmp->espacioGlobal>8){//la memoria global es de 1064 unidades
            Proceso *VACIO=(Proceso*)malloc(sizeof(Proceso));//se crea el proceso HUECO
            strcpy(VACIO->nombre,"Hueco");
            VACIO->espacio=16;
            VACIO->identificador=0;
            VACIO->instruccion=0;
            VACIO->instruccionEjecutadas=0;
            VACIO->instruccionRestante=0;
            VACIO->posicion=aux;
            Bloque *hueco=(Bloque*)malloc(sizeof(Bloque));
            if(!hueco){
                printf("Error al crear un hueco\n");
                exit(1);
            }
            hueco->espacio=16;
            hueco->registroBase=*base;
            hueco->registroLimite=*limite;
            hueco->inicioProceso=*VACIO;//no se le asigna ningun proceso
            hueco->next=NULL;
            strcpy(hueco->nombreProceso,VACIO->nombre);
            if(!tmp->first){
                tmp->first=hueco;
                tmp->last=hueco;
            }else{
                tmp->last->next=hueco;
                tmp->last=hueco;
            }
            *base=*limite+1; //se actualiza el registro base
            *limite+=16;//se actualiza el registro limite
            tmp->espacioGlobal-=hueco->espacio;
            aux+=1;
        }else{
            Proceso *VACIO=(Proceso*)malloc(sizeof(Proceso));//se crea el proceso HUECO
            strcpy(VACIO->nombre,"Hueco");
            VACIO->espacio=8;
            VACIO->identificador=0;
            VACIO->instruccion=0;
            VACIO->instruccionEjecutadas=0;
            VACIO->instruccionRestante=0;
            VACIO->posicion=aux;
            Bloque *hueco=(Bloque*)malloc(sizeof(Bloque));
            if(!hueco){
                printf("Error al crear un hueco\n");
                exit(1);
            }
            hueco->espacio=8;
            hueco->registroBase=*base;
            hueco->registroLimite=*limite-8;
            hueco->inicioProceso=*VACIO;//no se le asigna ningun proceso
            hueco->next=NULL;
            strcpy(hueco->nombreProceso,VACIO->nombre);
            if(!tmp->first){
                tmp->first=hueco;
                tmp->last=hueco;
            }else{
                tmp->last->next=hueco;
                tmp->last=hueco;
            }
            *base=*limite+1; //se actualiza el registro base
            *limite+=8;//se actualiza el registro limite
            tmp->espacioGlobal-=hueco->espacio;
            aux+=1;
        }
    }while(tmp->espacioGlobal>0);
    tmp->espacioGlobal=1064;
    printf("El espacio de memoria disponible es %d\n",tmp->espacioGlobal);
}

Bloque *asignarMemoria(Proceso *nuevoProceso, memoria *U1,int *base, int *limite){
    Bloque *nuevoBloque=(Bloque*)malloc(sizeof(Bloque));
    Bloque *parcial=U1->first;
    Bloque *anterior=NULL;
    int x,parte=1;
    do{
        if(parcial->registroBase == *base && parcial->registroLimite == *limite){
            //se hace el bucle para que se encuentre en el hueco con el registro base y limite
            //si lo encuentra termina el bucle
            break;
        }
        anterior=parcial; //guarda el bloque anterior por si no es el primero
        parcial=parcial->next;
        parte++;
        x=0;
    }while(!x==1);
    //ya se encontro el hueco se procede a liberarlo y a asignar el proceso al hueco  
    //y restamos el espacio global se desconecta de la memoria
    //agregamos el nuevo bloque a memoria
    nuevoProceso->posicion=parcial->inicioProceso.posicion;
    nuevoBloque->espacio=16;
    nuevoBloque->registroBase=parcial->registroBase;
    nuevoBloque->registroLimite=parcial->registroLimite;
    nuevoBloque->inicioProceso=*nuevoProceso; //se le asigna el nuevoProceso
    //se le asigna al nuevo proceso la posicion que tenia el proceso VACIO
    strcpy(nuevoBloque->nombreProceso,nuevoProceso->nombre);//se le asigna el nombre del proceso al bloque
    //se procede a asignar el bloque nuevo del proceso a la memoria
    if(*base==0 && *limite==15){
        //si es el primer bloque lo asigna correctamente
        nuevoBloque->next=parcial->next;
        U1->first=nuevoBloque;
    }else{
        //si no es el primer registro
        anterior->next=nuevoBloque;
        nuevoBloque->next=parcial->next;
    }
    U1->espacioGlobal-=16;

    free(parcial);
    return nuevoBloque;
}
void registroAleatorio(int *A, int *B, memoria *U1){
    //considera que los registros son de 16
    int x; 
    Bloque *afortunado=U1->first;
    do{
        int aleatorio=1+rand()%67;
        x++;
        while(afortunado->inicioProceso.posicion!=aleatorio){
            afortunado=afortunado->next;
            if(afortunado->inicioProceso.posicion == aleatorio){
                printf("se encontro con exito\n");
            }
        }
        if(strcmp(afortunado->nombreProceso,"Hueco") == 0){
            *A=afortunado->registroBase;
            *B=afortunado->registroLimite;
            break;
        }
    }while(x<=32);
    //si hace mas de 32 habra error de segmentacion
}

Proceso *agregarProceso(Cola *cola,memoria *U1,int Arr[4],int *cont){   //agrega el proceso a la lista de procesos
    if(!U1->espacioGlobal>8){
        printf("Ya no hay memoria, se necesita ejecutar o matar procesos\n");
        return NULL;
    }
    cont+=1;//cuenta los procesos que se crean
    Proceso *nuevo_proceso = (Proceso*)malloc(sizeof(Proceso));
    if(nuevo_proceso==NULL){
        printf("Error al asignar memoria");
        exit(1);
    }
    nuevo_proceso->espacio=Arr[rand()%4];//se le asigna 64 o hasta 512 localidades
    //nuevo_proceso->posicion=*cont; //ya se le asigno la poscicion en memoria
    nuevo_proceso->identificador=rand()%1000;
    nuevo_proceso->instruccion=10 + rand()%(21); //numero de instrucciones dado por el programa entre 10 y 30
    nuevo_proceso->instruccionRestante = nuevo_proceso->instruccion;
    nuevo_proceso->siguiente=NULL;   //se inicializa el puntero siguiente en NULL para que no apunte a ningun otro nodo
    nuevo_proceso->instruccionEjecutadas=0;
    printf("Ingrese el nombre del proceso %d: ",*cont);
    scanf("%29s",nuevo_proceso->nombre); //limitar la entrada a 29 caracteres
    if(!cola->inicio){  
                       //entra al if si es el primer proceso en agregarse
        cola->inicio= nuevo_proceso;
        cola->final = nuevo_proceso;
    }else{                              //entre en else si ya hay un proceso inicial
        cola->final->siguiente=nuevo_proceso;
        cola->final=nuevo_proceso;
    }
    return nuevo_proceso;
}
void asignarPaginas(Bloque *nuevo,Tabla *Principal ,Tabla *T1,int num){
    Pagina *nuevaPagina=(Pagina *)malloc(sizeof(Pagina));
    if(nuevaPagina==NULL){
        printf("Error al crear la pagina\n");
        exit(1);
    }
    strcpy(nuevaPagina->nombreProc,nuevo->nombreProceso);
    nuevaPagina->REGBA=nuevo->registroBase;
    nuevaPagina->REGLIM=nuevo->registroLimite;
    nuevaPagina->numPag=num;
    nuevaPagina->otraPagina=NULL;
    if(!T1->inicio){
        T1->inicio=nuevaPagina;
        T1->final=nuevaPagina;
        if (!Principal->siguienteTabla) {
            Principal->siguienteTabla = T1;
        } else {
            Tabla *siguiente = Principal->siguienteTabla;
            siguiente->siguienteTabla = T1;
        }
    }else{
        T1->final->otraPagina=nuevaPagina;
        T1->final=nuevaPagina;
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
void mostraMemoria(memoria *U1){
    Bloque *temp = U1->first;
    if(!temp){
        printf("La memoria esta vacia\n");
        return;
    }
    printf("\n\t/*/*/*------------%s---------*/*/*/\n",U1->nombreUnidad);
    printf("El espacio de memoria es %d\n\n",U1->espacioGlobal);
    while(temp){
        printf("-----------------Bloque %d-------------------------\n",temp->inicioProceso.posicion);
        printf("Proceso: %s\nPosicion: %d\nID: %d\nEspacio: %d\n",temp->inicioProceso.nombre,temp->inicioProceso.posicion,temp->inicioProceso.identificador,temp->espacio);
        printf("Registro Base = %d && Registro Limite = %d\n\n",temp->registroBase,temp->registroLimite);
        temp=temp->next;
    }
}

void mostraTabla(Tabla *Principal) {
    Tabla *tablaActual = Principal;
    while (tablaActual) {
        Pagina *aux = tablaActual->inicio;
        if (!aux) {
            printf("La tabla base está vacía\n");
        }
        printf("\nTabla: %s\n", tablaActual->nombreCola);
        while (aux) {
            printf("Pagina %d\n Proceso %s\nRegistroBase= %d\nRegistroLimite= %d\n", aux->numPag, aux->nombreProc, aux->REGBA, aux->REGLIM);
            aux = aux->otraPagina;
        }
        tablaActual = tablaActual->siguienteTabla;
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
    Proceso *tempo=cola->inicio;
    int aux; //auxiliar para restar instrucciones
    if (!tempo){ //validacion de una cola vacia
        printf("La cola de %s esta vacia\n",cola->nombre);
    }
    else{
        aux = tempo->instruccionRestante - 5;
        tempo->instruccionRestante = aux; //recuperacion del conteo de instrucciones restantes
        printf("\nProceso %s se ha ejecutado \n", tempo->nombre); //Muestra el nombre del proceso ejecutadp
        printf("Instrucciones restantes: %d\n", tempo->instruccionRestante); //Muestra las instrucciones restantes
        tempo->instruccionEjecutadas+=5;
    }
}

void *procesoSiguiente(Cola *cola, Cola *ES, Cola *Inter,int valor){
    Proceso *aux = cola->inicio; //este es el proceso activo
    Proceso *tmp = Inter->inicio; //este es el proceso de interrupcion que se encuentra al inicio
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
                printf("El proceso Activo se movio a la cola de E/S\n");
                cola->inicio = cola->inicio->siguiente;
                aux->siguiente = NULL;   //desconecta el proceso de la cola

                if(!ES->inicio){
                    ES->inicio=aux;
                    ES->final=aux;
                }else{
                    ES->final->siguiente=aux;  //inserta el proceso al final de la cola
                    ES->final=aux; //se actualiza el puntero final;
                }      
                break;
            case 3:
                printf("El proceso Activo solicito interrupcion\n");
                if(!Inter->inicio && !Inter->final){
                    printf("No hay interrupciones\n");
                }else{
                    Inter->inicio=Inter->inicio->siguiente;
                    tmp->siguiente=NULL; //desconectamos la interrupcion de su cola
                    //agregamos la interrupcion al inicio de la cola
                    tmp->siguiente=cola->inicio;
                    cola->inicio=tmp;
                }
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
void matarProceso(Cola *cola, Cola *tmp, memoria *U1){ //se eliminara el proceso inicial de la cola de procesos y se agregara a la de eliminados
    if (!tmp->inicio){ //validacion de una cola vacia
        printf("La cola de %s esta vacia\n",tmp->nombre);
        return;
    }else{
        Proceso *puntero = tmp->inicio;
        if(tmp->inicio->siguiente==tmp->inicio){
            tmp->inicio=NULL;
            tmp->final=NULL;
            //U1->espacioGlobal+=tmp->inicio->espacio;//ultima modificacion
        }else{
            tmp->inicio=tmp->inicio->siguiente;
            //U1->espacioGlobal+=tmp->inicio->espacio;//ultima modificacion
        }

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
int colaVacia(Cola *cola){
    if(cola->inicio!=NULL && cola->final!=NULL){
        printf("Hay procesos en las colas\n");
        return 1;
    }else{
        printf("La cola de %s esta vacia\n",cola->nombre);
        return 0;
    }
}
void liberar(Cola *cola){
    while(cola->inicio){
    Proceso *proceso_actual=quitarProceso(cola);
    printf("Proceso Ejecutado %s\n",proceso_actual->nombre);
    free(proceso_actual);
    }
}

Bloque *quitarMemoria(memoria *U1){      //quita el proceso que se encuentra al inicio
    if(!U1->first){                 //si entra al if es que ya no hay ningun proceso
        return NULL;
    }
    Bloque *proceso_removido=U1->first;  //se crea un apuntador hacia el inicio de la cola
    U1->first=U1->first->next;   //movemos el puntero de la cola de inicio, al siguiente proceso.
    if(!U1->first){             //entra al if si el proceso al que se movio  el puntero es el ultimo de la cola
        U1->last=NULL;   //se pone en NULL el puntero del final de la cola
    }
    U1->espacioGlobal+=proceso_removido->espacio;
    return proceso_removido;  //retorna el proceso que se quitara osea el que esta al inico de la cola
}
void freememory(memoria *U1){
    while(U1->first){
        Bloque *memoria_actual=quitarMemoria(U1);
        free(memoria_actual);
    }
    printf("Se libero la memoria\n");
}

void simulador(){
    Cola cola, E_S, interrupcion, eliminados, *colaActual;  //se crean varias colas una para los procesos, E/S y las interrupciones
    memoria unidad1; 
    Bloque *nuevo;
    Proceso *auxiliar;
    Tabla Principal;
    crearCola(&cola,"Procesos");  //se inicializa la cola de los procesos
    crearCola(&E_S,"E/S");  //se inicializa la cola E/S
    crearCola(&interrupcion,"Interrupciones"); //se inicializa a cola de interrupciones
    crearCola(&eliminados,"Procesos Eliminados");//se inicializa la cola de eliminados
    crearMemoria(&unidad1,"Unidad 1"); //se inicializa la memoria
    crearTabla(&Principal,"Tabla Principal");
    colaActual=&cola;
    printf("-----------Bienvenido al simulador de Procesos------------\n");
    int y[4] = {64,128,256,512};
    int numeroProc,x,s,z,i,j,k,a,len, Inicio=0, Final=15, cont=0;  //variable donde se guardara el numero de procesos y algunas axiliares
    //inicia el bucle para simular la interfaz de usuario
    agregarHuecos(&unidad1,&Inicio,&Final);
    Inicio=0, Final=15;
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
        printf("11)mostrar lista ligada de procesos y huecos\n");
        printf("12)mostrar tabla de paginas\n");
        printf("13)Salida\n");
        printf("---------------------Ejecucion de la opcion elegida--------------------\nOpcion = ");
        scanf("%d",&x);
        printf("\n");
        switch (x){
        case 0:
            printf("\t------Opcion 0------\n");
            printf("Que proceso desea crear\n1)Proceso X\n2)Entrada/Salida\n3)Interrupcion\nOpcion == ");
            scanf("%d",&a);
            if(a==1){
                printf("\n¿Cuantos procesos desea crear?: ");
                scanf("%d",&numeroProc);
                for(i=1;i<=numeroProc;i++){
                Tabla *TablaProceso=(Tabla*)malloc(sizeof(Tabla));
                auxiliar=agregarProceso(colaActual,&unidad1,y,&cont);
                crearTabla(TablaProceso,auxiliar->nombre);
                if(auxiliar==NULL){
                    printf("No se asigno memoria\n");
                    break;
                    }
                    for(len=1;len<=auxiliar->espacio/16;len++){
                        nuevo=asignarMemoria(auxiliar,&unidad1,&Inicio, &Final);
                        registroAleatorio(&Inicio,&Final,&unidad1);
                        asignarPaginas(nuevo,&Principal,TablaProceso,len);
                    }
                }//se debe continuar para que lo asigne a la memoria
            }else if(a==2 && colaActual==&cola){
                printf("\n¿Cuantos proceso de E/S desea tener?: ");
                scanf("%d",&z);
                for(j=1;j<=z;j++){
                Tabla *TablaProceso=(Tabla*)malloc(sizeof(Tabla));
                auxiliar=agregarProceso(&E_S,&unidad1,y,&cont);
                crearTabla(TablaProceso,auxiliar->nombre);
                if(auxiliar==NULL){
                    printf("No se asigno memoria\n");
                    break;
                    }
                    for(len=1;len<=auxiliar->espacio/16;len++){
                        nuevo=asignarMemoria(auxiliar,&unidad1,&Inicio, &Final);
                        printf("Vizualisador 2\n");
                        registroAleatorio(&Inicio,&Final,&unidad1);
                        asignarPaginas(nuevo,&Principal,TablaProceso,len);
                    }
                }//se debe continuar para que lo asigne a la memoria
            }else if(a==3 && colaActual==&cola){
                printf("\n¿Cuantos proceso de interrupcion desea tener?: ");
                scanf("%d",&s);
                for(k=1;k<=s;k++){
                Tabla *TablaProceso=(Tabla*)malloc(sizeof(Tabla));
                auxiliar=agregarProceso(&interrupcion,&unidad1,y,&cont);
                crearTabla(TablaProceso,auxiliar->nombre);
                if(auxiliar==NULL){
                    printf("No se asigno memoria\n");
                    break;
                    }
                    for(len=1;len<=auxiliar->espacio/16;len++){
                        nuevo=asignarMemoria(auxiliar,&unidad1,&Inicio, &Final);
                        printf("Vizualisador 2\n");
                        registroAleatorio(&Inicio,&Final,&unidad1);
                        asignarPaginas(nuevo,&Principal,TablaProceso,len);
                    }
                }//se debe continuar para que lo asigne a la memoria
            }
            break;
        case 1:
            printf("\t------Opcion 1------\n");
            procesoActivo(&cola);
            break;
        case 2:
            printf("\t------Opcion 2------\n");
            ejecutarProceso(&cola);
            break;
        case 3:
            printf("\t------Opcion 3------\n");
            int y=0+ rand()%(2);
            procesoSiguiente(&cola,&E_S,&interrupcion,3);
            break;
        case 4:
            printf("\t------Opcion 4------\n");
            mostrarCola(&cola);
            mostrarCola2(&E_S);
            mostrarCola2(&interrupcion);
            mostrarCola2(&eliminados);
            break;
        case 5:
            printf("\t------Opcion 5------\n");
            ejecutarProcesoES_IN(&cola,&E_S);
            break;  
        case 6:
            printf("\t------Opcion 6------\n");
            ejecutarProcesoES_IN(&cola,&interrupcion);
            break;
        case 7:
            printf("\t------Opcion 7------\n");
            matarProceso(&eliminados,colaActual,&unidad1);
        case 8:
            printf("\t------Opcion 8------\n");
            mostrarCola2(&cola);
        case 9:
            printf("\t------Opcion 9------\n");
            mostrarCola2(&E_S);
        case 10:
            printf("\t------Opcion 10------\n");
            mostrarCola2(&interrupcion);
            break;
        case 11:
            printf("\t------Opcion 11------\n");
            mostraMemoria(&unidad1);
            break;
        case 12:
            printf("\t------Opcion 12------\n");
            mostraTabla(&Principal);
            break;
        case 13:
            printf("\t------Opcion 13------\n");
            int A ; char *B=(char*)malloc(1*sizeof(char));
            A=colaVacia(&cola);
            if(A==1){
                printf("Desea eliminar todos los procesos que quedan? \nSi(Y), No(N) = ");
                scanf("%s",B);
                if(B[0]=='Y' || B[0]=='S'){
                    liberar(&cola);
                    liberar(&E_S);
                    liberar(&interrupcion);
                    liberar(&eliminados);
                    freememory(&unidad1);
                    free(B);
                    x=14;
                }
            }else{ //sino repite el ciclo para agregar datos si es que tiene procesos aun
                if(colaActual->final==NULL || colaActual->inicio==NULL){ //si la cola de procesos esta vacia realmente se sale
                    liberar(&cola);
                    liberar(&E_S);
                    liberar(&interrupcion);
                    liberar(&eliminados);
                    freememory(&unidad1);
                    free(B);
                    x=14;
                }
            }
            break;
        default:
            printf("\n****No es una opcion del menu ingrese una valida o 13 para salir*****\n");
            x=13;
            break;
        }
    }while(x!= 14);
    printf("Hasta pronto :)\n");
}

void main(){
    srand(time(NULL));
    simulador();
    printf("simulador terminado\n");
}
                                                                                                                                            