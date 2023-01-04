#define  _POSIX_C_SOURCE 200809L

#define LONGITUD_TABLA 1000
#define LONGITUD_USUARIO 15
#define NOMBRE_USUARIO 0

#include "strutil.h"
#include "count_min_sketch.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

//Analiza la línea y aumenta los contadores que guarda el hash de usuarios
//según la cantidad de etiquetas y si estas fueron usadas o no.
void analizar_linea(char *linea, hash_t *hash_usuarios, hash_t *hash_etiquetas) {
    linea[strlen(linea)-1] = '\0';
    char **etiquetas = split(linea, ','),
        *usuario = strdup(etiquetas[NOMBRE_USUARIO]);
    size_t i = 1; //Ignoro el user name para contar los tags.
    if (!hash_pertenece(hash_usuarios, usuario)) {
        size_t *contador = calloc(1, sizeof(size_t));
        hash_guardar(hash_usuarios, usuario, contador);
    }
    while (etiquetas[i]) {
        char *usuario_etiqueta = strdup(etiquetas[NOMBRE_USUARIO]);
        usuario_etiqueta = realloc(usuario_etiqueta, strlen(usuario_etiqueta)+strlen(etiquetas[i])+1);
        strcat(usuario_etiqueta, etiquetas[i]);
        if (!hash_pertenece(hash_etiquetas, usuario_etiqueta)) {
            hash_guardar(hash_etiquetas, usuario_etiqueta, NULL);
            *(size_t*)hash_obtener(hash_usuarios, usuario) = *(size_t*)hash_obtener(hash_usuarios, usuario)+1;
        }
        i++;
        free(usuario_etiqueta);
    }

    free(usuario);
    free_strv(etiquetas);
}

//Revisa hasta donde se debe "cortar" la cadena para que esta
//vuelva a ser la original (ignorando los '*').
size_t longitud_hasta_caracter_nulo(char *str, size_t n) {
    size_t i = 1; //Un nombre de usuario no puede estar vacío.
    while (str[i] && str[i] != '*') { i++; }
    return i;
}

//Dado el arreglo ya ordenado, imprime por salida estándar respetando el orden.
void imprimir_resultados(elemento_t *arreglo[], size_t n) {
    size_t actual = 0;
    char *clave = NULL;
    for (size_t i = 0; i < n; i++) {
        if (!i || actual != elemento_obtener_valor(arreglo[i])) {
            actual = elemento_obtener_valor(arreglo[i]);
            fprintf(stdout, "%ld:", actual);
        }
        clave = elemento_obtener_clave(arreglo[i]);
        clave[longitud_hasta_caracter_nulo(clave, LONGITUD_USUARIO)] = '\0';
        if (i != n-1 && actual == elemento_obtener_valor(arreglo[i+1])) {
            fprintf(stdout, " %s,", clave);
        } else {
            fprintf(stdout, " %s\n", clave);
        }
    }
}

//Usando el hash de usuarios, genera y ordena el arreglo de elementos, para luego imprimirlos.
void obtener_resultados(hash_t *hash_usuarios) {
    elemento_t **arreglo_elementos = hash_obtener_arreglo_de_elementos(hash_usuarios);
    arreglo_elementos = ordenar_arreglo_de_elementos(arreglo_elementos, hash_cantidad(hash_usuarios), '~', 0, 15);
    imprimir_resultados(arreglo_elementos, hash_cantidad(hash_usuarios));
    destruir_arreglo_elementos(arreglo_elementos, hash_cantidad(hash_usuarios));
}

int main(int arg, char *argv[]) {
    FILE *archivo = fopen(argv[1], "r");
    char *linea = NULL;
    size_t capacidad = LONGITUD_USUARIO, leidos = 0;

    hash_t *hash_usuarios = hash_crear(free),
        *hash_etiquetas = hash_crear(NULL);

    leidos = getline(&linea, &capacidad, archivo);
    while(!feof(archivo)) {
        analizar_linea(linea, hash_usuarios, hash_etiquetas);
        if (!leidos) break;
        leidos = getline(&linea, &capacidad, archivo);
    }
    hash_destruir(hash_etiquetas);
    obtener_resultados(hash_usuarios);
    
    hash_destruir(hash_usuarios);
    free(linea);
    fclose(archivo);
    return 0;
}