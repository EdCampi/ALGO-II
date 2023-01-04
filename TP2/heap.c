#define _POSIX_C_SOURCE 200809L

#include "heap.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define REDIMENSION 2
#define REDIMENSION_ACHICAR 4

/* ESTRUCTURA - HEAP */

struct heap {
    elemento_t **datos;
    cmp_func_t comparar;
    size_t cantidad;
    size_t capacidad;
    size_t capacidad_original;
};

/* PRIMITIVAS - HEAP */

heap_t *heap_crear(cmp_func_t cmp, size_t n) {
    heap_t *heap = malloc(sizeof(heap_t));
    heap->capacidad = n;
    heap->capacidad_original = n;
    heap->cantidad = 0;
    heap->comparar = cmp;

    heap->datos = malloc(n*sizeof(elemento_t*));
    if(!heap->datos) return NULL;
    return heap;
}

//Realiza un intercambio entre dos posiciones dado un arreglo de elementos_t.
void intercambiar(elemento_t* arr[], size_t pos_padre, size_t pos_hijo) {
    elemento_t *elemento_aux = arr[pos_padre];
    arr[pos_padre] = arr[pos_hijo];
    arr[pos_hijo] = elemento_aux; 
}

//Realiza un upheap para encontrar el lugar adecuado en el arreglo para el elemento.
void upheap(elemento_t *arreglo[], size_t hijo, cmp_func_t cmp) {
    if (hijo <= 0) { return; }
    size_t padre = (hijo-1)/2;
    int ok = cmp(elemento_obtener_valor(arreglo[padre]), elemento_obtener_valor(arreglo[hijo])),
        clave_ok = strcmp(elemento_obtener_clave(arreglo[padre]), elemento_obtener_clave(arreglo[hijo]));

    if (ok < 0 || (ok == 0 && clave_ok > 0) ) {
        intercambiar(arreglo, padre, hijo);
        upheap(arreglo, padre, cmp);
    }
}

//Calcula y devuelve la posición con menor prioridad entre los hijos o la del padre si ninguna satisface las condiciones.
size_t pos_minimo(elemento_t *arreglo[], cmp_func_t cmp, size_t pos_padre, size_t pos_hijo_i, size_t pos_hijo_d) {
    
    int izq_vs_der = cmp(elemento_obtener_valor(arreglo[pos_hijo_i]), elemento_obtener_valor(arreglo[pos_hijo_d])),
        padre_vs_der = cmp(elemento_obtener_valor(arreglo[pos_padre]), elemento_obtener_valor(arreglo[pos_hijo_d])),
        padre_vs_izq = cmp(elemento_obtener_valor(arreglo[pos_padre]), elemento_obtener_valor(arreglo[pos_hijo_i]));
    //Comparacion de claves si me da menor a cero es mas grade (A >> Z).
    int clave_izq_vs_der = strcmp(elemento_obtener_clave(arreglo[pos_hijo_i]), elemento_obtener_clave(arreglo[pos_hijo_d])),
        clave_padre_vs_der = strcmp(elemento_obtener_clave(arreglo[pos_padre]), elemento_obtener_clave(arreglo[pos_hijo_d])),
        clave_padre_vs_izq = strcmp(elemento_obtener_clave(arreglo[pos_padre]), elemento_obtener_clave(arreglo[pos_hijo_i]));
    
    //Los hijos son iguales en valor, el padre es menor.
    if (!izq_vs_der && padre_vs_der < 0) {
        if (clave_izq_vs_der < 0) return pos_hijo_i;
        return pos_hijo_d;
    }
    if (izq_vs_der < 0 && padre_vs_der < 0) return pos_hijo_d; 
    if (izq_vs_der > 0 && padre_vs_izq < 0) return pos_hijo_i;

    //Si tanto el padre y los hijos son iguales en valor, paso a comparar claves.
    if (!izq_vs_der && !padre_vs_der) {
        if (clave_izq_vs_der <= 0 && clave_padre_vs_izq > 0) return pos_hijo_i;
        if (clave_izq_vs_der > 0 && clave_padre_vs_der > 0) return pos_hijo_d;
    }
    
    return pos_padre;
}

//Realiza un downheap para encontrar la posición más idonea para el elemento una vez intercambiados para desencolar uno.
void downheap(elemento_t *arreglo[], size_t longitud, size_t posicion_padre, cmp_func_t cmp) {
    if (posicion_padre >= longitud) return;

    size_t hijo_izquierdo = (2*posicion_padre)+1, hijo_derecho = (hijo_izquierdo+1 < longitud) ? (2*posicion_padre)+2 : hijo_izquierdo,
        posicion_minimo = (hijo_derecho < longitud) ? pos_minimo(arreglo, cmp, posicion_padre, hijo_izquierdo, hijo_derecho) : posicion_padre;

    if (posicion_minimo != posicion_padre) {
        intercambiar(arreglo, posicion_padre, posicion_minimo);
        downheap(arreglo, longitud, posicion_minimo, cmp);
    }
}

//Genera un nuevo arreglo para el heap con una capacidad diferente.
bool redimension_heap(heap_t *heap, size_t nueva_capacidad) {
    elemento_t **nuevo_arreglo = realloc(heap->datos, sizeof(elemento_t*)*nueva_capacidad);
    if (!nuevo_arreglo) { return false; }
    heap->datos = nuevo_arreglo;
    heap->capacidad = nueva_capacidad;
    return true;
}

bool heap_encolar(heap_t *heap, size_t elem, const char *clave) {
    if (heap->cantidad == heap->capacidad) { //no se para que tengo esto si tengo que tener un valor fijo
        bool ok = redimension_heap(heap, heap->capacidad*REDIMENSION);
        if (!ok) { return false; }
    }

    char *clave_aux = strdup(clave);
    elemento_t *nuevo = elemento_nuevo(elem, clave_aux);
    if (!nuevo) { return false; }
    heap->datos[heap->cantidad] = nuevo;
    upheap(heap->datos, heap->cantidad, heap->comparar);
    heap->cantidad++;
    return true;
}

size_t longitud(heap_t *tda) {
    return tda->cantidad;
}

bool heap_esta_vacio(heap_t *tda) {
    return !tda->cantidad;
}

void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap)) { return NULL; }

    elemento_t *elemento_aux = heap->datos[0];
    heap->datos[0] = heap->datos[heap->cantidad-1];
    downheap(heap->datos, heap->cantidad, 0, heap->comparar);
    heap->cantidad--;

    if (heap->cantidad <= heap->capacidad/REDIMENSION_ACHICAR && (heap->capacidad/REDIMENSION)+1 >= heap->capacidad_original) {
        size_t nueva_capacidad = (heap->capacidad/REDIMENSION)+1;
        redimension_heap(heap, nueva_capacidad);
    }
    return elemento_aux;
}

void heap_destruir(heap_t *heap) {
    for (size_t i = 0; i < heap->cantidad; i++) {
        elemento_destruir(heap->datos[i]);
    }
    free(heap->datos);
    free(heap);
}

void reiniciar_heap(heap_t *heap) {
    heap->capacidad = heap->capacidad_original;
    size_t i = 0;
    while (i < heap->cantidad) {
        elemento_destruir(heap->datos[i]);
        i++;
    }
    heap->datos = realloc(heap->datos, sizeof(elemento_t*)*heap->capacidad);
    heap->cantidad = 0;
}