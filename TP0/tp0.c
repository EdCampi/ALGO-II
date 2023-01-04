#include "tp0.h"
#include <stdio.h>

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap(int *x, int *y) {
    //Intercambia dos valores enteros.

    int aux_x = *x;
    *x = *y;
    *y = aux_x; 
}


int maximo(int vector[], int n) {
    /* Devuelve la posición del mayor elemento del arreglo, o -1 si el
    el vector es de largo 0. Si el máximo elemento aparece más de una
    vez, se debe devolver la primera posición en que ocurre.*/

    if (!n) {
        return -1;
    }
    
    int maximo = vector[0], posicion_maxima = 0;
    for (int i = 1; i < n; i++) {
        if (vector[i] > maximo) {
            posicion_maxima = i;
            maximo = vector[i];
        }
    }
    return posicion_maxima;
}


int comparar(int vector1[], int n1, int vector2[], int n2) {
    /* Compara dos arreglos de longitud especificada.
    Devuelve -1 si el primer arreglo es menor que el segundo; 0 si son
    iguales; o 1 si el primero es el mayor.
    Un arreglo es menor a otro cuando al compararlos elemento a elemento,
    el primer elemento en el que difieren no existe o es menor. */
    if (!n1 && !n2) {
        return 0;
    }
    
    int i = 0, j = 0;
    while (i < n1 && j < n2) {
        if (vector1[i] < vector2[j]) {
            return -1;
        }
        if (vector1[i] > vector2[j]) {
            return 1;
        }
        i++;
        j++;
    }

    if (i >= n1 && j < n2) { return -1; }
    if (j >= n2 && i < n1) { return 1; }
    return 0;
}


void seleccion(int vector[], int n) {
    // Ordena el arreglo recibido mediante el algoritmo de selección.
    for (int i = n-1; i > 0; i--) {
        int posicion_maximo = maximo(vector, i+1);
        swap(&vector[i], &vector[posicion_maximo]);
    }
}