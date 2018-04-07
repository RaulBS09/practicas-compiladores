#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"
#include "tablaSimbolos.h"
#include "analizadorLexico.h"

void iniciarAnalisis(){
    tabla T;
    tabla resultado;
    
    crea(&T);
    printf("--Comienzo del analisis--\n");
    while((resultado=nextCompLex(&T)) != NULL){
        int codigo = getCodigo(resultado); //Obtenemos el codigo del componente lexico
        if(codigo != SPACE && codigo != COMENTARIO){ //Si es un espacio o un comentario, no se imprime
            size_t tam = strlen(getLexema(resultado)); //Tamaño del lexema
            char *lexema = (char*)malloc(tam*sizeof(char));
            free(lexema); //Hago esto porque si no lo hacia asi imprimia cosas raras
            lexema = (char*)malloc(tam*sizeof(char));
            
            strncpy(lexema, getLexema(resultado), tam); //Copiado de la cadena

            printf("< %s, %d>\n", lexema, codigo); //Imprimimos
        }
    }
    printf("\n----------Impresion de la tabla de simbolos--------\n");
    imprimir(T); //Se muestra la tabla de simbolos
    
    destruye(&T); //Se destruye la tabla de simbolos
}

