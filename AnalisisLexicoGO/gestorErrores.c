#include <stdio.h>
#include <stdlib.h>

void errorAbrirArchivo(char* nombre){
    printf(">> ERROR: Ha ocurrido un error al intentar abrir el archivo: %s", nombre);
}

void errorNumMal(char* lexema){
    printf("ERROR: El numero %s no esta bien formado\n", lexema);
}

void errorComillas(char* lexema){
    printf("ERROR: Existe un grupo de comillas no cerrado en el codigo\n");
    printf("Concretamente > %s\n", lexema);
}

void errorComentario(char *lexema){
    printf("El codigo contiene un comentario sin cerrar.\n");
    printf("Concretamente > %s\n", lexema);
}