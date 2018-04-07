#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tablaSimbolos.h"
#include "sistemaEntrada.h"
#include "definiciones.h"
#include "gestorErrores.h"

#define TAM 8

tabla nodo; //Resultado que se devuelve al analizador sintactico
tipoelem lexemaActual; //Donde se almacena el lexema analizado
char caracterActual, cAnterior;
/*cAnterior se utliza para cuando tienes que volver atras en la lectura
 del fichero, porque tal y como esta implementado el sistema de entrada no se 
 puede, por ello uso esta variable para guardar algun caracter que no deberia
 ser ignorado*/
int tamActual, codigo;
/*tamActual es el tamaño maximo de lexema parcial, cuando se realiza realloc
 codigo es para los numeros e identificadores, para distinguirlos de las palabras
 reservadas*/

void reservaLexema(){
    lexemaActual = realloc(lexemaActual, strlen(lexemaActual)+TAM); //Se realoja la memoria
    
    tamActual += TAM; //Se actualiza el tamaño actual del buffer
}

void alfaNum(){    
    do{
        if(strlen(lexemaActual) >= tamActual){ //Si se acaba el espacio
            reservaLexema();
        }else{
            lexemaActual[strlen(lexemaActual)] = caracterActual;
            caracterActual = siguienteCaracter();
        }
    }while(isalnum(caracterActual));    //Mientras sea alfanumerico
    
    if(ispunct(caracterActual) || caracterActual == '\n'){ //Por si hay un simbolo que no es un espacio
        cAnterior = caracterActual;
    }
}

void string(){
    do{
        if(strlen(lexemaActual) >= tamActual){
            reservaLexema();
        }else{
            lexemaActual[strlen(lexemaActual)] = caracterActual;
            cAnterior = caracterActual;
            caracterActual = siguienteCaracter();
            if(caracterActual == EOF){ //Si no se cierran las comillas
                errorComillas(lexemaActual);
                break;
            }
        }
    }while(!(caracterActual == '"' && cAnterior != '\\'));
    //se detiene cuando el caracter actual son unas comillas y no estaban escapadas
    
    cAnterior = '\0';
    if(strlen(lexemaActual) >= tamActual)
        reservaLexema();
    
    lexemaActual[strlen(lexemaActual)] = caracterActual; //Mete las comillas finales
}

void comentarioUnaLinea(){ //Comentarios de una unica linea
    
    do{
        if(strlen(lexemaActual) >= tamActual){
            reservaLexema();
        }else{
            lexemaActual[strlen(lexemaActual)] = caracterActual;
            caracterActual = siguienteCaracter();
        }
    }while(caracterActual != '\n'); //Se finaliza al realizar un salto de linea
    
    cAnterior = caracterActual;
}

void comentarioMulti(){ //Funcion que reconoce comentarios multilinea
    do{
        if(strlen(lexemaActual) >= tamActual){
            reservaLexema();
        }else{
            lexemaActual[strlen(lexemaActual)] = caracterActual;
            cAnterior = caracterActual;
            caracterActual = siguienteCaracter();
            if(caracterActual == EOF){ //Por si no se cierra el comentario
                errorComentario(lexemaActual);
                break;
            }
        }
    }while(!(caracterActual == '/' && cAnterior == '*'));
    //Se finaliza cuando el se encuentre un slash precedido de un asterisco
    cAnterior = '\0';
    if(strlen(lexemaActual) >= tamActual)
        reservaLexema();
    lexemaActual[strlen(lexemaActual)] = caracterActual;
}

void exponencial(){
    do{
        if(strlen(lexemaActual) >= tamActual){
            reservaLexema();
        }else{
            lexemaActual[strlen(lexemaActual)] = caracterActual;
            caracterActual = siguienteCaracter();
        }
    }while(isdigit(caracterActual));
    
    if(caracterActual == '-' || caracterActual == '+'){ //Comprueba si despues de la 'e' viene un signo + o -
        if(lexemaActual[strlen(lexemaActual) -1] == 'e' || lexemaActual[strlen(lexemaActual) -1] == 'E'){
            do{ //Se sigue reconociendo el numero exponencial
                if(strlen(lexemaActual) >= tamActual){
                    reservaLexema();
                }else{
                    lexemaActual[strlen(lexemaActual)] = caracterActual;
                    caracterActual = siguienteCaracter();
                }
            }while(isdigit(caracterActual));
            if(ispunct(caracterActual))
                cAnterior = caracterActual;
            else if(caracterActual == 'i'){
                lexemaActual[strlen(lexemaActual)] = caracterActual;
                codigo=IMAGINARIO;
            }
        }else
            cAnterior = caracterActual;
    }else if(ispunct(caracterActual)){
        cAnterior = caracterActual;
    }else if(caracterActual == 'i'){ //si es imaginario
        lexemaActual[strlen(lexemaActual)] = caracterActual;
        codigo=IMAGINARIO;
    }
}

void flotante(){
    codigo = FLOAT32;
    do{
        if(strlen(lexemaActual) >= tamActual){
            reservaLexema();
        }else{
            lexemaActual[strlen(lexemaActual)] = caracterActual;

            caracterActual = siguienteCaracter();
        }
    }while(isdigit(caracterActual));
    if(caracterActual == 'e' || caracterActual == 'E'){
        exponencial();
    }else if(ispunct(caracterActual)){
        cAnterior = caracterActual;
    }else if(caracterActual == 'i'){ //si es imaginario
        lexemaActual[strlen(lexemaActual)] = caracterActual;
        codigo=IMAGINARIO;
    }
}

void BadFace(){
    char badFace[]= "0xBadFace42";
    int i;
    lexemaActual[1] = caracterActual;
    //Comprueba caracter a caracter que sea identico
    for(i=2; i < strlen(badFace); i++){
        lexemaActual[i] = siguienteCaracter();
        if(lexemaActual[i] != badFace[i]){
            errorNumMal(lexemaActual);
            break; //NUMERO MAL FORMADO
        }
    }
    caracterActual = siguienteCaracter();
    
    if(ispunct(caracterActual)){
        cAnterior = caracterActual;
    }else if(!isspace(caracterActual)){
        errorNumMal(lexemaActual);
    }
}

void integers(){
    codigo = INT;
    do{
        if(strlen(lexemaActual) >= tamActual){
            reservaLexema();
        }else{
            lexemaActual[strlen(lexemaActual)] = caracterActual;
            caracterActual = siguienteCaracter();
        }
    }while(isdigit(caracterActual));
    
    if(caracterActual == '.')
        flotante();
    else if(caracterActual == 'e' || caracterActual == 'E')
        exponencial();
    else if(ispunct(caracterActual))
        cAnterior = caracterActual;
    else if(caracterActual == 'x' && strlen(lexemaActual) == 1 && lexemaActual[0] == '0')
        BadFace();
    else if(caracterActual == 'i'){
        lexemaActual[strlen(lexemaActual)] = caracterActual;
        codigo=IMAGINARIO;
    }
}

tabla nextCompLex(tabla *T){
    int code;
    if(nodo != NULL){
        free(nodo);
        nodo=NULL;
    }
    lexemaActual = (char*) malloc (TAM*sizeof(char));
    free(lexemaActual);
    lexemaActual = (char*) malloc (TAM*sizeof(char));
    
    if(cAnterior != '\0'){ //Esto es por si se debe ir hacia atras en los caracteres leidos
        caracterActual = cAnterior;
        cAnterior = '\0';
    }else
        caracterActual = siguienteCaracter();
    
    tamActual=TAM;

    if(isalpha(caracterActual) || caracterActual == '_'){ //Cadena alfanumerica
        alfaNum();
        code = busca(T, lexemaActual);
        creaNodo(&nodo, lexemaActual, code);
        free(lexemaActual);
        return nodo;
    }else if(isspace(caracterActual)){ //Espacio, tabulacion o salto de linea
        if(caracterActual == '\n'){
            creaNodo(&nodo, lexemaActual, SALTO_LINEA);
            free(lexemaActual);
            return nodo;
        }else
            creaNodo(&nodo, lexemaActual, SPACE);
            free(lexemaActual);
            return nodo;
    }else if(caracterActual == '"'){ //Inicio de un String
        string();
        creaNodo(&nodo, lexemaActual, STRING);
        return nodo;
    }else if(ispunct(caracterActual)){ //Signos de puntuacion u operadores
        lexemaActual[strlen(lexemaActual)] = caracterActual;
        switch(caracterActual){
            case '<':
                caracterActual=siguienteCaracter();
                if(caracterActual == '='){
                    lexemaActual[1] = caracterActual;
                    creaNodo(&nodo, lexemaActual, MENORIGUAL);
                    return nodo;
                }
                else if( caracterActual == '-'){
                    lexemaActual[1] = caracterActual;
                    creaNodo(&nodo, lexemaActual, ASIGNACION);
                    return nodo;
                }
                else
                    cAnterior = caracterActual;
                break;
            case '>':
                caracterActual=siguienteCaracter();
                if(caracterActual == '='){
                    lexemaActual[1] = caracterActual;
                    creaNodo(&nodo, lexemaActual, MAYORIGUAL);
                    return nodo;
                }
                else
                    cAnterior = caracterActual;
                break;
            case '=':
                caracterActual = siguienteCaracter();
                if(caracterActual == '='){
                    lexemaActual[1] = caracterActual;
                    creaNodo(&nodo, lexemaActual, IGUALIGUAL);
                    return nodo;
                }else
                    cAnterior = caracterActual;
                break;
            case '.':
                caracterActual = siguienteCaracter();
                if(isdigit(caracterActual)){
                    flotante();
                    creaNodo(&nodo, lexemaActual, FLOAT32);
                    free(lexemaActual);
                    return nodo;
                }else
                    cAnterior = caracterActual;
                break;
            case ':':
                caracterActual = siguienteCaracter();
                if(caracterActual == '='){
                    lexemaActual[1] = caracterActual;
                    creaNodo(&nodo, lexemaActual, DOSPUNTOSIGUAL);
                    return nodo;
                }else
                    cAnterior = caracterActual;
                break;
            case '/':
                caracterActual = siguienteCaracter();
                if(caracterActual == '/'){
                    comentarioUnaLinea();
                    creaNodo(&nodo, lexemaActual, COMENTARIO);
                    free(lexemaActual);
                    return nodo;
                }else if(caracterActual == '*'){
                    comentarioMulti();
                    creaNodo(&nodo, lexemaActual, COMENTARIO);
                    free(lexemaActual);
                    return nodo;
                }else
                    cAnterior = caracterActual;
                break;
            case '+':
                caracterActual = siguienteCaracter();
                if(caracterActual == '='){
                    lexemaActual[1] = caracterActual;
                    creaNodo(&nodo, lexemaActual, MASIGUAL);
                    free(lexemaActual);
                    return nodo;
                }else if(caracterActual == '+'){
                    lexemaActual[1] = caracterActual;
                    creaNodo(&nodo, lexemaActual, MASMAS);
                    free(lexemaActual);
                    return nodo;
                }else
                    cAnterior = caracterActual;
                break;
            case '-':
                caracterActual = siguienteCaracter();
                if(isdigit(caracterActual)){
                    integers();
                    creaNodo(&nodo, lexemaActual, codigo);
                    free(lexemaActual);
                    return nodo;
                }
                break;
        }
        creaNodo(&nodo, lexemaActual, caracterActual);
        return nodo;
    }
    else if(isdigit(caracterActual)){ //Numeros
        integers();
        creaNodo(&nodo, lexemaActual, codigo);
        codigo = 0;
        return nodo;
    }
    
    
}