#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>


/* ARBOL BINARIO DE BÚSQUEDA */

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

//Crea un TDA árbol vacío, el mismo también almacena una función para comparar claves y para destruir los datos dentro del árbol. 
//POST: Devuelve el puntero al árbol si este se creó correctamente, caso contrario devuelve un puntero nulo.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

//Guarda el dato con la clave asociada dentro del árbol, eligiendo su posición
//idonea siguiendo el criterio que posee la función de comparación de claves dada al momento de crear el árbol.
//POST: Devuelve verdadero si se pudo guardar correctamente el dato, falso en caso contrario.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

//Borra el elemento del árbol dada la clave del mismo.
//POST: Devuelve el puntero al dato del elemento si este existe, sino devuelve un puntero nulo.
void *abb_borrar(abb_t *arbol, const char *clave);

//Permite obtener el puntero al dato del elemento del árbol, si se pasó una clave que pertenezca al mismo.
//POST: Se devuelve el puntero al dato del elemento elegido, si el mismo no existe se devuelve un puntero nulo.
void *abb_obtener(const abb_t *arbol, const char *clave);

//Verifica que una clave pertenezca o no al árbol.
//POST: Devuelve verdadero si la clave pertenece, falso en caso contrario.
bool abb_pertenece(const abb_t *arbol, const char *clave);

//Devuelve la cantidad de elementos que hay dentro del árbol.
size_t abb_cantidad(const abb_t *arbol);

//Destruye el árbol y sus datos contenidos, si al momento de crear el mismo se
//dio una función de destrucción no nula.
void abb_destruir(abb_t *arbol);


/* ITERADOR INTERNO - TIPO DE RECORRIDO: IN ORDER */

//Reccorre el árbol de manera in order hasta que la función visitar, aplicada para cada elemento del árbol
//devuelva falso, si nunca se da este caso se iterará hasta el final del árbol.
void abb_in_order(abb_t *arbol, bool visitar(const char *clave, void *dato, void *extra), void *extra);


/* ITERADOR EXTERNO - TIPO DE RECORRIDO: IN ORDER */

typedef struct abb_iter abb_iter_t;

//Crea un iterador externo y devuelve el puntero al mismo, que recorre el árbol de manera in order.
//Devuelve un puntero nulo si se produjó algún error al crear el iterador.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

//Devuelve true si se puede avanzar el iterador hacia el próximo elemento.
//Caso contrario devuelve falso.
bool abb_iter_in_avanzar(abb_iter_t *iter);

//Si el iterador alcanzó el final del árbol devuelve un puntero nulo, caso contrario
//devuelve el puntero a la clave del elemento del árbol.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

//Devuelve true si el iterador ya alcanzó el final del árbol.
bool abb_iter_in_al_final(const abb_iter_t *iter);

//Destruye el iterador externo.
void abb_iter_in_destruir(abb_iter_t* iter);

#endif