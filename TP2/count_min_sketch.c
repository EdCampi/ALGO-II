#include "count_min_sketch.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#define CANTIDAD_CONTADORES 3


/* FUNCIONES DE HASHING */
/* Las funciones fueron obtenidas de: http://www.cs.yorku.ca/~oz/hash.html */

static unsigned long obtener_indice_primera_opcion(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

static unsigned long obtener_indice_segunda_opcion(str) 
    unsigned char *str;
    {
        unsigned long hash = 0;
        int c;

        while ((c = *str++))
            hash = c + (hash << 6) + (hash << 16) - hash;

        return hash;
    }

static unsigned long obtener_indice_tercera_opcion(const char *str) {
	unsigned int hash = 0;
	int c;

	while ((c = *str++))
	    hash += c;

	return hash;
}

/* ESTRUCTURAS */

struct count_min_sketch {
    size_t capacidad; 
    size_t **tabla_de_contadores;
};

/* PRIMITIVAS */

count_min_sketch_t *cms_crear(size_t capacidad_de_cada_contador) {
    count_min_sketch_t *tda = malloc(sizeof(count_min_sketch_t));
    if (!tda) { return NULL; }

    tda->capacidad = capacidad_de_cada_contador;
    tda->tabla_de_contadores = malloc(sizeof(size_t*)*CANTIDAD_CONTADORES);
    if (!tda->tabla_de_contadores) { return NULL; }

    for (int i = 0; i < CANTIDAD_CONTADORES; i++) {
        tda->tabla_de_contadores[i] = calloc(tda->capacidad, sizeof(size_t));
        if (!tda->tabla_de_contadores[i]) { return NULL; }
    }
    return tda;
}

//Función que genera los tres indices de la matriz de contadores.
size_t *generar_indices(const char *str, size_t capacidad) {
    size_t *indices = malloc(sizeof(size_t)*CANTIDAD_CONTADORES);
    indices[0] = obtener_indice_primera_opcion(str) % capacidad;
    indices[1] = obtener_indice_segunda_opcion(str) % capacidad;
    indices[2] = obtener_indice_tercera_opcion(str) % capacidad;
    return indices;
}

//Aumenta en uno la cantidad presente en los indices de la tabla interna del tda.
void aumentar_contador(count_min_sketch_t *tda, size_t *indices) {
    for (int i = 0; i < CANTIDAD_CONTADORES; i++) {
        tda->tabla_de_contadores[i][indices[i]]++;
    }
}

//Dado los tres indices busca el valor mínimo entre esos.
size_t iterar_por_minimo(count_min_sketch_t *tda, size_t *indices) {
    size_t minimo = tda->tabla_de_contadores[0][indices[0]];
    for (int i = 0; i < CANTIDAD_CONTADORES; i++) {
        size_t actual = tda->tabla_de_contadores[i][indices[i]];
        if (actual < minimo) { minimo = actual; }
    }
    return minimo;
}

void cms_aumentar(count_min_sketch_t *tda, const char *str1) {
    size_t *indices = generar_indices(str1, tda->capacidad);
    aumentar_contador(tda, indices);
    free(indices);
}

size_t cms_ocurrencias(count_min_sketch_t *tda, const char *str1) {
    size_t *indices = generar_indices(str1, tda->capacidad),
    minimo = iterar_por_minimo(tda, indices);
    free(indices);
    return minimo;
}

void cms_destruir(count_min_sketch_t *tda) {
    for (int i = 0; i < CANTIDAD_CONTADORES; i++) {
        free(tda->tabla_de_contadores[i]);
    }
    free(tda->tabla_de_contadores);
    free(tda);
}