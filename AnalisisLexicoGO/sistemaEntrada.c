#include <stdio.h>
#include <stdlib.h>
#include "gestorErrores.h"

#define TAM 20

//Definicion de variables
FILE *f;
char buf1[TAM+1], buf2[TAM+1];
int numBufer, contador;

void cargaBufer(){
    int i;
    //Funcion para el cargado de los buffers con el contenido del fichero
    for(i=0; i < TAM ; i++) buf1[i] = fgetc(f);
    buf1[TAM]= EOF;
    
    for(i=0; i < TAM ; i++) buf2[i] = fgetc(f);
    buf2[TAM]= EOF;
}

int abrirFichero(){
    f = fopen("concurrentSum.go", "r");
    if(f == NULL){
        errorAbrirArchivo("concurrentSum.go");
        return 1;
    }
    
    cargaBufer();
    
    numBufer=1;
    contador=0;
    
    return 0;
}

char siguienteCaracter(){
    char resultado;
    if(f==NULL){
        if(abrirFichero())
            resultado = EOF;
    }
    switch(numBufer){ //Este switch controla de que buffer coger los datos
        case 1:
            resultado = buf1[contador];
            if(resultado == EOF){
                if(contador != TAM){
                    resultado=EOF;
                }
                else{
                    if(buf2[0] == EOF){
                        resultado = EOF;
                    }else{
                        resultado = buf2[0];
                        numBufer=2;
                        contador=1;
                    }
                }
            }else
                contador++;
            break;
        
        case 2:
            resultado = buf2[contador];
            if(resultado == EOF){
                if(contador != TAM){
                    resultado = EOF;
                }else{
                    cargaBufer();
                    if(buf1[0] == EOF){
                        resultado = EOF;
                    }else{
                        resultado = buf1[0];
                        numBufer=1;
                        contador=1;
                    }
                }
            }else
                contador++;
        break;
    }
    
    return resultado;
}

