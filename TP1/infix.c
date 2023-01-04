#include "calc_helper.h"
#include "pila.h"
#include "cola.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INDICE_INICIAL 0
#define CANTIDAD_INICIAL 0
#define PESO_INICIAL 0
#define CANTIDAD_CHAR_INICIAL 1
#define LONGITUD_CADENA_OPERANDOR 2
#define LONGITUD_DE_ALERTA_ERROR 7
#define LONGITUD_CADENA_POR_DEFECTO 50

/* 
ORDENES DE PRECEDENCIA:
1: + -
2: / *
3: ^ 
*/


//Función auxliar. Dada una cola, limpia tanto los nodos internos, como los tokens que están dentro de los mismos.
void limpiar_cola_invalida(cola_t *cola_respuesta) {
    while (!cola_esta_vacia(cola_respuesta)) {
        struct calc_token *token = (struct calc_token*)cola_desencolar(cola_respuesta);
        free(token);
    }
}

//Imprime mediante la salida estándar, la expresión equivalente de la operación matemática dada
//en notación infija a notación posfija. O un mensaje de "ERROR", si la operación es inválida.
void formar_cadena_convertida(cola_t *cola_convertida, size_t carateres_invalidos) {
    if (carateres_invalidos) {
        fprintf(stdout, "ERROR\n");
        return;
    }
    if (cola_esta_vacia(cola_convertida)) {
        fprintf(stdout, "\n");
        return;
    }

    char *cadena_stdout = calloc(CANTIDAD_CHAR_INICIAL, sizeof(char));
    char error_stdout[LONGITUD_DE_ALERTA_ERROR] = "ERROR\n";
    bool error = false;
    size_t longitud_cadena = sizeof(char);
    
    while (!cola_esta_vacia(cola_convertida)) {
        struct calc_token *token = (struct calc_token*)cola_desencolar(cola_convertida);
        if (token->type == TOK_NUM && !error) {
            longitud_cadena += LONGITUD_CADENA_POR_DEFECTO;
            cadena_stdout = realloc(cadena_stdout, longitud_cadena);
            char numero[LONGITUD_CADENA_POR_DEFECTO];
            sprintf(numero, "%ld ", token->value);
            strcat(cadena_stdout, numero);
        } else if (token->type == TOK_OPER && !error){
            longitud_cadena += LONGITUD_CADENA_OPERANDOR;
            cadena_stdout = realloc(cadena_stdout, longitud_cadena);
            switch (token->oper.op) {
                case OP_ADD:
                    strcat(cadena_stdout, "+ ");
                    break;
                case OP_SUB:
                    strcat(cadena_stdout, "- ");
                    break;
                case OP_MUL:
                    strcat(cadena_stdout, "* ");
                    break;
                case OP_DIV:
                    strcat(cadena_stdout, "/ ");
                    break;
                default: //OP_POW
                    strcat(cadena_stdout, "^ ");
                    break;
            }
        } else { //TOK_LPAREN
            error = true;
        }
        free(token);
    }
    strcat(cadena_stdout, "\n");
    (error) ? fprintf(stdout, "%s", error_stdout) : fprintf(stdout, "%s",cadena_stdout);
    free(cadena_stdout);
}

//Función auxilar para modularizar la función "apilar_con_orden". Realiza el trabajo interno del ciclo "while" para mantener
//el orden de precendencia correcto en la pila, además de agregar a la cola los tokens desapilados para mantener el orden correcto en la respuesta. 
void ordenar_cola_con_orden(pila_t* pila_operadores, cola_t* cola_respuesta) {
    struct calc_token *token_aux_switch = (struct calc_token*)pila_desapilar(pila_operadores);
    cola_encolar(cola_respuesta, token_aux_switch);
}

//Función para modularizar el bloque de condicionales "switch" usando el orden de precedencia de la suma y resta.
void apilar_suma_resta(struct calc_token *token_aux, struct calc_token *token, cola_t *cola_tokens, pila_t *pila_operadores) {
    while (token_aux && token_aux->type != TOK_LPAREN) {
        ordenar_cola_con_orden(pila_operadores, cola_tokens);
        token_aux = (struct calc_token*)pila_ver_tope(pila_operadores);
    }
    pila_apilar(pila_operadores, token);
}

//Función para modularizar el bloque de condicionales "switch" usando el orden de precedencia de la multiplicación y división.
void apilar_multiplicacion_division(struct calc_token *token_aux, struct calc_token *token, cola_t *cola_tokens, pila_t *pila_operadores) {
    while (token_aux && token_aux->type != TOK_LPAREN && (token_aux->oper.op == OP_POW || token_aux->oper.op == OP_MUL || token_aux->oper.op == OP_DIV)) {
        ordenar_cola_con_orden(pila_operadores, cola_tokens);
        token_aux = (struct calc_token*)pila_ver_tope(pila_operadores);
    }
    pila_apilar(pila_operadores, token);
}

//Se encarga de apilar el token dado manteniendo el orden de precedencia y organizando el resultado. Funciona utilizando la cola y pila
//del programa junto con el token actual y uno auxiliar, que es el tope de la pila de operadores.
void apilar_con_orden(struct calc_token *token_aux, struct calc_token *token, cola_t *cola_tokens, pila_t *pila_operadores) {
    switch (token->oper.op) {
        case OP_SUB:
            apilar_suma_resta(token_aux, token, cola_tokens, pila_operadores);
            break;
        case OP_ADD:
            apilar_suma_resta(token_aux, token, cola_tokens, pila_operadores);
            break;
        case OP_DIV:
            apilar_multiplicacion_division(token_aux, token, cola_tokens, pila_operadores);
            break;
        case OP_MUL:
            apilar_multiplicacion_division(token_aux, token, cola_tokens, pila_operadores);
            break;
        default: //OP_POW
            pila_apilar(pila_operadores, token);
            break;
    }
}

//Realiza el manejo de la cadena del archivo dado que contiene la expresión a convertir.
void conversor_infijo_a_posfijo(char *linea) {
    cola_t *cola_tokens = cola_crear();
    pila_t *pila_operadores = pila_crear();

    size_t i = INDICE_INICIAL,
    contador_caracter_invalido = CANTIDAD_INICIAL;
    char **contenido_de_la_linea = infix_split(linea);

    while (contenido_de_la_linea[i]) {
        struct calc_token *token = malloc(sizeof(struct calc_token)),
            *token_aux = NULL;
        bool caracter_valido = calc_parse(contenido_de_la_linea[i], token),
            iterar;
        if (!caracter_valido) {
            contador_caracter_invalido++;
            free(token);
            free(contenido_de_la_linea[i]);
            i++;;
            continue;
        }
        switch (token->type) {
            case TOK_NUM:
                cola_encolar(cola_tokens, token);
                break;
            case TOK_LPAREN:
                pila_apilar(pila_operadores, token);
                break;
            case TOK_RPAREN:
                iterar = true;
                free(token);
                while (!pila_esta_vacia(pila_operadores) && iterar) {
                    token_aux = (struct calc_token*)pila_desapilar(pila_operadores);
                    if (token_aux->type == TOK_LPAREN) {
                        free(token_aux);
                        iterar = false;
                    } else { cola_encolar(cola_tokens, token_aux); }
                }
                if (iterar) { contador_caracter_invalido++; }
                break;
            default: //TOK_OPER
                token_aux = (struct calc_token*)pila_ver_tope(pila_operadores);
                if (token_aux) {
                    apilar_con_orden(token_aux, token, cola_tokens, pila_operadores);
                } else { pila_apilar(pila_operadores, token); }
                break;
        }
        free(contenido_de_la_linea[i]);
        i++;
    }
    while(!pila_esta_vacia(pila_operadores)) {
        cola_encolar(cola_tokens, pila_desapilar(pila_operadores));
    }
    if (contador_caracter_invalido) { limpiar_cola_invalida(cola_tokens); }
    formar_cadena_convertida(cola_tokens, contador_caracter_invalido);
    free(contenido_de_la_linea);
    cola_destruir(cola_tokens, NULL);
    pila_destruir(pila_operadores); 
}

//Esta función revisa si la primera líenea tiene o no salto de línea.
bool unica_linea_sin_salto(FILE *archivo, char **linea, size_t *capacidad, size_t *leidos) {
    size_t peso_archivo = PESO_INICIAL;
    if (archivo) {
        fseek(archivo, *capacidad, SEEK_END);
        peso_archivo = ftell(archivo);
        rewind(archivo);
    }
    *leidos = (peso_archivo) ? getline(linea, capacidad, archivo) : CANTIDAD_INICIAL;
    if (!(*leidos)) { return false; }
    return true;
}

int main(void) {
    FILE *archivo = stdin;
    char *linea = NULL;
    size_t capacidad = CANTIDAD_INICIAL, leidos = CANTIDAD_INICIAL;
    bool linea_sin_salto = unica_linea_sin_salto(archivo, &linea, &capacidad, &leidos);
    if (!linea_sin_salto) { return 0; }

    while(!feof(archivo)) {
        size_t leidos_aux = leidos;
        conversor_infijo_a_posfijo(linea);
        leidos = getline(&linea, &capacidad, archivo);
        linea_sin_salto = (strlen(linea) != (leidos_aux) && feof(archivo)) ? true : false;
    }

    if (linea_sin_salto) {
        if (!strlen(linea)) { return 0; } //Línea sin salto de línea vacía.
        conversor_infijo_a_posfijo(linea);
    }
    free(linea);
    fclose(archivo);
    return 0;
}