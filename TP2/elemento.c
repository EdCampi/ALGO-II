#define _POSIX_C_SOURCE 200809L

#include "elemento.h"

#include <stdbool.h>  // bool
#include <stddef.h>   // size_t
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ESTRUCTURA */

struct elemento {
    char *clave;
    size_t valor;
};

/* PRIMITIVAS */

elemento_t *elemento_nuevo(size_t elem, char *clave) {
    elemento_t *elemento_nuevo = malloc(sizeof(elemento_t));
    if (!elemento_nuevo) { return NULL; }
    elemento_nuevo->valor = elem;
    elemento_nuevo->clave = clave;
    return elemento_nuevo;
}

char* elemento_obtener_clave(elemento_t *elemento) {
    return elemento->clave;
}

size_t elemento_obtener_valor(elemento_t *elemento) {
    return elemento->valor;
}

void elemento_destruir(elemento_t *elemento) {
    free(elemento->clave);
    free(elemento);
}

elemento_t **ordenar_arreglo_de_elementos(elemento_t *arreglo[], size_t n, size_t max, size_t min, int indice) {
    elemento_t **res = calloc(n, sizeof(elemento_t*));
    size_t k = max - min, contador[k+1], i = 0;
    memset(contador, 0, sizeof(contador));

    while (i < n) {
        size_t valor = (indice != -1) ? (elemento_obtener_clave(arreglo[i])[indice])-min : elemento_obtener_valor(arreglo[i])-min;
        contador[valor]++;
        i++;
    }
    size_t acumulador[k+1], valor_act = 0;
    memset(acumulador, 0, sizeof(acumulador));

    for (i = 0; i < k; i++) {
        valor_act += contador[i];
        acumulador[i+1] = valor_act;
    }

    for (i = 0; i < n; i++) {
        size_t valor = (indice != -1) ? (elemento_obtener_clave(arreglo[i])[indice])-min : elemento_obtener_valor(arreglo[i])-min,
            indice_arreglo_res = acumulador[valor];
        
        for (size_t j = 0; j < contador[valor]; j++) {
            if (j > 0) { acumulador[valor]++; break; }
            res[indice_arreglo_res] = arreglo[i];
            indice_arreglo_res++;
        }
    }
    free(arreglo);
    if (indice < 0 ) return res;
    if (indice > 0) return ordenar_arreglo_de_elementos(res, n, max, min, indice-1);
    size_t nuevo_maximo = 0;
    for (i = 0; i < n; i++) {
        size_t valor_actual = elemento_obtener_valor(res[i]);
        if (nuevo_maximo < valor_actual) nuevo_maximo = valor_actual;
    }
    return ordenar_arreglo_de_elementos(res, n, nuevo_maximo, min, indice-1);
}

void destruir_arreglo_elementos(elemento_t *arrreglo[], size_t n) {
    for (int i = 0; i < n; i++) {
        elemento_destruir(arrreglo[i]);
    }
    free(arrreglo);
}