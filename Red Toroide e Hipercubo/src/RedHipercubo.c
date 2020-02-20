#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define MAX_DATOS 1024
#define D 3

int* vecinosHipercubo(int nodo);

int contadorDatos();

int main(int argc, char *argv[]){
    int i=0;
    int* vecinoHipercubo;
    double numero = 0;
    double numeroNuevo = 0;
    int tamanoVector=pow(2,D);
    double valores[tamanoVector];
    char* char_fichero;
    double valor;
    char linea[80];
    char *token;
    const char separador[2] = ",";
    FILE *archivo;
    int indice_char=0;
    MPI_Status status;
    int size,rank;
    int datos;
    int continuar=0;

    MPI_Init(&argc,&argv); 
   
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(rank==0){
         datos=contadorDatos();

        if(datos>size){
            continuar=1;
            printf("El numero de nodos a ejecutar es menor que el numero de datos\r\n");
            MPI_Bcast(&continuar,1,MPI_INT,0,MPI_COMM_WORLD);
        }else{
            if(pow(2,D)==size){
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
            exit(EXIT_FAILURE);
        }

        char_fichero=fgets(linea,MAX_DATOS*sizeof(char),archivo);

        token = strtok(linea, separador);

        while( token != NULL ) { 
            valores[i++]=atof(token);
            token = strtok(NULL, separador);
        }

        fclose(archivo);

        for(i=1;i<tamanoVector;i++){
            numero=valores[i];
            MPI_Bsend(&numero,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
        }
        numero=valores[0];
        
        vecinoHipercubo=vecinosHipercubo(rank);
        for (i=0;i<D;i++){
            MPI_Bsend(&numero,1,MPI_DOUBLE,vecinoHipercubo[i],0,MPI_COMM_WORLD);
            MPI_Recv(&numeroNuevo,1,MPI_DOUBLE,vecinoHipercubo[i],MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            if(numeroNuevo>numero){
                numero=numeroNuevo;
            }
        }
        printf("Numero maximo %lf\n",numero);
        
        }
    }else{
        MPI_Bcast(&continuar,1,MPI_INT,0,MPI_COMM_WORLD);
        if(continuar==0){

        MPI_Recv(&numero,1,MPI_DOUBLE,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        
        vecinoHipercubo=vecinosHipercubo(rank);
        for (i=0;i<D;i++){
            MPI_Bsend(&numero,1,MPI_DOUBLE,vecinoHipercubo[i],0,MPI_COMM_WORLD);
            MPI_Recv(&numeroNuevo,1,MPI_DOUBLE,vecinoHipercubo[i],MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            if(numeroNuevo>numero){
                numero=numeroNuevo;
            }
        }
        
        }
    }

    MPI_Finalize();
   
   return EXIT_SUCCESS;
}

int* vecinosHipercubo(int nodo){
    static int vecinos [D];
    int i=0;
    int desplazamiento=1;

    for (i=0;i<D;i++){
        vecinos[i]=nodo^desplazamiento;
        desplazamiento=desplazamiento<<1;
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