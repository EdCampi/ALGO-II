//Incluido únicamente para probar una pila dinámica detro de una cola.

#include "pila.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define CAPACIDAD_INCIAL 20
#define CANTIDAD_INICIAL 0
#define DOBLE 2
#define MITAD 2
#define CUADRUPLE 4


struct pila {
    void **datos;
    size_t cantidad;
    size_t capacidad;
};


pila_t *pila_crear(void) {
    pila_t *pila = malloc(sizeof(pila_t));
    if (!pila) { return NULL; }
    pila->datos = malloc(sizeof(void*) * CAPACIDAD_INCIAL);
    pila->capacidad = CAPACIDAD_INCIAL;
    pila->cantidad = CANTIDAD_INICIAL;
    return pila;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
    return pila->cantidad == CANTIDAD_INICIAL;
}

//Función auxiliar de "pila_apilar".
void pila_agregar(pila_t *pila, void *valor) {
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
}

//Función auxliar de "pila_apilar" y "pila_desapilar".
//Devuelve falso y se encarga de liberar la pila si ocurre un error durante el manejo de memoria.
bool redimension(pila_t *pila, size_t capacidad) {
    pila->datos = realloc(pila->datos, capacidad * sizeof(void*));
    if (!pila->datos) {
        free(pila);
        return false; 
    }
    pila->capacidad = capacidad;
    return true;
}

bool pila_apilar(pila_t *pila, void *valor) {
    size_t cantidad_actual = pila->cantidad, capacidad_actual = pila->capacidad;
    if (cantidad_actual >= capacidad_actual) {
        bool redimensionado = redimension(pila, capacidad_actual * DOBLE);
        if (!redimensionado) { return false; }
    }
    pila_agregar(pila, valor);
    return true;
}

void *pila_ver_tope(const pila_t *pila) {
    if (!pila->cantidad) { return NULL; }
    return pila->datos[pila->cantidad - 1];
}

void *pila_desapilar(pila_t *pila) {
    if (!pila->cantidad) { return NULL; }
    size_t capacidad_actual = pila->capacidad, cantidad_actual = pila->cantidad;
    void *dato_aux = pila->datos[cantidad_actual - 1];
    pila->cantidad = cantidad_actual - 1;

    size_t mitad_de__la_capacidad = capacidad_actual / MITAD, cuadruple_de_los_elementos = pila->cantidad * CUADRUPLE;

    if (cuadruple_de_los_elementos <= capacidad_actual && mitad_de__la_capacidad >= CAPACIDAD_INCIAL) {
        bool redimensionado = redimension(pila, mitad_de__la_capacidad);
        if (!redimensionado) { return NULL; }
    }
    return dato_aux;
}