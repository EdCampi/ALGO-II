#include "heap.h"
#include "testing.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

int funcion_de_comparacion_de_enteros(const void *x, const void* y) {
    if (*(int*)x < *(int*)y) {
        return -1;
    }
    if (*(int*)x > *(int*)y) {
        return 1;
    }
    return 0;
}

static void prueba_crear_heap_vacio(void) {
    heap_t *heap = heap_crear(funcion_de_comparacion_de_enteros);

    print_test("Heap recién creado está vacío", heap_esta_vacio(heap));
    print_test("Cantidad es 0", !heap_cantidad(heap));
    print_test("Desencolar devuelve un vector nulo", !heap_desencolar(heap));
    print_test("Ver máximo es nulo", !heap_ver_max(heap));

    heap_destruir(heap, NULL);
}

static void prueba_heap_guardar(void) {
    heap_t *heap = heap_crear(funcion_de_comparacion_de_enteros);
    int uno = 1, dos = 2, tres = 3, cuatro = 4, cinco = 5, seis = 6, siete = 7, ocho = 8, nueve = 9, diez = 10; 

    printf("Primera ronda\n");
    print_test("Encolo 5", heap_encolar(heap, &cinco));
    print_test("Encolo 4", heap_encolar(heap, &cuatro));
    print_test("Encolo 8", heap_encolar(heap, &ocho));
    print_test("Encolo 9", heap_encolar(heap, &nueve));
    print_test("Encolo 2", heap_encolar(heap, &dos));
    
    print_test("Desencolar me da el 9", heap_desencolar(heap) == &nueve);
    print_test("Desencolar me da el 8", heap_desencolar(heap) == &ocho);
    print_test("Desencolar me da el 5", heap_desencolar(heap) == &cinco);
    print_test("Desencolar me da el 4", heap_desencolar(heap) == &cuatro);
    print_test("Desencolar me da el 2", heap_desencolar(heap) == &dos);
    print_test("Heap está vacío", heap_esta_vacio(heap));

    printf("Segunda ronda\n");
    print_test("Encolo 5", heap_encolar(heap, &cinco));
    print_test("Encolo 4", heap_encolar(heap, &cuatro));
    print_test("Encolo 8", heap_encolar(heap, &ocho));
    print_test("Encolo 9", heap_encolar(heap, &nueve));
    print_test("Encolo 2", heap_encolar(heap, &dos));
    print_test("Encolo 7", heap_encolar(heap, &siete));
    print_test("Encolo 6", heap_encolar(heap, &seis));
    print_test("Encolo 10", heap_encolar(heap, &diez));
    print_test("Encolo 1", heap_encolar(heap, &uno));
    print_test("Encolo 3", heap_encolar(heap, &tres));

    /*        10
            /    \
           9      7
          / \    / \
         8   3  5   6
        / \  /
       4   1 2         */
    print_test("Desencolar me da el 10", heap_desencolar(heap) == &diez);
    /*       9
           /   \
          8     7
         / \   / \
        4   3 5   6
       / \
      2   1          */
    print_test("Desencolar me da el 9", heap_desencolar(heap) == &nueve);
    /*      8
          /   \
         4     7
        / \   / \
       1   3 5   6
      /
     2              */
    print_test("Desencolar me da el 8", heap_desencolar(heap) == &ocho);
    /*      7
          /   \
         4     6
        / \   / \
       1   3 5   2 */
    print_test("Desencolar me da el 7", heap_desencolar(heap) == &siete);
    /*      6
          /   \
         4     2
        / \   /
       1   3 5    */
    print_test("Desencolar me da el 6", heap_desencolar(heap) == &seis);
    /*      5
          /   \
         4     3
        / \
       2   1      */
    print_test("Desencolar me da el 5", heap_desencolar(heap) == &cinco);
    /*      4
          /   \
         3     2
        /
       1          */
    print_test("Desencolar me da el 4", heap_desencolar(heap) == &cuatro);
    /*    3
         / \
        1   2 */
    print_test("Desencolar me da el 3", heap_desencolar(heap) == &tres);
    /*  2
        \
         1 */
    print_test("Desencolar me da el 2", heap_desencolar(heap) == &dos);
    /* 1 */
    print_test("Desencolar me da el 1", heap_desencolar(heap) == &uno);

    print_test("Heap está vacío", heap_esta_vacio(heap));
    heap_destruir(heap, NULL);

}

static void prueba_heap_repetidos(void) {
    heap_t *heap = heap_crear(funcion_de_comparacion_de_enteros);
    int uno = 1, dos = 2, tres = 3;

    print_test("Encolo el 2", heap_encolar(heap, &dos));
    print_test("Vuelvo a encolar el 2", heap_encolar(heap, &dos));
    print_test("Vuelvo a encolar el 2 x2", heap_encolar(heap, &dos));
    print_test("Cantidad es 3", heap_cantidad(heap) == 3);

    print_test("Encolo el 1", heap_encolar(heap, &uno));
    print_test("Vuelvo a encolar el 1", heap_encolar(heap, &uno));
    print_test("Vuelvo a encolar el 1 x2", heap_encolar(heap, &uno));
    print_test("Vuelvo a encolar el 1 x3", heap_encolar(heap, &uno));
    print_test("Cantidad es 7", heap_cantidad(heap) == 7);

    print_test("Encolo el 3", heap_encolar(heap, &tres));
    print_test("Cantidad es 8", heap_cantidad(heap) == 8);

    print_test("Desencolar es 3", heap_desencolar(heap) == &tres);
    print_test("Desencolar es 2", heap_desencolar(heap) == &dos);
    print_test("Desencolar es 2", heap_desencolar(heap) == &dos);
    print_test("Desencolar es 2", heap_desencolar(heap) == &dos);
    print_test("Desencolar es 1", heap_desencolar(heap) == &uno);
    print_test("Desencolar es 1", heap_desencolar(heap) == &uno);
    print_test("Desencolar es 1", heap_desencolar(heap) == &uno);
    print_test("Desencolar es 1", heap_desencolar(heap) == &uno);

    print_test("Heap está vacío", heap_esta_vacio(heap));


    heap_destruir(heap, NULL);
}

static void prueba_destruir_heap_vacio_con_funcion(void) {
    heap_t *heap = heap_crear(funcion_de_comparacion_de_enteros);

    print_test("Heap recién creado está vacío", heap_esta_vacio(heap));
    print_test("Cantidad es 0", !heap_cantidad(heap));
    print_test("Desencolar devuelve un vector nulo", !heap_desencolar(heap));
    print_test("Ver máximo es nulo", !heap_ver_max(heap));

    heap_destruir(heap, free);
}

static void prueba_destruir_heap_con_funcion(void) {
    heap_t *heap = heap_crear(funcion_de_comparacion_de_enteros);
    int *uno = malloc(sizeof(int)*20), *dos = malloc(sizeof(int)*20), *tres = malloc(sizeof(int)*20), *cuatro = malloc(sizeof(int)*50), *cinco = malloc(sizeof(int)*50);
    *uno = 1, *dos = 2, *tres = 3, *cuatro = 4, *cinco = 5;

    print_test("Encolo el 2", heap_encolar(heap, dos));
    print_test("Encolo el 3", heap_encolar(heap, tres));
    print_test("Encolo el 5", heap_encolar(heap, cinco));
    print_test("Encolo el 4", heap_encolar(heap, cuatro));
    print_test("Encolo el 1", heap_encolar(heap, uno));

    heap_destruir(heap, free);
}

static void prueba_heap_volumen(size_t volumen) {
    heap_t *heap = heap_crear(funcion_de_comparacion_de_enteros);
    long numeros[volumen];

    for (size_t i = 0; i < volumen; i++) {
        numeros[i] = (long)i;
        heap_encolar(heap, &numeros[i]);
    }
    for (long i = (long)volumen-1; i >= 0; i--) {
        void *dato_aux = heap_desencolar(heap);
        if (dato_aux != &numeros[i]) {
            printf("Error el desencolar, %ld\n", numeros[i]);
            break;
        }
    }
    print_test("Heap despues de vaciar el gran volumen está vacío", heap_esta_vacio(heap));

    heap_destruir(heap, NULL);
}

static void prueba_arreglo_heap(void) {
    int digitos[30];
    void *numeros[30];
    for (int i = 0; i < 30; i++) {
        digitos[i] = i;
        numeros[i] = &digitos[i];
    }
    heap_t *heap = heap_crear_arr(numeros, 30, funcion_de_comparacion_de_enteros);
    print_test("El primero en salir es el más grande (29)", heap_desencolar(heap) == &digitos[29]);
    print_test("El segundo en salir es el segundo más grande (28)", heap_desencolar(heap) == &digitos[28]);
    for (long i = 27; i >= 0; i--) {
        void *dato_aux = heap_desencolar(heap);
        if (dato_aux != &digitos[i]) {
            printf("Error el desencolar, %d\n", digitos[i]);
            break;
        }
    }
    print_test("Heap está vacío y ningún elemento salió fuera de su orden", heap_esta_vacio(heap));

    heap_destruir(heap, NULL);
}

static void prueba_heap_sort_arreglo_desordenado(void) {
    int digitos[30] = {29, 22, 23, 0, 1, 3, 10, 20, 21, 5, 8, 18, 19, 24, 14, 28, 26, 25, 27, 17, 4, 16, 2, 11, 12, 7, 13, 9 , 6, 15};
    void *numeros[30];
    for (int i = 0; i < 30; i++) {
        numeros[i] = &digitos[i];
    }
    heap_sort(numeros, 30, funcion_de_comparacion_de_enteros);
    bool ok = true;
    int ultimo_numero = *(int*)numeros[0];
    for (int i = 1; i < 30; i++) {
        if (ultimo_numero >= *(int*)numeros[i]) { ok = false; }
    }
    print_test("Los números están ordenados", ok);
}

static void prueba_heap_sort_arreglo_ordenado(long cant) {
    int digitos[cant];
    void *numeros[cant];
    for (int i = 0; i < cant; i++) {
        digitos[i] = i;
        numeros[i] = &digitos[i];
    }

    heap_sort(numeros, (size_t)cant, funcion_de_comparacion_de_enteros);
    bool ok = true;
    int ultimo_numero = *(int*)numeros[0];
    for (int i = 1; i < cant; i++) {
        if (ultimo_numero >= *(int*)numeros[i]) { ok = false; }
    }

    print_test("Los números se mantuvieron en el orden correcto", ok);
}

void pruebas_heap_estudiante(void) {
    printf("Prueba crear un heap vacío\n");
    prueba_crear_heap_vacio();
    printf("-------------------------\n");
    printf("Prueba guardar varios datos en un heap\n");
    prueba_heap_guardar();
    printf("-------------------------\n");
    printf("Prueba repetidos\n");
    prueba_heap_repetidos();
    printf("-------------------------\n");
    printf("Prueba destuir heap vacío con función free");
    prueba_destruir_heap_vacio_con_funcion();
    printf("-------------------------\n");
    printf("Prueba destuir heap lleno con función free");
    prueba_destruir_heap_con_funcion();
    printf("-------------------------\n");
    printf("Prueba encolar en volumen\n");
    prueba_heap_volumen(5000);
    printf("-------------------------\n");
    printf("Prueba modificar el arreglo para que se vuelva un heap\n");
    prueba_arreglo_heap();
    printf("-------------------------\n");
    printf("Prueba heap sort en arreglo desordenado\n");
    prueba_heap_sort_arreglo_desordenado();
    printf("-------------------------\n");
    printf("Prueba heap sort en arreglo ordenado\n");
    prueba_heap_sort_arreglo_ordenado(1000);
    printf("-------------------------\n");
}

#ifndef CORRECTOR

int main(void) {
    pruebas_heap_estudiante();
    return 0;
}

#endif