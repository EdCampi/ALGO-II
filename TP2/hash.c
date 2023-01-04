#define _POSIX_C_SOURCE 200809L

#include "hash.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VACIO 0
#define BORRADO 1
#define LLENO 2
#define CANTIDAD_INICIAL 0
#define LIMITE_MINIMO_DE_CARGA 0.2
#define LIMITE_MAXIMO_DE_CARGA 0.75
#define CRITERIO_DE_REDIMENSION_ACHICAR 2
#define CRITERIO_DE_REDIMENSION_AGRANDAR 3
#define LONGITUD_CADENA 15
#define CAPACIDAD_INICIAL 20

//HASH CERRADO

/*ESTRUCTURAS HASH*/

typedef struct hash_nodo {
    char *clave_nodo;
    void *dato;
    int estado;
} hash_nodo_t;

struct hash {
    size_t capacidad;
    size_t cantidad;
    size_t borrados;
    hash_destruir_dato_t funcion_destruir;
    hash_nodo_t *tabla;
};

/*PRIMITIVAS DEL HASH*/

bool generar_tabla(hash_t *hash) {
    hash->tabla = malloc(sizeof(hash_nodo_t)*hash->capacidad);
    if (!hash->tabla) { return false; }
    for (size_t i = 0; i < hash->capacidad; i++) {
        hash->tabla[i].estado = VACIO;
        hash->tabla[i].dato = NULL;
        hash->tabla[i].clave_nodo = NULL;
    }
    return true;
}
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    
    hash_t *hash = malloc(sizeof(hash_t));
    if (!hash) { return NULL; }

    hash->cantidad = CANTIDAD_INICIAL;
    hash->capacidad = CAPACIDAD_INICIAL;
    hash->borrados = CANTIDAD_INICIAL;
    hash->funcion_destruir = destruir_dato;
    
    bool tabla_generada = generar_tabla(hash);
    if (!tabla_generada) {  free(hash); return NULL; }
    return hash;
}

//Función de hashing (encontrada en http://www.cs.yorku.ca/~oz/hash.html).
static unsigned long obtener_indice(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

//Función auxliar inserta en la tabla de hash un elemento en la posición indicada y marca su celda como "LLENA".
void insertar_en_tabla(hash_t *hash, size_t indice, const char *clave, void *dato, bool reemplazar) {
    if (!reemplazar) {
        char *clave_aux = strdup(clave);
        hash->tabla[indice].clave_nodo = clave_aux;   
    } else {
        if (hash->funcion_destruir) { hash->funcion_destruir(hash->tabla[indice].dato); }
    }
    hash->tabla[indice].dato = dato;
    hash->tabla[indice].estado = LLENO;
}

//Devuelve la posición correcta donde debería ir ubicada la clave.
void buscar_indice(const hash_t *hash, size_t *indice, const char* clave) {
    while (hash->tabla[*indice].estado != VACIO) {
        if (hash->tabla[*indice].estado == LLENO && !strcmp(hash->tabla[*indice].clave_nodo, clave)) { break; }
        *indice = (*indice < hash->capacidad-1) ? *indice+1 : 0;
    }
}

//Función auxuliar de "hash_guardar" guarda en la posición que corresponda el par clave-dato.
bool agegar_en_la_tabla(hash_t *hash, size_t indice, const char *clave, void* dato) {
    bool reemplazar = hash_pertenece(hash, clave);
    buscar_indice(hash, &indice, clave);
    insertar_en_tabla(hash, indice, clave, dato, reemplazar);
    return true;
}

//Función auxiliar para generar claves auxiliares que no sea constantes y obtener el inidice de la misma.
size_t generador_de_indices(const char *clave, size_t capacidad) {
    size_t indice = obtener_indice(clave) % (capacidad);
    return indice;
}

//Función auxiliar para redimensionar la tabla (achicarla o agrandarla).
bool redimension(hash_t *hash, size_t nueva_capacidad) {
    hash_nodo_t *tabla_anterior = hash->tabla;
    size_t capacidad_anterior = hash->capacidad, 
        borrados_previos = hash->borrados;
    hash->capacidad = nueva_capacidad;
    hash->borrados = CANTIDAD_INICIAL;
    bool tabla_redimensionada = generar_tabla(hash);
    if (!tabla_redimensionada) { //Trato de "revertir" el proceso (solamente devuelvo las propiedades anteriores).
        hash->tabla = tabla_anterior;
        hash->capacidad = capacidad_anterior;
        hash->borrados = borrados_previos;
        return false;
    }

    for (size_t i = 0; i < capacidad_anterior; i++) {
        if (tabla_anterior[i].estado == LLENO) {
            
            size_t indice = generador_de_indices(tabla_anterior[i].clave_nodo, hash->capacidad-1);
            agegar_en_la_tabla(hash, indice, tabla_anterior[i].clave_nodo, tabla_anterior[i].dato);
            free(tabla_anterior[i].clave_nodo);
        }
    }
    free(tabla_anterior);
    return true;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {

    //Redimensión
    float factor_de_carga = ((float)hash->cantidad + (float)hash->borrados) / (float)hash->capacidad;
    if (factor_de_carga >= LIMITE_MAXIMO_DE_CARGA) {
        bool redimension_ok = redimension(hash, hash->capacidad*CRITERIO_DE_REDIMENSION_AGRANDAR);
        if (!redimension_ok) { return false; }
        
    }

    size_t indice = generador_de_indices(clave, hash->capacidad-1); 
    if (!hash_pertenece(hash, clave)) { hash->cantidad++; }
    return agegar_en_la_tabla(hash, indice, clave, dato);
}

//Función aulixar para eliminar un dato, liberar la memoria solicitada para almacenar la clave y marcar la celda como borrada.
void eliminar_dato(hash_t *hash, size_t indice) {
    hash->tabla[indice].estado = BORRADO;
    free(hash->tabla[indice].clave_nodo);
    hash->tabla[indice].clave_nodo = NULL;
    hash->tabla[indice].dato = NULL;
    hash->cantidad--;
    hash->borrados++;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    if (!hash_cantidad(hash) || !hash_pertenece(hash, clave)) { return NULL; }

    float factor_de_carga = (float)hash->cantidad / (float)hash->capacidad;
    if (factor_de_carga <= LIMITE_MINIMO_DE_CARGA && hash->capacidad/CRITERIO_DE_REDIMENSION_ACHICAR >= CAPACIDAD_INICIAL) {
        bool redimensionado_ok = redimension(hash, hash->capacidad/CRITERIO_DE_REDIMENSION_ACHICAR);
        if (!redimensionado_ok) { return NULL; }
    }
    
    size_t indice = generador_de_indices(clave, hash->capacidad-1); 
    buscar_indice(hash, &indice, clave);
    void *dato_aux = hash->tabla[indice].dato;
    eliminar_dato(hash, indice);
    return dato_aux;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
    if (!hash_cantidad(hash) || !hash_pertenece(hash, clave)) { return NULL; }

    size_t indice = generador_de_indices(clave, hash->capacidad-1); 
    buscar_indice(hash, &indice, clave);
    return (hash->tabla[indice].estado == LLENO) ? hash->tabla[indice].dato : NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    if (!hash_cantidad(hash)) { return NULL; }
    size_t indice = generador_de_indices(clave, hash->capacidad-1); 
    buscar_indice(hash, &indice, clave);
    return (hash->tabla[indice].estado == LLENO) ? true : false;
}

size_t hash_cantidad(const hash_t *hash) {
    return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
    for (size_t i = 0; i < hash->capacidad; i++) {
        if (hash->tabla[i].estado == LLENO) {
            if (hash->funcion_destruir) { hash->funcion_destruir(hash->tabla[i].dato); }
            free(hash->tabla[i].clave_nodo);
        }
    }
    free(hash->tabla);
    free(hash);
}

void hash_reiniciar(hash_t *hash) {
    for (size_t i = 0; i < hash->capacidad; i++) {
        if (hash->tabla[i].estado == LLENO) {
            if (hash->funcion_destruir) { hash->funcion_destruir(hash->tabla[i].dato); }
            free(hash->tabla[i].clave_nodo);
            hash->tabla[i].estado = VACIO;
        }
    }
    redimension(hash, CAPACIDAD_INICIAL);
    hash->cantidad = 0;
    hash->capacidad = CAPACIDAD_INICIAL;
    hash->borrados = 0;
}

/*ITERADOR DEL HASH*/

/*Estructura*/

struct hash_iter {
    const hash_t *hash_tda;
    size_t indice_actual;
};

//Función que solo itera continuamente para encontrar un nodo lleno o llegar al final del hash.
void iterar_hasta_nodo_lleno(hash_iter_t *iterador) {
    while (iterador->hash_tda->tabla[iterador->indice_actual].estado != LLENO && !hash_iter_al_final(iterador)) {
        iterador->indice_actual++;
    }
}

hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t *iterador = malloc(sizeof(hash_iter_t));
    if (!iterador) { return NULL; }

    iterador->indice_actual = (!hash_cantidad(hash)) ? hash->capacidad : 0;
    iterador->hash_tda = hash;
    if (!hash_iter_al_final(iterador)) iterar_hasta_nodo_lleno(iterador);
    return iterador;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) { return false; }
    iter->indice_actual++;
    iterar_hasta_nodo_lleno(iter);
    return (iter->hash_tda->tabla[iter->indice_actual].estado == LLENO);
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) { return NULL; }
    return iter->hash_tda->tabla[iter->indice_actual].clave_nodo;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
    return (iter->indice_actual >= iter->hash_tda->capacidad-1);
}

void hash_iter_destruir(hash_iter_t *iter) {
    free(iter);
}

/* PRIMITIVAS PARA INTERACTUAR CON ELEMENTOS_T */

char **hash_obtener_arreglo_claves(hash_t *hash) {
    char **arreglo = malloc(sizeof(char*)*hash->cantidad+1);
    size_t indice_arreglo = 0;
    for (size_t i = 0; i < hash->capacidad; i++) {
        if (hash->tabla[i].estado == LLENO) {
            arreglo[indice_arreglo] = hash->tabla[i].clave_nodo;
            indice_arreglo++;
        }
    }
    return arreglo;
}

elemento_t **hash_obtener_arreglo_de_elementos(hash_t *hash) {
    elemento_t **res = calloc(hash->cantidad, sizeof(elemento_t*));
    if (!res) return NULL;

    size_t i, indice_res = 0;
    for (i = 0; i < hash->capacidad; i++) {
        if (hash->tabla[i].estado != LLENO) continue;
        char *clave = strdup(hash->tabla[i].clave_nodo),
        neutros[] = "***************";
        size_t longitud_restante = LONGITUD_CADENA - strlen(clave),
        valor_actual = *(size_t*)hash->tabla[i].dato;
        clave = realloc(clave, strlen(neutros)+1);
        strncat(clave, neutros, longitud_restante);
        clave[LONGITUD_CADENA] = '\0';

        res[indice_res] = elemento_nuevo(valor_actual, clave);
        indice_res++;
    }
    return res;
}