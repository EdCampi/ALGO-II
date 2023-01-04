#include "cola.h"
#include "testing.h"
#include "pila.h" //Incluido solo para las pruebas de la pila dentro de la cola.



#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//Wrapper para la función "pila_destruir" de "pila.c".
void _destruir_pila(void *pila) {
    pila_destruir(pila);
}


static void prueba_cola_nueva(void) {
    printf("PRUEBAS COLA NUEVA\n");
    cola_t *cola = cola_crear();
    
    print_test("Está vacía == true", cola_esta_vacia(cola) == true);
    print_test("Ver primero == NULL", cola_ver_primero(cola) == NULL);
    print_test("Desecolar == NULL", cola_desencolar(cola) == NULL);
    cola_destruir(cola,  NULL);
    printf("-------------\n");
}

static void prueba_cola_encolar_desencolar_tres_elementos(void) {
    printf("ENCOLAR TRES ELEMENTOS\n");
    cola_t *cola = cola_crear();

    print_test("Está vacía == true", cola_esta_vacia(cola) == true);
    char a = 'a', b = 'b', c = 'c';
    cola_encolar(cola, &a);
    print_test("Ver primero == puntero a \"a\"", cola_ver_primero(cola) == &a);
    cola_encolar(cola, &b);
    print_test("Encolar \"b\" -> Ver primero == puntero a \"a\"", cola_ver_primero(cola) == &a);
    cola_encolar(cola, &c);
    print_test("Encolar \"c\" -> Ver primero == puntero a \"a\"", cola_ver_primero(cola) == &a);

    void * puntero_a = cola_desencolar(cola);
    print_test("Desencolar == puntero a \"a\"", puntero_a == &a);
    print_test("Ver primero == puntero a \"b\"", cola_ver_primero(cola) == &b);

    void * puntero_b = cola_desencolar(cola);
    print_test("Desencolar == puntero a \"b\"", puntero_b == &b);
    print_test("Ver primero == puntero a \"c\"", cola_ver_primero(cola) == &c);

    void * puntero_c = cola_desencolar(cola);
    print_test("Desencolar == puntero a \"c\"", puntero_c == &c);
    print_test("Está vacía == true", cola_esta_vacia(cola) == true);
    print_test("Ver primero == NULL", cola_ver_primero(cola) == NULL);
    print_test("Desencolar == NULL", cola_desencolar(cola) == NULL);
    print_test("Está vacía (2da vez) == true", cola_esta_vacia(cola) == true);
    print_test("Ver primero (2da vez) == NULL", cola_ver_primero(cola) == NULL);
    print_test("Desencolar (2da vez) == NULL", cola_desencolar(cola) == NULL);

    cola_destruir(cola,  NULL);
    printf("-------------\n");
}

static void prueba_destruir_con_funcion_nula(void) {
    printf("PRUEBA DESTRUIR 100 ELEMENTOS SIN FUNCIÓN, NULL\n");
    cola_t *cola = cola_crear();

    int datos[100];
    for (int i = 0; i < 100; i++) {
        datos[i] = i;
        cola_encolar(cola, &datos[i]);
    }
    print_test("Encolar 100 punteros -> Ver primero == puntero a 0", cola_ver_primero(cola) == &datos[0]);

    cola_destruir(cola, NULL);
    printf("-------------\n");
}

static void prueba_destruir_con_funcion(void) {
    printf("DESTRUIR COLA CON FUNCIÓN FREE\n");
    cola_t *cola = cola_crear();

    void *mega_entero = malloc(sizeof(int) * 10000);
    void *char_gigante = malloc(sizeof(char) * 100000);
    cola_encolar(cola, mega_entero);
    print_test("Ver primero ==  puntero a una posición de memoria creada con malloc", cola_ver_primero(cola) == mega_entero);
    bool booleano_1 = cola_encolar(cola, char_gigante);
    print_test("Encolar puntero a posición de memoria creada con malloc por segunda vez == true", booleano_1 == true);

    cola_destruir(cola, free);
    printf("-------------\n");
}

static void prueba_encolar_dos_pilas_dinamicas(void) {
    printf("ENCOLAR DOS PILAS DINÁMICAS / DESTRUIR CON FUNCIÓN \"PILA_DESTRUIR\"\n");
    cola_t *cola = cola_crear();
    pila_t *pila = pila_crear();
    pila_t *pila_2 = pila_crear();

    int datos[100], datos_2[1000];
    for (int i = 0; i < 100; i++) {
        datos[i] = i;
        pila_apilar(pila, &datos[i]);
    }
    for (int j = 0; j < 1000; j++) {
        datos_2[j] = j;
        pila_apilar(pila, &datos_2[j]);
    }

    bool booleano_1 = cola_encolar(cola, pila);
    print_test("Encolar pila dinámica con 100 punteros == true", booleano_1 == true);
    print_test("Ver primero == puntero a pila_1", cola_ver_primero(cola) == pila);

    bool booleano_2 = cola_encolar(cola, pila_2);
    print_test("Encolar pila dinámica con 1000 punteros == true", booleano_2 == true);
    print_test("Ver primero == puntero a pila_1", cola_ver_primero(cola) == pila);

    cola_destruir(cola, _destruir_pila);
    printf("-------------\n");
}

static void prueba_encolar_un_gran_volumen(void) {
    printf("PRUEBA VOLUMEN\n");
    cola_t *cola = cola_crear();

    size_t datos[100000];
    for (size_t i = 0; i < 100000; i++) {
        datos[i] = i;
        cola_encolar(cola, &datos[i]);
    }

    print_test("Después de 100000 elementos, primer elemento es 0", cola_ver_primero(cola) == &datos[0]);
    cola_desencolar(cola);
    print_test("Desencolar -> primer elemento es 1", cola_ver_primero(cola) == &datos[1]);
    cola_desencolar(cola);
    print_test("Desencolar -> primer elemento es 2", cola_ver_primero(cola) == &datos[2]);
    cola_desencolar(cola);
    print_test("Desencolar -> primer elemento es 3", cola_ver_primero(cola) == &datos[3]);
    cola_desencolar(cola);
    print_test("Desencolar -> primer elemento es 4", cola_ver_primero(cola) == &datos[4]);
    cola_desencolar(cola);
    print_test("Desencolar -> primer elemento es 5", cola_ver_primero(cola) == &datos[5]);

    for (size_t i = 0; i < 99990; i++) {
        cola_desencolar(cola);
    }

    print_test("Desencolar casi todo -> primer elemento es 99995", cola_ver_primero(cola) == &datos[99995]);
    cola_desencolar(cola);
    print_test("Desencolar casi todo -> primer elemento es 99996", cola_ver_primero(cola) == &datos[99996]);
    cola_desencolar(cola);
    print_test("Desencolar casi todo -> primer elemento es 99997", cola_ver_primero(cola) == &datos[99997]);
    cola_desencolar(cola);
    print_test("Desencolar casi todo -> primer elemento es 99998", cola_ver_primero(cola) == &datos[99998]);
    cola_desencolar(cola);
    print_test("Desencolar casi todo -> primer elemento es 99999", cola_ver_primero(cola) == &datos[99999]);
    cola_desencolar(cola);

    print_test("Desencolar todo -> esta vacia == true", cola_esta_vacia(cola) == true);
    print_test("Desencolar todo -> ver primero == NULL", cola_ver_primero(cola) == NULL);
    print_test("Desencolar todo -> desencolar == NULL", cola_desencolar(cola) == NULL);

    cola_destruir(cola, NULL);
    printf("-------------\n");
}

static void prueba_encolar_null(void) {
    printf("ENCOLAR NULL\n");
    cola_t *cola = cola_crear();

    void *dato = NULL;
    print_test("Antes de encolar NULL está vacía == true", cola_esta_vacia(cola) == true);

    cola_encolar(cola, dato);
    print_test("Encolar NULL -> está vacía == false", cola_esta_vacia(cola) == false);
    print_test("Desencolar == puntero NULL", cola_desencolar(cola) == dato);
    print_test("Desencolar (sin elementos) == NULL", cola_desencolar(cola) == NULL);
    print_test("Está vacía == true", cola_esta_vacia(cola) == true);

    cola_destruir(cola, NULL);
    printf("-------------\n");
}


void pruebas_cola_estudiante(void) {
    prueba_cola_nueva();
    prueba_cola_encolar_desencolar_tres_elementos();
    prueba_destruir_con_funcion_nula();
    prueba_destruir_con_funcion();
    prueba_encolar_dos_pilas_dinamicas(); 
    prueba_encolar_un_gran_volumen();
    prueba_encolar_null();
}

#ifndef CORRECTOR  

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;
}

#endif