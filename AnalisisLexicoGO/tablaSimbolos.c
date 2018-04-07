#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"

////////////////////////////////////////////////////////////
typedef char* tipoelem;
typedef char  *tipoclave;
////////////////////////////////////////////////////////////

struct celda {
    tipoelem lexema;
    int codigo;
    struct celda *izq, *der;
};

typedef struct celda *tabla;

void destruye(tabla *A) {
    if (*A != NULL) {
        destruye(&(*A)->izq);
        destruye(&(*A)->der);
        free(*A);
        *A = NULL;
    }
}

unsigned esVacio(tabla A) {
    return A == NULL;
}

void inserta(tabla *A, tipoelem E, int code) {
    if(esVacio(*A)){
        *A = (tabla) malloc(sizeof (struct celda));
        (*A)->lexema = (char*) malloc(strlen(E)*sizeof(char));
        free((*A)->lexema);
        (*A)->lexema = (char*) malloc(strlen(E)*sizeof(char));
        strncpy((*A)->lexema, E, strlen(E));
        (*A)->codigo = code;
        (*A)->izq = NULL;
        (*A)->der = NULL;
    }
}

int busca(tabla *A, tipoclave nombre) {
    if (esVacio(*A)){
        inserta(A, nombre, ID);
        return ID;
    }else if (strcmp(nombre, (*A)->lexema) < 0)
        busca(&(*A)->izq, nombre);
    else if (strcmp(nombre, (*A)->lexema) == 0)
        return (*A)->codigo;
    else
        busca(&(*A)->der, nombre);
}

void insertaPalabraReservada(tabla *A, tipoelem nombre, int code){
    if (esVacio(*A)){
        inserta(A, nombre, code);    
    }else if (strcmp(nombre, (*A)->lexema) <  0)
        insertaPalabraReservada(&(*A)->izq, nombre, code);     
    else if(strcmp(nombre, (*A)->lexema) > 0)
        insertaPalabraReservada(&(*A)->der, nombre, code);
        
    
}

void crea(tabla *A) {
    FILE *palabras;
    char palabraR[20];
    char auxCode[5];
    int codigo;
    *A = NULL;
    palabras = fopen("palabrasReservadas", "r");
    //Se obtienen las palabras reservadas de un fichero de texto
    while(fgets(palabraR, 20,palabras) != NULL){
        palabraR[strlen(palabraR) -1] = '\0';
        fgets(auxCode,5,palabras);
        codigo = atoi(auxCode);
        
        insertaPalabraReservada(A, palabraR, codigo);
    }

    fclose(palabras);
}

void imprimir(tabla A){ 
    if (!esVacio(A)) { 
       imprimir(A->izq);
       printf("< %s , %d> \n", A->lexema, A->codigo);
       imprimir(A->der);
    }
}

tipoelem getLexema(tabla A){
    return A->lexema;
}

int getCodigo(tabla A){
    return A->codigo;
}

void creaNodo(tabla *A, tipoelem nombre, int codigo){
    //Esta funcion es para crear un nodo de resultado
    if(esVacio(*A)){
        *A = (tabla) malloc(sizeof (struct celda));
        size_t tam = strlen(nombre);
        (*A)->lexema = (char*) malloc(tam*sizeof(char));
        free((*A)->lexema);
        (*A)->lexema = (char*) malloc(tam*sizeof(char));
        
        strncpy((*A)->lexema, nombre, strlen(nombre));

        (*A)->codigo = codigo;
        (*A)->izq = NULL;
        (*A)->der = NULL;
    }
}