#include <stddef.h>

/* DEFINICIÓN DE ESTRUCTURAS */
struct count_min_sketch;
typedef struct count_min_sketch count_min_sketch_t;

/* PRIMITIVAS DE LA ESTRUCTURA COUNT MIN SKETCH */

//Genera una matriz con 3 filas donde se puede llevar la cuenta de la cantidad de ocurrencias de unas determinadas claves dadas.
count_min_sketch_t *cms_crear(size_t capacidad_de_cada_contador);

//Dada una o dos cadenas de texto aumenta el contador de ocurrencias en la matriz utilizando 3 funciones de hashing diferentes.
//PRE: Si no hay segunda cadena de texto se debe ingresar un puntero nulo como tercer parámetro.
void cms_aumentar(count_min_sketch_t *tda, const char *str1);

//Devuelve el valor mínimo entre los 3 valores que la estructura proporciona como contadores de ocurrencias.
//PRE: Si no hay segunda cadena de texto se debe ingresar un puntero nulo como tercer parámetro.
size_t cms_ocurrencias(count_min_sketch_t *tda, const char *str1);

//Destruye la matriz dada evitando así perdidas de memoria.
void cms_destruir(count_min_sketch_t *tda);