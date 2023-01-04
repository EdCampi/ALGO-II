#include "pila.h"
#include "testing.h"

#include <stdbool.h>
#include <stdio.h>


static void prueba_pila_vacia(void) {
    pila_t *pila = pila_crear();
    print_test("PILA ESTÁ VACIA", pila_esta_vacia(pila) == true);
    pila_destruir(pila);
    printf("------------\n");
}

static void prueba_pila_apilar_desapilar_enteros(void) {
    pila_t *pila = pila_crear();
    int numeros[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    pila_apilar(pila, &numeros[0]);
    print_test("SE APILÓ EL PRIMER NÚMERO, 1", pila_ver_tope(pila) == &numeros[0]);
    print_test("PILA NO DEBE ESTAR VACIA", pila_esta_vacia(pila) == false);
    for (int i = 1; i < 10; i++) {
        pila_apilar(pila, &numeros[i]);
    }
    print_test("TOPE DEBERÍA SER 10", *(int*)pila_ver_tope(pila) == 10);
    print_test("DESAPILAR DEVUELVE 10", *(int *)pila_desapilar(pila) == 10);
    print_test("TOPE ES 9", *(int *)pila_ver_tope(pila) == 9);
    print_test("DESAPILAR DEVUELVE 9", *(int *)pila_desapilar(pila) == 9);
    print_test("TOPE ES 8", *(int *)pila_ver_tope(pila) == 8);
    print_test("VUELVO A APILAR EL 1", pila_apilar(pila, &numeros[0]) == true);
    print_test("NUEVO TOPE ES 1", *(int *)pila_ver_tope(pila) == 1);


    while (pila_ver_tope(pila) != NULL) {
        pila_desapilar(pila);
    }
    print_test("PILA SE VACIO", pila_esta_vacia(pila) == true);
    pila_destruir(pila);
    printf("------------\n");
}

static void prueba_pila_apilar_null(void) {
    
    pila_t *pila = pila_crear();
    void * es_nada = NULL;

    print_test("PUEDO APILAR \"NULL\"", pila_apilar(pila, es_nada) == true);
    print_test("NUEVO TOPE ES NULL", pila_ver_tope(pila) == NULL);
    print_test("DESPILAR DEVUELVE NULL", pila_desapilar(pila) == NULL);
    
    pila_destruir(pila);
    printf("------------\n");  
}

static void prueba_pila_recien_creada(void) {
    pila_t *pila = pila_crear();
    print_test("PILA REACIEN CREADA, ESTA_VACIA TRUE", pila_esta_vacia(pila) == true);
    print_test("PILA REACIEN CREADA, VER_TOPE NULL", pila_ver_tope(pila) == NULL);
    print_test("PILA REACIEN CREADA, DESAPILAR NULL", pila_desapilar(pila) == NULL);
    pila_destruir(pila);

    printf("------------\n");
}

static void prueba_pila_apilar_un_gran_volumen(void) {
    pila_t *pila = pila_crear();
    long i = 0, n[10100];
    while (i < 10100) {
        n[i] = i;
        pila_apilar(pila, &n[i]);
        i++;
    }
    print_test("PILA_VER_TOPE DESPUÉS DEL WHILE ES 10099", *(long *)pila_ver_tope(pila) == 10099);
    pila_desapilar(pila);
    print_test("PILA_VER_TOPE DESPUÉS DE DESAPILAR ES 10098", *(long *)pila_ver_tope(pila) == 10098);
    pila_desapilar(pila);
    print_test("PILA_VER_TOPE DESPUÉS DE DESAPILAR ES 10097", *(long *)pila_ver_tope(pila) == 10097);
    pila_desapilar(pila);
    print_test("PILA_VER_TOPE DESPUÉS DE DESAPILAR ES 10096", *(long *)pila_ver_tope(pila) == 10096);
    pila_desapilar(pila);
    print_test("PILA_VER_TOPE DESPUÉS DE DESAPILAR ES 10095", *(long *)pila_ver_tope(pila) == 10095);
    i = i - 4;
    while (i > 4) {
        pila_desapilar(pila);
        i--;
    }
    print_test("PILA_VER_TOPE DESPUÉS DE DESAPILAR CASI TODO ES PUNTERO AL NÚMERO 3", *(long *)pila_ver_tope(pila) == 3);
    pila_desapilar(pila);
    print_test("PILA_VER_TOPE DESPUÉS DE DESAPILAR CASI TODO ES PUNTERO AL NÚMERO 2", *(long *)pila_ver_tope(pila) == 2);
    pila_desapilar(pila);
    print_test("PILA_VER_TOPE DESPUÉS DE DESAPILAR CASI TODO ES PUNTERO AL NÚMERO 1", *(long *)pila_ver_tope(pila) == 1);
    pila_desapilar(pila);
    print_test("PILA_VER_TOPE DESPUÉS DE DESAPILAR CASI TODO ES PUNTERO AL NÚMERO 0", *(long *)pila_ver_tope(pila) == 0);
    pila_desapilar(pila);
    print_test("PILA_VER_TOPE DESPUÉS DESAPILAR TODO ES NULL", pila_ver_tope(pila) == NULL);
    print_test("PILA_DESAPILAR DESPUÉS DE QUE SE DESAPILÓ TODO ES NULL", pila_desapilar(pila) == NULL);
    print_test("PILA_ESTA_VACIA", pila_esta_vacia(pila) == true);
    pila_destruir(pila);
    printf("------------\n");
}


void pruebas_pila_estudiante() {
    prueba_pila_vacia();
    prueba_pila_apilar_desapilar_enteros();
    prueba_pila_apilar_null();
    prueba_pila_recien_creada();
    prueba_pila_apilar_un_gran_volumen();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
