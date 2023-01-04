#include <stdbool.h>
#include <stdlib.h>

/* ESTRUCTURA */

struct elemento;
typedef struct elemento elemento_t;

/* PRIMITIVAS ELEMENTO */

//Crea un elemento nuevo, una estructura que tiene una clave y un numero asociados.
elemento_t *elemento_nuevo(size_t elem, char *clave);

//Devuelve la clave asociada al elemento.
char *elemento_obtener_clave(elemento_t *elemento);

//Devuelve el valor asociado al elemento.
size_t elemento_obtener_valor(elemento_t *elemento);

//Destruye el elemento dado.
void elemento_destruir(elemento_t *elemento);

/* Ordena usando counting sort, ordena el arreglo primero por letras de manera recursiva y luego por el numero asociado al elemento.
 * PRE: Ingresar "-1" en el índice para ordenar números, de otra manera
 * un número positivo será utilizado para ordenar recursivamente
 * cadenas de texto desde el indice dado hacia el indice 0, para finalmente 
 * ordenar por numero (siguiendo la recursividad de índice hacia -1).
 * K = max-min --> min siempre será 0.
 */
elemento_t **ordenar_arreglo_de_elementos(elemento_t *arreglo[], size_t n, size_t max, size_t min, int indice);

//Dado un arreglo de elementos destruye el mismo y los elementos dentro de este.
void destruir_arreglo_elementos(elemento_t *arrreglo[], size_t n);