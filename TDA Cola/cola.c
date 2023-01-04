#include "cola.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define CANTIDAD_INICIAL 0


typedef struct nodo {
    void* dato;
    struct nodo* siguiente; 
} nodo_t;

struct cola {
    size_t cantidad;
    nodo_t* primero;
    nodo_t* ultimo;
};


nodo_t *nodo_crear(void *elemento) {
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if(!nodo) { return NULL; }
    nodo->dato = elemento;
    nodo->siguiente = NULL;
    return nodo;
}

cola_t *cola_crear(void) {
    cola_t *cola = malloc(sizeof(cola_t));
    if (!cola) { return NULL; }
    
    cola->primero = NULL;
    cola->ultimo = NULL;
    cola->cantidad = CANTIDAD_INICIAL;

    return cola;
}

bool cola_esta_vacia(const cola_t *cola) {
    return !cola->cantidad;
}

bool cola_encolar(cola_t* cola, void *valor) {
    nodo_t *nodo = nodo_crear(valor);
    if (!nodo) {
        free(cola);
        return false; 
    }
    
    if (cola_esta_vacia(cola)) {
        cola->primero = nodo;
    } else {
        cola->ultimo->siguiente = nodo;
    }
    cola->ultimo = nodo;
    cola->cantidad++;
    return true;
}

void *cola_ver_primero(const cola_t *cola) {
    if (cola_esta_vacia(cola)) { return NULL; }
    return cola->primero->dato;
}

void *cola_desencolar(cola_t *cola) {
    if (cola_esta_vacia(cola)) { return NULL; }
    void *primer_dato = cola->primero->dato;
    void *primer_nodo = cola->primero;
    cola->primero = cola->primero->siguiente;
    cola->cantidad--;
    free(primer_nodo);
    return primer_dato;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)) {
    if (!destruir_dato) {
        while (!cola_esta_vacia(cola)) {
            cola_desencolar(cola);
        }
    }

    while (!cola_esta_vacia(cola)) {
        void *puntero_a_eliminar = cola_ver_primero(cola);
        destruir_dato(puntero_a_eliminar);
        cola_desencolar(cola);
    }
    free(cola);
}