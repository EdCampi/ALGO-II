#include <stdbool.h>
#include <stdlib.h>

#include "hash.h"

typedef int (*cmp_func_t)(size_t a, size_t b);

struct heap;
typedef struct heap heap_t;

/* PRIMITIVAS HEAP */

//Crea una estructura vacía con propiedad de heap, esta nueva estrucutra tiene la posibilidad de almacenar una clave asociadas en cada elemento.
heap_t *heap_crear(cmp_func_t cmp, size_t n);

//Devuelve true si la estructura está vacía, caso contrario falso.
bool heap_esta_vacio(heap_t *heap);

//Encola la clave con el dato asociada mantienendo el orden de la estructura según el criterio de comparación proporcionado por la función
//con la se inició la estructura previamente.
bool heap_encolar(heap_t *heap, size_t dato, const char *clave);

//Devuelve el dato con mayor prioridad eliminandolo de la estructura.
void *heap_desencolar(heap_t *heap);

//Devuelve la cantidad de elementos presentes en la estructura.
size_t longitud(heap_t *heap);

//Destruye el heap pasado por parámetro.
void heap_destruir(heap_t *heap);

//Reinicia el estado del heap dejándolo como recién creado. 
void reiniciar_heap(heap_t *heap);