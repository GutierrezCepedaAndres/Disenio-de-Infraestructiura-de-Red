#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define MAX_DATOS 1024
#define NORTE 0
#define SUR 1
#define ESTE 2
#define OESTE 3
#define L 4

int* vecinosToroide(int nodo);
int contadorDatos();

int main(int argc, char *argv[]){
    int i=0;
    int* vecinoToroide;
    double numero = 0;
    double numeroNuevo = 0;
    double valores[L*L];
    char* char_fichero;
    double valor;
    char linea[80];
    char *token;
    const char separador[2] = ",";
    FILE *archivo;
    int datos;
    int continuar=0;

    MPI_Init(&argc,&argv); 
    MPI_Status status;
    int size,rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);


    if(rank==0){
        datos=contadorDatos();

        if(datos>size){
            continuar=1;
            printf("El numero de nodos a ejecutar es menor que el numero de datos\r\n");
            MPI_Bcast(&continuar,1,MPI_INT,0,MPI_COMM_WORLD);
        }else{
            if(L*L==size){
                continuar=0;
                printf("El numero de nodos a ejecutar correcto\r\n");
                MPI_Bcast(&continuar,1,MPI_INT,0,MPI_COMM_WORLD);
            }else{
                continuar=1;
                printf("El numero de nodos ejecutados es distinto a los nodos requeridos\r\n");
                MPI_Bcast(&continuar,1,MPI_INT,0,MPI_COMM_WORLD);
            }
            
        }

        if(continuar==0){
        
        if((archivo=fopen("./dirs/datos.dat","r"))==NULL){
            printf("Error al abrir el fichero\n");
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }

        char_fichero=fgets(linea,MAX_DATOS*sizeof(char),archivo);

        token = strtok(linea, separador);

        while( token != NULL ) { 
            valores[i++]=atof(token);
            token = strtok(NULL, separador);
        }

        fclose(archivo);

        for(i=1;i<(L*L);i++){
            numero=valores[i];
            MPI_Bsend(&numero,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
        }
        numero=valores[0];
        vecinoToroide=vecinosToroide(rank);
        for (i=0;i<L;i++){
            MPI_Bsend(&numero,1,MPI_DOUBLE,vecinoToroide[NORTE],0,MPI_COMM_WORLD);
            MPI_Recv(&numeroNuevo,1,MPI_DOUBLE,vecinoToroide[SUR],MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            if(numeroNuevo<numero){
                numero=numeroNuevo;
            }
        }
        for (i=0;i<L;i++){
            MPI_Bsend(&numero,1,MPI_DOUBLE,vecinoToroide[ESTE],0,MPI_COMM_WORLD);
            MPI_Recv(&numeroNuevo,1,MPI_DOUBLE,vecinoToroide[OESTE],MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            if(numeroNuevo<numero){
                numero=numeroNuevo;
            }
        }
        printf("Numero minimo %lf\n",numero);
        }
    }else{

        MPI_Bcast(&continuar,1,MPI_INT,0,MPI_COMM_WORLD);
        if(continuar==0){

        MPI_Recv(&numero,1,MPI_DOUBLE,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        vecinoToroide=vecinosToroide(rank);
        for (i=0;i<L;i++){
            MPI_Bsend(&numero,1,MPI_DOUBLE,vecinoToroide[NORTE],0,MPI_COMM_WORLD);
            MPI_Recv(&numeroNuevo,1,MPI_DOUBLE,vecinoToroide[SUR],MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            if(numeroNuevo<numero){
                numero=numeroNuevo;
            }
        }
        for (i=0;i<L;i++){
            MPI_Bsend(&numero,1,MPI_DOUBLE,vecinoToroide[ESTE],0,MPI_COMM_WORLD);
            MPI_Recv(&numeroNuevo,1,MPI_DOUBLE,vecinoToroide[OESTE],MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            if(numeroNuevo<numero){
                numero=numeroNuevo;
            }
        }
        }
    }

    MPI_Finalize();
   
   return EXIT_SUCCESS;
}

int* vecinosToroide(int nodo){

    static int vecinos [L];

    int modulo = nodo%L;
    int division  = nodo/L;

    switch(modulo){
        case (L-1):
            vecinos[ESTE]=nodo-1;
            vecinos[OESTE]=nodo-(L-1);
        break;
        case 0:
            vecinos[ESTE]=nodo+(L-1);
            vecinos[OESTE]=nodo+1;
        break;
        default:
            vecinos[ESTE]=nodo-1;
            vecinos[OESTE]=nodo+1;
        break;
    }

    switch(division){
        case (L-1):
            vecinos[SUR]=nodo-L;
            vecinos[NORTE]=nodo-(L*(L-1));
        break;
        case 0:
            vecinos[SUR]=nodo+(L*(L-1));
            vecinos[NORTE]=nodo+L;
        break;
        default:
            vecinos[SUR]=nodo-L;
            vecinos[NORTE]=nodo+L;
        break;
    }

    return vecinos;
}

int contadorDatos(){
    int datos=0;

    char* char_fichero;
    char linea[80];
    char *token;
    const char separador[2] = ",";
    FILE *archivo;
    
    if((archivo=fopen("./dirs/datos.dat","r"))==NULL){
            printf("Error al abrir el fichero\n");
            exit(EXIT_FAILURE);
        }

        char_fichero=fgets(linea,MAX_DATOS*sizeof(char),archivo);

        token = strtok(linea, separador);

        while( token != NULL ) { 
            datos++;
            token = strtok(NULL, separador);
        }

        fclose(archivo);

        return datos;
}