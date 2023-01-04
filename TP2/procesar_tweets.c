#define  _POSIX_C_SOURCE 200809L

#define PROMEDIO_TAG_LINEA 5
#define LONGITUD_CONTADORES 75
#define BASE_DECIMAL 10

#include "count_min_sketch.h"
#include "strutil.h"
#include "heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

//Funcion auxiliar para el heap
int funcion_de_comparacion_de_enteros(size_t x, size_t y) {
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

//Funcipon que analiza cada tag, aumentando la cantidad de apariciones en la tabla de CMS y guardando la cadena en un hash.
void analizar_tag(char *tag, count_min_sketch_t *contadores, hash_t *hash) {
    cms_aumentar(contadores, tag);
    hash_guardar(hash, tag, NULL);
}

//Función que analiza la línea (de tipo CSV), la divide en la etiquetas, y trabaja con cada etiqueta ignorando el nombre de usuario.
void analizar_linea(char *linea, size_t n, size_t k, count_min_sketch_t *contadores, heap_t *heap, hash_t *hash, char *etiquetas[]) { //o(N) donde N es la cantidad de lineas
    linea[strlen(linea)-1]= '\0';
    char **usuario_etiquetas = split(linea, ',');
    size_t i = 1; //Ignoro la primera parte, el nombre de usuario.
    while (usuario_etiquetas[i]) {
        analizar_tag(usuario_etiquetas[i], contadores, hash);
        i++;
    }
    free_strv(usuario_etiquetas);
}

//Dado un heap, un hash lleno de claves, y una tabla de CMS, el mismo imprime en orden descendente y alfabético los k TTs de
//las líneas del archvo CSV dado por entrada estándar.
void imprimir_resultado(heap_t *heap, count_min_sketch_t *contadores, hash_t *hash, size_t *contador_ciclos, size_t k) {
    char **arreglo = hash_obtener_arreglo_claves(hash);
    size_t k_aux = k;

    for (size_t i = 0; i < hash_cantidad(hash); i++) {
        size_t minimo = cms_ocurrencias(contadores, arreglo[i]);
        heap_encolar(heap, minimo, arreglo[i]);
    }
    free(arreglo);
    fprintf(stdout, "--- %lu\n", *contador_ciclos);
    *contador_ciclos = *contador_ciclos + 1;

    while (k_aux && !heap_esta_vacio(heap)) {
        elemento_t *elemento = heap_desencolar(heap);
        fprintf(stdout, "%ld %s\n", elemento_obtener_valor(elemento), elemento_obtener_clave(elemento));
        k_aux--;
        elemento_destruir(elemento);
    }
    hash_reiniciar(hash);
    reiniciar_heap(heap);
}

//Realiza una lectura y analisis del archivo dado por parámetro.
void leer_archivo(FILE *archivo, count_min_sketch_t *contadores, heap_t *heap, hash_t *hash, size_t n, size_t k) {
    char *claves[n], *linea = NULL;
    size_t capacidad = 0, contador_ciclos = 1, n_aux = n;
    
    while(getline(&linea, &capacidad, archivo) != -1) {
        if (n_aux) {
            analizar_linea(linea, n, k, contadores, heap, hash, claves);
            n_aux--;
        } else {
            n_aux = n-1;
            imprimir_resultado(heap, contadores, hash, &contador_ciclos, k);
            analizar_linea(linea, n, k, contadores, heap, hash, claves);
        }
    }
    imprimir_resultado(heap, contadores, hash, &contador_ciclos, k);
    free(linea);
}

int main(int arg, char *argv[]) {
    size_t n = (size_t)strtol(argv[1], NULL, BASE_DECIMAL),
        k = (size_t)strtol(argv[2], NULL, BASE_DECIMAL);
    FILE *archivo = stdin;

    count_min_sketch_t *contadores = cms_crear(n*LONGITUD_CONTADORES);
    heap_t *heap = heap_crear(funcion_de_comparacion_de_enteros, n*PROMEDIO_TAG_LINEA);
    hash_t *hash = hash_crear(NULL);

    leer_archivo(archivo, contadores, heap, hash, n, k);

    hash_destruir(hash);
    cms_destruir(contadores);
    heap_destruir(heap);
    return 0;
}