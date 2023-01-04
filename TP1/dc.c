#define _POSIX_C_SOURCE 200809L

#include "calc_helper.h"
#include "cola.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CAPACIDAD_INICIAL 0
#define INDICE_INICIAL 0
#define CANTIDAD_INICIAL 0
#define NO_REDUCIR 0
#define REDUCIR_EN_UNO 1
#define UN_UNICO_RESULTADO 1
#define REDUCIR_EN_DOS 2
#define LONGITUD_CADENA_ERROR 6
#define LONGITUD_CADENA_DEFECTO 50


//Función axuliar para modularizar la función "operar_con_token". Simplemente apila el resultado y
//ajusta los contadores dados.
void apilar_resultado(pilanum_t *pila, calc_num numero, size_t *contador_numeros, size_t cantidad_a_reducir, size_t *contador_signos) {
    *contador_signos = *contador_signos - 1;
    *contador_numeros = *contador_numeros - cantidad_a_reducir;
    apilar_num(pila, numero);
}

//Dada una pila con numeros y un token de operación, realiza la operacion dada apilando el resultado en la misma pila.
//También ajusta los contadores dados según el tipo de operacion indicada.
void operar_con_token(struct calc_token *token, pilanum_t *pila, size_t *contador_numeros, size_t *contador_signos, size_t *contador_invalidos) {
    calc_num numero_1 = (calc_num)NULL,
        numero_2 = (calc_num)NULL,
        numero_3 = (calc_num)NULL;
    bool desapilado = desapilar_num(pila, &numero_2);
    if (!desapilado) { *contador_invalidos = *contador_invalidos + 1; return; }
    if (token->oper.op != OP_RAIZ) {
        desapilado = desapilar_num(pila, &numero_1);
        if (!desapilado) { *contador_invalidos = *contador_invalidos + 1; return; }
    }
    switch (token->oper.op) {
        case OP_ADD:
            apilar_resultado(pila, numero_1+numero_2, contador_numeros, REDUCIR_EN_UNO, contador_signos);
            break;
        case OP_SUB:
            apilar_resultado(pila, numero_1-numero_2, contador_numeros, REDUCIR_EN_UNO, contador_signos);
            break;
        case OP_MUL:
            apilar_resultado(pila, numero_1*numero_2, contador_numeros, REDUCIR_EN_UNO, contador_signos);
            break;
        case OP_DIV:
            if (!numero_2) {
                *contador_invalidos = *contador_invalidos + 1;
                break; 
            }
            apilar_resultado(pila, numero_1/numero_2, contador_numeros, REDUCIR_EN_UNO, contador_signos);
            break;
        case OP_POW:
            if (numero_2 < 0) {
                *contador_invalidos = *contador_invalidos + 1;
                break;
            }
            apilar_resultado(pila, pow(numero_1,numero_2), contador_numeros, REDUCIR_EN_UNO, contador_signos);
            break;
        case OP_LOG:
            if (numero_2 < 2) {
                *contador_invalidos = *contador_invalidos + 1;
                break;
            }
            apilar_resultado(pila, log(numero_1)/log(numero_2), contador_numeros, REDUCIR_EN_UNO, contador_signos);
            break;
        case OP_RAIZ:
            if (numero_2 < 0) {
                *contador_invalidos = *contador_invalidos + 1;
                break;
            }
            apilar_resultado(pila, sqrt(numero_2), contador_numeros, NO_REDUCIR, contador_signos);
            break;
        case OP_TERN:
            desapilar_num(pila, &numero_3);
            apilar_resultado(pila, (numero_3) ? numero_1 : numero_2, contador_numeros, REDUCIR_EN_DOS, contador_signos);
            break;
        default:
            *contador_invalidos = *contador_invalidos + 1;
    }
}

//Función auxliar para imprimir los resultados de cada línea que se encuentran dentro de la cola de resultados.
void imprimir_resultados(cola_t *resultados) {
    while (!cola_esta_vacia(resultados)) {
        char *res = (char*)cola_desencolar(resultados);
        fprintf(stdout, "%s\n", res);
        free(res);
    }
}

//Permite encular una cadena con el resultado de la operacion o el mensaje de error en los resultados.
void encolar_resultados(cola_t *cola, pilanum_t *pila, size_t *cantidad_de_numeros_restantes, size_t *cantidad_de_signos_invalidos, size_t *cantidad_de_operadores_restantes) {
    if (*cantidad_de_numeros_restantes == UN_UNICO_RESULTADO && *cantidad_de_signos_invalidos == CANTIDAD_INICIAL && *cantidad_de_operadores_restantes == CANTIDAD_INICIAL) {
        char *res = malloc(sizeof(char)*LONGITUD_CADENA_DEFECTO);
        calc_num respuesta = (calc_num)NULL;
        desapilar_num(pila, &respuesta);
        sprintf(res, "%ld", respuesta);
        cola_encolar(cola, res);
    } else {
        char *error = malloc(sizeof(char)*LONGITUD_CADENA_ERROR);
        strcpy(error, "ERROR");
        cola_encolar(cola, error);
    }
}

//Dado un archivo mediante entrada estándar compuesto por lineas con operaciones matemáticas en notación posfija
//devuelve mediante salida estánder (imprime) los resultados de las mismas en el orden dado.
void calculadora_notacion_posfija(char **linea, cola_t *resultados) {
    pilanum_t *pila_numeros = pilanum_crear();
    size_t i = 0,
        contador_apariciones_numeros = CANTIDAD_INICIAL,
        contador_invalidos = CANTIDAD_INICIAL,
        contador_signos = CANTIDAD_INICIAL;

    while (linea[i]) {
        struct calc_token *token = malloc(sizeof(struct calc_token));
        bool valido = calc_parse(linea[i], token);
        if (!valido) { contador_invalidos++; }
        switch (token->type) {
            case TOK_NUM:
                contador_apariciones_numeros++;
                apilar_num(pila_numeros, token->value);
                break;
            case TOK_OPER:
                contador_signos++;
                operar_con_token(token, pila_numeros, &contador_apariciones_numeros, &contador_signos, &contador_invalidos);
                break;
            default:
                contador_invalidos++;
                break;
        }
        free(token);
        free(linea[i]);
        i++;
    }

    encolar_resultados(resultados, pila_numeros, &contador_apariciones_numeros, &contador_invalidos, &contador_signos);
    pilanum_destruir(pila_numeros);
}

int main(void) {
    FILE *archivo = stdin;
    if (!archivo) { return 0; }
    
    cola_t *resultados = cola_crear();
    if (!resultados) { return 0; }

    char *line = NULL;
    size_t capacidad = CAPACIDAD_INICIAL;
    size_t cantidad_caracteres_leidos = getline(&line, &capacidad, archivo);
    if (!cantidad_caracteres_leidos) { return 0; } //La primera línea está vacía -> el archivo también.
    while(!feof(archivo)) {
        char **contenido_de_linea = dc_split(line);
        calculadora_notacion_posfija(contenido_de_linea, resultados);
        cantidad_caracteres_leidos = getline(&line, &capacidad, archivo);
        free(contenido_de_linea);
        
    }
    free(line);
    fclose(archivo);
    imprimir_resultados(resultados);
    cola_destruir(resultados, free);
    return 0;
}