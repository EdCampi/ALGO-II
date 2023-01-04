#define _POSIX_C_SOURCE 200809L

#include "abb.h"
#include "pila.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANTIDAD_INICIAL 0


/* ÁRBOL BINARIO DE BÚSUEDA */

typedef struct abb_elemento {
    char* clave;
    void* dato;
    struct abb_elemento* hijo_izquierdo;
    struct abb_elemento *hijo_derecho;
} abb_elemento_t;

struct abb {
    struct abb_elemento *raiz;
    abb_destruir_dato_t funcion_destruir;
    abb_comparar_clave_t funcion_comparar;
    size_t cantidad_de_elementos;
};

abb_elemento_t *crear_elemento(const char* clave, void* dato) {
    abb_elemento_t *elemento = malloc(sizeof(abb_elemento_t));
    if (!elemento) { return NULL; }
    char *clave_aux = strdup(clave);
    if (!clave_aux) { return NULL; }
    elemento->clave = clave_aux;
    elemento->dato = dato;
    elemento->hijo_derecho = NULL;
    elemento->hijo_izquierdo = NULL;
    return elemento;
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t *arbol = malloc(sizeof(abb_t));
    if (!arbol) { return NULL; }
    arbol->funcion_comparar = cmp;
    arbol->funcion_destruir = destruir_dato;
    arbol->cantidad_de_elementos = CANTIDAD_INICIAL;
    arbol->raiz = NULL;
    return arbol;
}

//Dado un elemeto reemplaza el dato asociado al mismo por el nuevo.
//POST: Libera la memoria tomada por el elemento anterior.
void reemplazar_en_el_elemento(abb_t *arbol, abb_elemento_t *elemento, void *dato) {
    if (arbol->funcion_destruir) { arbol->funcion_destruir(elemento->dato); }
    elemento->dato = dato;
}

//Función con dos utilidades:
//1. Conectar el elemento padre con el nuevo hijo, en el caso de una inserción.
//2. Si el booleano lo indica, eliminar una hoja del arbol.
void conectar_desconectar_con_el_padre(abb_t *arbol, abb_elemento_t *elemento, abb_elemento_t *padre, bool desconexion) {
    if (padre && arbol->funcion_comparar(padre->clave, elemento->clave) < 0) {
        padre->hijo_derecho = (desconexion) ? NULL : elemento;
    } else if (padre) {
        padre->hijo_izquierdo = (desconexion) ? NULL : elemento;
    }
    if (desconexion) {
        free(elemento->clave);
        free(elemento);
    }
}

//Si el elemento pertenece al árbol, esta función devuelve el padre o el elemento que sea buscado.
void navegar_al_elemento_coincidente(const abb_t *arbol, abb_elemento_t **elemento_act, abb_elemento_t **padre, const char *clave) {
    while (*elemento_act && arbol->funcion_comparar((*elemento_act)->clave, clave)) {
        if (padre) { *padre = *elemento_act; }
        if (arbol->funcion_comparar((*elemento_act)->clave, clave) < 0) {
            *elemento_act = ((*elemento_act)->hijo_derecho);
        } else {
            *elemento_act = ((*elemento_act)->hijo_izquierdo);
        }
    }
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    abb_elemento_t *elemento_act = arbol->raiz, *padre = NULL;
    if (abb_pertenece(arbol, clave)) {
        navegar_al_elemento_coincidente(arbol, &elemento_act, &padre, clave);
        reemplazar_en_el_elemento(arbol, elemento_act, dato);
        return true;
    }

    abb_elemento_t *elemento = crear_elemento(clave, dato);
    if (!elemento) { return false; }
    if (!elemento_act) {
        arbol->cantidad_de_elementos++;
        arbol->raiz = elemento;
        return true;
    }
    navegar_al_elemento_coincidente(arbol, &elemento_act, &padre, clave);

    conectar_desconectar_con_el_padre(arbol, elemento, padre, false);
    arbol->cantidad_de_elementos++;
    return true;
}

//Funcion para borrar un elemento que cuenta únicamente con un hijo.
void borrar_elemento_con_un_hijo(abb_t *arbol, abb_elemento_t *elemento_act, abb_elemento_t *padre) {
    abb_elemento_t *elemento_hijo = (elemento_act->hijo_izquierdo) ? elemento_act->hijo_izquierdo : elemento_act->hijo_derecho;
    conectar_desconectar_con_el_padre(arbol, elemento_hijo, padre, false);

    if (arbol->funcion_comparar(arbol->raiz->clave, elemento_act->clave) == 0) {
        arbol->raiz = (elemento_act->hijo_izquierdo) ? elemento_act->hijo_izquierdo : elemento_act->hijo_derecho;
    }
    free(elemento_act->clave);
    free(elemento_act);
}

//Busca un reemplazante para el lugar del elemento con nietos o el hijo derecho, el mismo se elige viendo entre los nietos el más chico (en este caso: el elemento que este
//más a la izquierda del hijo derecho) o se utiliza el hijo derecho en caso de tratarse el mismo de una hoja.
void* borrar_elemento_con_dos_hijos(abb_t *arbol, abb_elemento_t *elemento_act, abb_elemento_t *padre, void *dato_aux) {
    abb_elemento_t *elemento_a_intercambiar = elemento_act->hijo_derecho, *padre_aux = elemento_act;
    while (elemento_a_intercambiar->hijo_izquierdo) {
        padre_aux = elemento_a_intercambiar;
        elemento_a_intercambiar = elemento_a_intercambiar->hijo_izquierdo;
    }
    char *clave_reemplazante = strdup(elemento_a_intercambiar->clave);
    if (!clave_reemplazante) { return NULL; }
    void *dato_reemplazante = elemento_a_intercambiar->dato;

    if (!elemento_a_intercambiar->hijo_derecho && !elemento_a_intercambiar->hijo_izquierdo) {
        conectar_desconectar_con_el_padre(arbol, elemento_a_intercambiar, padre_aux, true);
    } else {
        borrar_elemento_con_un_hijo(arbol, elemento_a_intercambiar, padre_aux);
    }  
    free(elemento_act->clave);
    elemento_act->clave = clave_reemplazante;
    elemento_act->dato = dato_reemplazante;
    return dato_aux;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
    if (!abb_cantidad(arbol) || !abb_pertenece(arbol, clave)) { return NULL; }

    arbol->cantidad_de_elementos--;
    abb_elemento_t *elemento_act = arbol->raiz, *padre = NULL;
    navegar_al_elemento_coincidente(arbol, &elemento_act, &padre, clave);
    void *dato_aux = elemento_act->dato;

    if (!elemento_act->hijo_derecho && !elemento_act->hijo_izquierdo) {
        conectar_desconectar_con_el_padre(arbol, elemento_act, padre, true);
        if (!arbol->cantidad_de_elementos) { arbol->raiz = NULL; }
        return dato_aux;
    }
    if ((!elemento_act->hijo_izquierdo && elemento_act->hijo_derecho) || (elemento_act->hijo_izquierdo && !elemento_act->hijo_derecho)) {
        borrar_elemento_con_un_hijo(arbol, elemento_act, padre);
        return dato_aux;
    }

    return borrar_elemento_con_dos_hijos(arbol, elemento_act, padre, dato_aux);
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    if (!abb_pertenece(arbol, clave)) { return NULL; }
    abb_elemento_t *elemento_act = arbol->raiz;
    navegar_al_elemento_coincidente(arbol, &elemento_act, NULL, clave);
    return elemento_act->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    abb_elemento_t *elemento_act = arbol->raiz;

    if (!elemento_act || !arbol->cantidad_de_elementos) { return false; }
    navegar_al_elemento_coincidente(arbol, &elemento_act, NULL, clave);
    return (elemento_act) ? true : false;
}

size_t abb_cantidad(const abb_t *arbol) {
    return arbol->cantidad_de_elementos;
}

//Función auxiliar para destruir los datos internos del árbol, recorriendo el mismo de la manera pre order.
void destruir_elemento(abb_t *arbol, abb_elemento_t *elemento) {
    if (!elemento) { return; }
    if (arbol->funcion_destruir) { arbol->funcion_destruir(elemento->dato); }
    free(elemento->clave);
    destruir_elemento(arbol, elemento->hijo_derecho);
    destruir_elemento(arbol, elemento->hijo_izquierdo);
    free(elemento);
}

void abb_destruir(abb_t *arbol) {
    destruir_elemento(arbol, arbol->raiz);
    free(arbol);
}


/* ITERADOR INTERNO IN ORDER */

//Función auxiliar del iterador interno, donde se realiza la iteración en forma in order, el mismo depende de un booleano
//que actua como centinela para responder a la condición de corte, es decir, terminar la iteración.
void _abb_in_order(abb_elemento_t *elemento, bool visitar(const char *, void *, void *), void *extra, bool *continuar) {
    if (!elemento) { return; }
    if (*continuar) _abb_in_order(elemento->hijo_izquierdo, visitar, extra, continuar);
    if (*continuar && !visitar(elemento->clave, elemento->dato, extra)) {
        *continuar = false;
    }
    if (*continuar) _abb_in_order(elemento->hijo_derecho, visitar, extra, continuar);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
    if (!arbol) { return; }
    bool continuar = true;
    _abb_in_order(arbol->raiz, visitar, extra, &continuar);
}


/* ITERADOR EXTERNO IN ORDER */

struct abb_iter {
    abb_t *arbol_TDA;
    pila_t *pila_del_iterador;
};

//Función para modularizar el recorrido automático hacia la izquierda.
//POST: Apila todos los elementos que se encuentren en la pila dada.
void recorrer_hacia_la_izquierda(abb_elemento_t *elemento, pila_t *pila) {
    while (elemento) {
        pila_apilar(pila, (void *)elemento);
        elemento = elemento->hijo_izquierdo;
    }
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    abb_iter_t *iterador = malloc(sizeof(abb_iter_t));
    if (!iterador) { return NULL; }
    iterador->pila_del_iterador = pila_crear();
    if (!iterador->pila_del_iterador) { return NULL; }
    abb_elemento_t *elemento_act = arbol->raiz;
    recorrer_hacia_la_izquierda(elemento_act, iterador->pila_del_iterador);
    return iterador;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) { return false; }
    abb_elemento_t *elemento = (abb_elemento_t*)pila_desapilar(iter->pila_del_iterador);
    elemento = elemento->hijo_derecho;
    if (elemento) {
        recorrer_hacia_la_izquierda(elemento, iter->pila_del_iterador);
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) { return NULL; }
    return ((abb_elemento_t *)pila_ver_tope(iter->pila_del_iterador))->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return pila_esta_vacia(iter->pila_del_iterador);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila_del_iterador);
    free(iter);
}