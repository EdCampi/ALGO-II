#include "heap.h"

#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define CANTIDAD_INICIAL 0
#define CAPACIDAD_INICIAL 50
#define REDIMENSION 2
#define REDIMENSION_ACHICAR 4

struct heap {
    void **datos;
    cmp_func_t comparar;
    size_t cantidad;
    size_t capacidad;
};

//Dado un heap recién creado ajusta la propiedades de la estructura.
bool iniciar_heap(heap_t *heap, size_t cantidad, size_t capacidad, cmp_func_t cmp) {
    heap->cantidad = cantidad;
    heap->capacidad = capacidad;
    heap->comparar = cmp;
    heap->datos = malloc(sizeof(void*)*capacidad);
    if (!heap->datos) { return false; }
    return true;
}

heap_t *heap_crear(cmp_func_t cmp) {
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) { return NULL; }

    if(!iniciar_heap(heap, CANTIDAD_INICIAL, CAPACIDAD_INICIAL, cmp)) { free(heap); return NULL; }
    return heap;
}

//Calcula la posición del hijo más chico (o grande dependiendo de la función de comparación),
//si el padre es el más grade de los tres, se devuelve la misma posición.
size_t pos_minimo(void *arreglo[], cmp_func_t cmp, size_t pos_padre, size_t pos_hijo_i, size_t pos_hijo_d) {
    
    if (cmp(arreglo[pos_hijo_i], arreglo[pos_hijo_d]) <= 0 && cmp(arreglo[pos_padre], arreglo[pos_hijo_d]) < 0) {
        return pos_hijo_d;
    }
    if (cmp(arreglo[pos_hijo_i], arreglo[pos_hijo_d]) > 0 && cmp(arreglo[pos_padre], arreglo[pos_hijo_i]) < 0) {
        return pos_hijo_i;
    }
    return pos_padre;
}

//Realiza un cambio entre dos elementos de un arreglo dados sus respectivos índices.
void swap(void* arr[], size_t pos_padre, size_t pos_hijo) {
    void *dato_aux = arr[pos_padre];
    arr[pos_padre] = arr[pos_hijo];
    arr[pos_hijo] = dato_aux; 
}

//Modifica el arreglo haciendo un downheap al elemento dado para conseguir la propiedad de heap en el arreglo.
void downheap(void *arreglo[], size_t longitud, size_t posicion_padre, cmp_func_t cmp) {
    if (posicion_padre >= longitud) { return; }
    size_t hijo_izquierdo = (2*posicion_padre)+1, hijo_derecho = (hijo_izquierdo+1 < longitud) ? (2*posicion_padre)+2 : hijo_izquierdo;

    size_t posicion_minimo = (hijo_derecho < longitud) ? pos_minimo(arreglo, cmp, posicion_padre, hijo_izquierdo, hijo_derecho) : posicion_padre;
    if (posicion_minimo != posicion_padre) {
        swap(arreglo, posicion_padre, posicion_minimo);
        downheap(arreglo, longitud, posicion_minimo, cmp);
    }
}

//Esta función le da la propiedad de heap al arreglo.
void heapify(void *datos[], size_t n, cmp_func_t cmp) {
    for (long i = (long)n; i >= 0; i--) {
        downheap(datos, n, (size_t)i, cmp);
    }
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) { return NULL; }
    if (!iniciar_heap(heap, n, n, cmp)) { free(heap); return NULL; }

    for (size_t i = 0; i < n; i++) {
        heap->datos[i] = arreglo[i];
    }

    heapify(heap->datos, n, cmp);
    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
    for (size_t i = 0; i < heap->cantidad; i++) {
        if (destruir_elemento) { destruir_elemento(heap->datos[i]); }
    }
    free(heap->datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
    return !heap->cantidad;
}

//Realiza un upheap al elemento elegido si es necesario.
void upheap(void *arreglo[], size_t hijo, cmp_func_t cmp) {
    if (hijo <= 0) { return; }
    size_t padre = (hijo-1)/2;
    if (cmp(arreglo[padre], arreglo[hijo]) < 0) {
        swap(arreglo, padre, hijo);
        upheap(arreglo, padre, cmp);
    }
}

//Redimensiona el arreglo. Devuelve verdadero si se da exitosamente, falso en caso contrario.
//Si la redimensión falló el arreglo no se ve modificado.
bool redimension(heap_t *heap, size_t nueva_capacidad) {
    void *nuevo_arreglo = realloc(heap->datos, sizeof(void*) * nueva_capacidad);
    if (!nuevo_arreglo) { return false; }
    heap->datos = nuevo_arreglo;
    heap->capacidad = nueva_capacidad;
    return true;
}

bool heap_encolar(heap_t *heap, void *elem) {
    if (heap->cantidad == heap->capacidad) {
        if (!redimension(heap, heap->capacidad*REDIMENSION)) { return false; } //Si la redimensión falla al estar lleno el heap ya no se puede añadir el elemento a la cola.
    }

    heap->datos[heap->cantidad] = elem;
    upheap(heap->datos, heap->cantidad, heap->comparar);
    heap->cantidad++;
    return true;
}

void *heap_ver_max(const heap_t *heap) {
    if (heap_esta_vacio(heap)) { return NULL; }
    return heap->datos[0];
}

void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap)) { return NULL; }

    void *dato_aux = heap->datos[0];
    heap->datos[0] = heap->datos[heap->cantidad-1];
    downheap(heap->datos, heap->cantidad, 0, heap->comparar);
    heap->cantidad--;

    if (heap->cantidad <= heap->capacidad/REDIMENSION_ACHICAR && (heap->capacidad/REDIMENSION)+1 >= CAPACIDAD_INICIAL) {
        size_t nueva_capacidad = (heap->capacidad/REDIMENSION)+1;
        redimension(heap, nueva_capacidad);
    }
    return dato_aux;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
    heapify(elementos, cant, cmp);
    long longitud_relativa = cant;

    while (longitud_relativa > 0) {
        swap(elementos, 0, (size_t)longitud_relativa-1);
        longitud_relativa--;
        downheap(elementos, (size_t)longitud_relativa, 0, cmp);
    }
}