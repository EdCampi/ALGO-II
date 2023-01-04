#define _POSIX_C_SOURCE 200809L

#include "abb.h"
#include "testing.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

static void prueba_crear_abb_vacio() {
    abb_t *arbol = abb_crear(strcmp, NULL);
    print_test("Se creó un nuevo arbol", arbol);
    print_test("Arbol nuevo está vacío", abb_cantidad(arbol) == 0);
    print_test("Prueba pertenece clave \"1\" es falso", abb_pertenece(arbol, "1") == false);
    print_test("Prueba obtener clave \"1\" es nulo", abb_obtener(arbol, "1") == NULL);
    print_test("Prueba borrar clave \"1\" es nulo", abb_borrar(arbol, "1") == NULL);

    abb_destruir(arbol);
}

static void prueba_iterar_abb_vacio() {
    abb_t * arbol = abb_crear(strcmp, NULL);
    abb_iter_t *iterador = abb_iter_in_crear(arbol);

    print_test("Se creó le iterador", iterador);
    print_test("Prueba el iterador está la final", abb_iter_in_al_final(iterador));
    print_test("Prueba iterador avanzar es falso", abb_iter_in_avanzar(iterador) == false);
    print_test("Prueba iterador ver actual es nulo", abb_iter_in_ver_actual(iterador) == NULL);

    abb_iter_in_destruir(iterador);
    abb_destruir(arbol);
}

static void prueba_abb_guardar() {
    abb_t * arbol = abb_crear(strcmp, NULL);

    char *claveA = "a", *claveB = "b", *claveC = "c", *claveD = "d", *claveE = "e";
    char *dato1 = "uno", *dato2 = "dos", *dato3 = "tres", *dato4 = "cuatro", *dato5 = "cinco";

    //Guarda un solo valor y despues se borra.
    print_test("Guardó la clave \"1\"", abb_guardar(arbol, claveA, dato1));
    print_test("La cantidad es 1", abb_cantidad(arbol) == 1);
    print_test("Obtener clave 1", abb_obtener(arbol, claveA));
    print_test("Pertenece", abb_pertenece(arbol, claveA));
    print_test("Borrar", abb_borrar(arbol, claveA) == dato1);
    print_test("La cantidad es 0", abb_cantidad(arbol) == 0);

    //Vuelvo a guardar
    print_test("Guardo la clave \"C\"", abb_guardar(arbol, claveC, dato3));
    print_test("Guardo la clave \"E\"", abb_guardar(arbol, claveE, dato5));
    print_test("Guardo la clave \"B\"", abb_guardar(arbol, claveB, dato2));
    print_test("Guardo la clave \"A\"", abb_guardar(arbol, claveA, dato1));
    print_test("Guardo la clave \"D\"", abb_guardar(arbol, claveD, dato4));

    print_test("La cantidad es 5", abb_cantidad(arbol) == 5);
    print_test("Pertenece la clave \"A\" al arbol", abb_pertenece(arbol, claveA));
    print_test("Obtener la clave \"A\"", abb_obtener(arbol, claveA) == dato1);
    print_test("Borrar \"A\"", abb_borrar(arbol, claveA) == dato1);
    
    print_test("La cantidad es 4", abb_cantidad(arbol) == 4);
    print_test("Pertenece la clave \"E\" al arbol", abb_pertenece(arbol, claveE));
    print_test("Obtener la clave \"E\"", abb_obtener(arbol, claveE) == dato5);
    print_test("Borrar \"E\"", abb_borrar(arbol, claveE) == dato5);
    
    print_test("La cantidad es 3", abb_cantidad(arbol) == 3);
    print_test("Pertenece la clave \"B\" al arbol", abb_pertenece(arbol, claveB));
    print_test("Obtener la clave \"B\"", abb_obtener(arbol, claveB) == dato2);
    print_test("Borrar \"B\"", abb_borrar(arbol, claveB) == dato2);
    
    print_test("La cantidad es 2", abb_cantidad(arbol) == 2);
    print_test("Pertenece la clave \"C\" al arbol", abb_pertenece(arbol, claveC));
    print_test("Obtener la clave \"C\"", abb_obtener(arbol, claveC) == dato3);
    print_test("Borrar \"C\"", abb_borrar(arbol, claveC) == dato3);
    
    print_test("La cantidad es 1", abb_cantidad(arbol) == 1);
    print_test("Pertenece la clave \"D\" al arbol", abb_pertenece(arbol, claveD));
    print_test("Obtener la clave \"D\"", abb_obtener(arbol, claveD) == dato4);
    print_test("Borrar \"D\"", abb_borrar(arbol, claveD) == dato4);

    print_test("Está vacio", abb_cantidad(arbol) == 0);

    print_test("No pertenece la clave \"A\"", abb_pertenece(arbol, claveA) == false);
    print_test("Obtener la clave \"A\" es nulo", abb_obtener(arbol, claveA) == NULL);
    print_test("Borrar es null", abb_borrar(arbol, claveA) == NULL);
    print_test("No pertenece la clave \"E\"", abb_pertenece(arbol, claveE) == false);
    print_test("Obtener la clave \"E\" es nulo", abb_obtener(arbol, claveE) == NULL);
    print_test("Borrar", abb_borrar(arbol, claveE) == NULL);
    print_test("No ertenece la clave \"B\"", abb_pertenece(arbol, claveB) == false);
    print_test("Obtener la clave \"B\" es nulo", abb_obtener(arbol, claveB) == NULL);
    print_test("Borrar", abb_borrar(arbol, claveB) == NULL);
    print_test("No pertenece la clave \"C\"", abb_pertenece(arbol, claveC) == false);
    print_test("Obtener la clave \"C\" es nulo", abb_obtener(arbol, claveC) == NULL);
    print_test("Borrar", abb_borrar(arbol, claveC) == NULL);
    print_test("No pertenece la clave \"D\"", abb_pertenece(arbol, claveD) == false);
    print_test("Obtener la clave \"D\" es nulo", abb_obtener(arbol, claveD) == NULL);
    print_test("Borrar", abb_borrar(arbol, claveD) == NULL);

    abb_destruir(arbol);
}

static void prueba_abb_reemplazar() {
    abb_t * arbol = abb_crear(strcmp, NULL);

    char *claveA = "a", *claveB = "b", *claveC = "c", *claveD = "d", *claveE = "e";
    char *dato1 = "uno", *dato2 = "dos", *dato3 = "tres", *dato4 = "cuatro", *dato5 = "cinco";
    char *dato1_0 = "UNO", *dato2_0 = "DOS", *dato3_0 = "TRES", *dato4_0 = "CUATRO", *dato5_0 = "CINCO";

    //Vuelvo a insertar
    print_test("Guardo la clave \"C\"", abb_guardar(arbol, claveC, dato3));
    print_test("Guardo la clave \"E\"", abb_guardar(arbol, claveE, dato5));
    print_test("Guardo la clave \"B\"", abb_guardar(arbol, claveB, dato2));
    print_test("Guardo la clave \"A\"", abb_guardar(arbol, claveA, dato1));
    print_test("Guardo la clave \"D\"", abb_guardar(arbol, claveD, dato4));

    print_test("Vuelvo a guardar la clave \"C\" con otro dato", abb_guardar(arbol, claveC, dato3_0));
    print_test("Vuelvo a guardar la clave \"E\" con otro dato", abb_guardar(arbol, claveE, dato5_0));
    print_test("Vuelvo a guardar la clave \"B\" con otro dato", abb_guardar(arbol, claveB, dato2_0));
    print_test("Vuelvo a guardar la clave \"A\" con otro dato", abb_guardar(arbol, claveA, dato1_0));
    print_test("Vuelvo a guardar la clave \"D\" con otro dato", abb_guardar(arbol, claveD, dato4_0));

    print_test("La cantidad sigue siendo 5", abb_cantidad(arbol) == 5);
    print_test("Pertenece la clave \"A\" al arbol", abb_pertenece(arbol, claveA));
    print_test("Obtener la clave \"A\" no es el dato original", abb_obtener(arbol, claveA) != dato1);
    print_test("Obtener la clave \"A\" es el dato reemplazado", abb_obtener(arbol, claveA) == dato1_0);

    print_test("Obtener la clave \"B\" no es el dato original", abb_obtener(arbol, claveB) != dato2);
    print_test("Obtener la clave \"B\" es el dato reemplazado", abb_obtener(arbol, claveB) == dato2_0);

    print_test("Obtener la clave \"C\" no es el dato original", abb_obtener(arbol, claveC) != dato3);
    print_test("Obtener la clave \"C\" es el dato reemplazado", abb_obtener(arbol, claveC) == dato3_0);

    print_test("Obtener la clave \"D\" no es el dato original", abb_obtener(arbol, claveD) != dato4);
    print_test("Obtener la clave \"D\" es el dato reemplazado", abb_obtener(arbol, claveD) == dato4_0);

    print_test("Obtener la clave \"E\" no es el dato original", abb_obtener(arbol, claveE) != dato5);
    print_test("Obtener la clave \"E\" es el dato reemplazado", abb_obtener(arbol, claveE) == dato5_0);

    abb_destruir(arbol);
}

static void prueba_abb_reemplazar_con_destruir() {
    abb_t *arbol = abb_crear(strcmp, free);

    char *clave1 = "AB", *clave2 = "AC";
    char *dato1, *dato2, *dato3;

    dato1 = malloc(sizeof(char)*10);
    dato2 = malloc(sizeof(char)*20);
    dato3 = malloc(sizeof(char)*30);

    print_test("Guardo la primera clave \"AB\"", abb_guardar(arbol, clave1, dato1));
    print_test("Guardo la segunda clave \"AC\"", abb_guardar(arbol, clave2, dato2));
    print_test("Reemplazo la primera clave \"AB\"", abb_guardar(arbol, clave1, dato3));

    abb_destruir(arbol);
}

static void prueba_abb_borrar() {
    abb_t *arbol = abb_crear(strcmp, NULL);
    char *clave1 = "a", *clave2 = "b", *clave3 = "c", *clave4 = "d", *clave5 = "e", *clave6 = "o", *clave7 = "w", *clave8 = "y", *clave9 = "r", *clave10 = "z";
    char *dato1 = "A", *dato2 = "B", *dato3 = "C", *dato4 = "D", *dato5 = "E", *dato6 = "O", *dato7 = "W", *dato8 = "Y", *dato9 = "R", *dato10 = "Z";

    abb_guardar(arbol, clave3, dato3);  /*              C                 */
    abb_guardar(arbol, clave1, dato1);  /*            /   \               */
    abb_guardar(arbol, clave2, dato2);  /*           A     D              */
    abb_guardar(arbol, clave4, dato4);  /*            \     \             */
    abb_guardar(arbol, clave5, dato5);  /*             B     E            */
    abb_guardar(arbol, clave8, dato8);  /*                    \           */
    abb_guardar(arbol, clave9, dato9);  /*                     Y          */
    abb_guardar(arbol, clave10, dato10);/*                    / \         */
    abb_guardar(arbol, clave7, dato7);  /*                   R   Z        */
    abb_guardar(arbol, clave6, dato6);  /*                  / \           */
                                        /*                 O   W          */
    
    print_test("Cantidad es 10", abb_cantidad(arbol) == 10);

    print_test("Pertenece clave C", abb_pertenece(arbol, clave3));
    print_test("Obtener clave C", abb_obtener(arbol, clave3));
    print_test("Borrar clave C", abb_borrar(arbol, clave3));

    print_test("Pertenece clave C", !abb_pertenece(arbol, clave3));
    print_test("Obtener clave C", !abb_obtener(arbol, clave3));
    print_test("Borrar clave C", !abb_borrar(arbol, clave3));

    print_test("Existe A", abb_pertenece(arbol, clave1));
    print_test("Existe D", abb_pertenece(arbol, clave4));
    print_test("Obtener D", abb_obtener(arbol, clave4));

    /*              D                 */
    /*            /   \               */
    /*           A     E              */
    /*            \     \             */
    /*             B     Y            */
    /*                  / \           */
    /*                 R   Z          */
    /*                / \             */
    /*               O   W            */

    print_test("Pertenece clave Y", abb_pertenece(arbol, clave8));
    print_test("Obtener clave Y", abb_obtener(arbol, clave8));
    print_test("Borrar clave Y", abb_borrar(arbol, clave8));

    print_test("No existe Y", !abb_pertenece(arbol, clave8));
    print_test("Existe W", abb_pertenece(arbol, clave7));
    print_test("Existe R", abb_pertenece(arbol, clave9));
    print_test("Existe Z", abb_pertenece(arbol, clave10));
    print_test("Existe O", abb_pertenece(arbol, clave6));

    /*              D                 */
    /*            /   \               */
    /*           A     E              */
    /*            \     \             */
    /*             B     Z            */
    /*                  /             */
    /*                 R              */
    /*                / \             */
    /*               O   W            */

    print_test("Borrar clave W", abb_borrar(arbol, clave7));
    print_test("No existe W", !abb_pertenece(arbol, clave7));
    print_test("Existe R", abb_pertenece(arbol, clave9));
    print_test("Existe Z", abb_pertenece(arbol, clave10));
    print_test("Existe O", abb_pertenece(arbol, clave6));

    /*              D                 */
    /*            /   \               */
    /*           A     E              */
    /*            \     \             */
    /*             B     Z            */
    /*                  /             */
    /*                 R              */
    /*                /               */
    /*               O                */

    print_test("Borrar clave D", abb_borrar(arbol, clave4));
    print_test("No existe D", !abb_pertenece(arbol, clave4));
    print_test("Existe E", abb_pertenece(arbol, clave5));
    print_test("Existe A", abb_pertenece(arbol, clave1));
    print_test("Existe O", abb_pertenece(arbol, clave6));

    /*               E               */
    /*            /     \            */
    /*           A       Z           */
    /*            \     /            */
    /*             B   R             */
    /*                /              */
    /*               0               */

    print_test("Borrar clave A", abb_borrar(arbol, clave1));
    print_test("Existe E", abb_pertenece(arbol, clave5));
    print_test("Existe B", abb_pertenece(arbol, clave2));
    print_test("Existe Z", abb_pertenece(arbol, clave10));

    /*               E               */
    /*            /     \            */
    /*           B       Z           */
    /*                  /            */
    /*                 R             */
    /*                /              */
    /*               0               */


    print_test("Borrar clave Z", abb_borrar(arbol, clave10));
    print_test("Existe R", abb_pertenece(arbol, clave9));
    print_test("Existe B", abb_pertenece(arbol, clave2));
    print_test("Existe O", abb_pertenece(arbol, clave6));

    /*             E           */
    /*            / \          */
    /*           B   R         */
    /*              /          */
    /*             O           */

    print_test("Borrar clave R", abb_borrar(arbol, clave9));
    print_test("No existe R", !abb_pertenece(arbol, clave9));
    print_test("Existe B", abb_pertenece(arbol, clave2));
    print_test("Existe O", abb_pertenece(arbol, clave6));
    print_test("Existe E", abb_pertenece(arbol, clave5));

    /*             E          */
    /*            / \         */
    /*           B   0        */

    print_test("Borrar clave E", abb_borrar(arbol, clave5));
    print_test("No existe E", !abb_pertenece(arbol, clave5));
    print_test("Existe B", abb_pertenece(arbol, clave2));
    print_test("Existe 0", abb_pertenece(arbol, clave6));


    /*            O          */
    /*           /           */
    /*          B            */

    print_test("Borrar clave O", abb_borrar(arbol, clave6));
    print_test("No existe O", !abb_pertenece(arbol, clave6));
    print_test("Existe B", abb_pertenece(arbol, clave2));

    /*          B         */

    print_test("Borrar clave B", abb_borrar(arbol, clave2));
    print_test("No existe B", !abb_pertenece(arbol, clave2));
    print_test("La cantidad es 0", !abb_cantidad(arbol));

    print_test("No existe la clave A", !abb_pertenece(arbol, clave1));
    print_test("No existe la clave B", !abb_pertenece(arbol, clave2));
    print_test("No existe la clave C", !abb_pertenece(arbol, clave3));
    print_test("No existe la clave D", !abb_pertenece(arbol, clave4));
    print_test("No existe la clave E", !abb_pertenece(arbol, clave5));
    print_test("No existe la clave O", !abb_pertenece(arbol, clave6));
    print_test("No existe la clave W", !abb_pertenece(arbol, clave7));
    print_test("No existe la clave Y", !abb_pertenece(arbol, clave8));
    print_test("No existe la clave R", !abb_pertenece(arbol, clave9));
    print_test("No existe la clave Z", !abb_pertenece(arbol, clave10));

    abb_destruir(arbol);
}

static void prueba_abb_clave_vacia() {
    abb_t *arbol = abb_crear(strcmp, NULL);

    char *clave1 = "", *clave2 = " ", *clave3 = "a";
    char *dato1 = "", *dato2 = "b", *dato3 = "c";

    print_test("Guardar clave vacia \"\"", abb_guardar(arbol, clave1, dato1));
    print_test("Existe clave \"\"", abb_pertenece(arbol, clave1));
    print_test("Puedo obtener el valor de \"\"", abb_obtener(arbol, clave1) == dato1);

    print_test("Guardar clave vacia \" \"", abb_guardar(arbol, clave2, dato2));
    print_test("Existe clave \" \"", abb_pertenece(arbol, clave2));
    print_test("Puedo obtener el valor de \" \"", abb_obtener(arbol, clave2) == dato2);

    print_test("Guardar clave no vacia \"a\"", abb_guardar(arbol, clave3, dato3));
    print_test("Existe clave \"a\"", abb_pertenece(arbol, clave3));
    print_test("Puedo obtener el valor de \"a\"", abb_obtener(arbol, clave3) == dato3);

    print_test("Borrar clave \"\"", abb_borrar(arbol, clave1));
    print_test("No existe \"\"", !abb_pertenece(arbol, clave1));

    print_test("Borrar clave \" \"", abb_borrar(arbol, clave2));
    print_test("No existe \" \"", !abb_pertenece(arbol, clave2));

    print_test("Borrar clave \"a\"", abb_borrar(arbol, clave3));
    print_test("No existe \"a\"", !abb_pertenece(arbol, clave3));

    abb_destruir(arbol);
}

static void prueba_abb_valor_null() {
    abb_t *arbol = abb_crear(strcmp, NULL);

    char *clave1 = "", *clave2 = "e", *clave3 = "d";
    char *dato1 = NULL, *dato2 = NULL, *dato3 = NULL;

    print_test("Puedo guardar una clave vacia con dato nulo", abb_guardar(arbol, clave1, dato1));
    print_test("Clave \"\" existe", abb_pertenece(arbol, clave1));
    print_test("Obtener el dato de \"\" es nulo", !abb_obtener(arbol, clave1));

    print_test("Puedo guardar una clave con dato nulo", abb_guardar(arbol, clave2, dato2));
    print_test("Clave \"e\" existe", abb_pertenece(arbol, clave2));
    print_test("Obtener el dato de \"e\" es nulo", !abb_obtener(arbol, clave2));

    print_test("Puedo guardar una clave con dato nulo", abb_guardar(arbol, clave3, dato3));
    print_test("Clave \"d\" existe", abb_pertenece(arbol, clave3));
    print_test("Obtener el dato de \"d\" es nulo", !abb_obtener(arbol, clave3));

    print_test("Borrar la clave vacia devuelve el puntero nulo", !abb_borrar(arbol, clave1));
    print_test("Borrar la clave no vacia \"e\" devuelve el puntero nulo", !abb_borrar(arbol, clave2));
    print_test("Borrar la clave no vacia \"d\" devuelve el puntero nulo", !abb_borrar(arbol, clave3));

    print_test("No pertenece ninguna clave", !abb_pertenece(arbol, clave1) && !abb_pertenece(arbol, clave2) && !abb_pertenece(arbol, clave3));

    abb_destruir(arbol);
}

static void prueba_abb_iterar() {
    abb_t *arbol = abb_crear(strcmp, NULL);
    const char *clave1 = "a", *clave2 = "b", *clave3 = "c", *clave4 = "d", *clave5 = "e", *clave6 = "o", *clave7 = "w", *clave8 = "y", *clave9 = "r", *clave10 = "z";
    char *dato1 = "A", *dato2 = "B", *dato3 = "C", *dato4 = "D", *dato5 = "E", *dato6 = "O", *dato7 = "W", *dato8 = "Y", *dato9 = "R", *dato10 = "Z";

    abb_guardar(arbol, clave3, dato3);  /*              C                 */
    abb_guardar(arbol, clave1, dato1);  /*            /   \               */
    abb_guardar(arbol, clave2, dato2);  /*           A     D              */
    abb_guardar(arbol, clave4, dato4);  /*            \     \             */
    abb_guardar(arbol, clave5, dato5);  /*             B     E            */
    abb_guardar(arbol, clave8, dato8);  /*                    \           */
    abb_guardar(arbol, clave9, dato9);  /*                     Y          */
    abb_guardar(arbol, clave10, dato10);/*                    / \         */
    abb_guardar(arbol, clave7, dato7);  /*                   R   Z        */
    abb_guardar(arbol, clave6, dato6);  /*                  / \           */
                                        /*                 O   W          */
    
    print_test("Cantidad es 10", abb_cantidad(arbol) == 10);

    abb_iter_t *iterador = abb_iter_in_crear(arbol);
    print_test("Se creó el iterador externo", iterador);

    print_test("Iterador no está al final", !abb_iter_in_al_final(iterador));
    print_test("Iterador ver actual es A", !strcmp(abb_iter_in_ver_actual(iterador), clave1));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es B", !strcmp(abb_iter_in_ver_actual(iterador), clave2));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es C", !strcmp(abb_iter_in_ver_actual(iterador), clave3));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es D", !strcmp(abb_iter_in_ver_actual(iterador), clave4));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es E", !strcmp(abb_iter_in_ver_actual(iterador), clave5));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es O", !strcmp(abb_iter_in_ver_actual(iterador), clave6));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es R", !strcmp(abb_iter_in_ver_actual(iterador), clave9));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es W", !strcmp(abb_iter_in_ver_actual(iterador), clave7));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es Y", !strcmp(abb_iter_in_ver_actual(iterador), clave8));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es Z", !strcmp(abb_iter_in_ver_actual(iterador), clave10));
    print_test("No está al final", !abb_iter_in_al_final(iterador));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Está al final", abb_iter_in_al_final(iterador));

    abb_iter_in_destruir(iterador);
    abb_destruir(arbol);
}

static void prueba_abb_iterar_borrando() {
    abb_t *arbol = abb_crear(strcmp, NULL);
    const char *clave1 = "a", *clave2 = "b", *clave3 = "c", *clave4 = "d", *clave5 = "e", *clave6 = "o", *clave7 = "w", *clave8 = "y", *clave9 = "r", *clave10 = "z";
    char *dato1 = "A", *dato2 = "B", *dato3 = "C", *dato4 = "D", *dato5 = "E", *dato6 = "O", *dato7 = "W", *dato8 = "Y", *dato9 = "R", *dato10 = "Z";

    abb_guardar(arbol, clave3, dato3);  /*              C                 */
    abb_guardar(arbol, clave1, dato1);  /*            /   \               */
    abb_guardar(arbol, clave2, dato2);  /*           A     D              */
    abb_guardar(arbol, clave4, dato4);  /*            \     \             */
    abb_guardar(arbol, clave5, dato5);  /*             B     E            */
    abb_guardar(arbol, clave8, dato8);  /*                    \           */
    abb_guardar(arbol, clave9, dato9);  /*                     Y          */
    abb_guardar(arbol, clave10, dato10);/*                    / \         */
    abb_guardar(arbol, clave7, dato7);  /*                   R   Z        */
    abb_guardar(arbol, clave6, dato6);  /*                  / \           */
                                        /*                 O   W          */
    
    print_test("Cantidad es 10", abb_cantidad(arbol) == 10);

    abb_iter_t *iterador = abb_iter_in_crear(arbol);
    print_test("Se creó el iterador externo", iterador);

    print_test("Iterador no está al final", !abb_iter_in_al_final(iterador));
    print_test("Iterador ver actual es A", !strcmp(abb_iter_in_ver_actual(iterador), clave1));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es B", !strcmp(abb_iter_in_ver_actual(iterador), clave2));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es C", !strcmp(abb_iter_in_ver_actual(iterador), clave3));
    print_test("Borro la D", abb_borrar(arbol, clave4));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es E", !strcmp(abb_iter_in_ver_actual(iterador), clave5));
    print_test("Borro la Y", abb_borrar(arbol, clave8));
    print_test("Borro la R", abb_borrar(arbol, clave9));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es O", !strcmp(abb_iter_in_ver_actual(iterador), clave6));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es W", !strcmp(abb_iter_in_ver_actual(iterador), clave7));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Iterador ver actual es Z", !strcmp(abb_iter_in_ver_actual(iterador), clave10));
    print_test("No está al final", !abb_iter_in_al_final(iterador));
    print_test("Avanzar", abb_iter_in_avanzar(iterador));
    print_test("Está al final", abb_iter_in_al_final(iterador));

    abb_iter_in_destruir(iterador);
    abb_destruir(arbol);
}

//Permite que el iterador interno recorra todos los elementos de un árbol, si alguno de estos resulta ser mayor a su
//siguiente elemento (el recorrido no es in order) rompe con el recorrido.
bool visitar_iterador_interno(const char *clave, void *dato, void *extra) {
    printf("Par clave-dato: [%s, %s]\n", clave, (char *)dato);
    if (*(char*)clave >= *(char*)extra) {
        *(char*)extra = *(char*)clave;
        return true;
    }
    return false;
}

static void prueba_abb_iterar_interno(){
    abb_t *arbol = abb_crear(strcmp, NULL);
    const char *clave1 = "a", *clave2 = "b", *clave3 = "c", *clave4 = "d", *clave5 = "e", *clave6 = "o", *clave7 = "w", *clave8 = "y", *clave9 = "r", *clave10 = "z";
    char *dato1 = "A", *dato2 = "B", *dato3 = "C", *dato4 = "D", *dato5 = "E", *dato6 = "O", *dato7 = "W", *dato8 = "Y", *dato9 = "R", *dato10 = "Z";

    abb_guardar(arbol, clave3, dato3);  /*              C                 */
    abb_guardar(arbol, clave1, dato1);  /*            /   \               */
    abb_guardar(arbol, clave2, dato2);  /*           A     D              */
    abb_guardar(arbol, clave4, dato4);  /*            \     \             */
    abb_guardar(arbol, clave5, dato5);  /*             B     E            */
    abb_guardar(arbol, clave8, dato8);  /*                    \           */
    abb_guardar(arbol, clave9, dato9);  /*                     Y          */
    abb_guardar(arbol, clave10, dato10);/*                    / \         */
    abb_guardar(arbol, clave7, dato7);  /*                   R   Z        */
    abb_guardar(arbol, clave6, dato6);  /*                  / \           */
                                        /*                 O   W          */
    
    print_test("Cantidad de elementos en el árbol son 10 [A, B, C, D, E, O, R, W, Y, Z]", abb_cantidad(arbol) == 10);
    
    char extra = *clave1;
    void *puntero_a_centinela = &extra;

    abb_in_order(arbol, visitar_iterador_interno, puntero_a_centinela);
    print_test("El dato extra pasó a tener el valor de la última clave después del recorrido in order", extra == *(char*)clave10);

    abb_destruir(arbol);
}

//Va recorriendo los elementos de un árbol y suma sus valores siempre que el dato de cada elemento y la sumatoria totales sean valores pares.
//De otra manera se corta el recorrido y no se sigue realizando la suma.
bool visitar_solo_pares(const char *clave, void *dato, void *extra) {
    if ((*(int*)dato % 2 == 0) && (*(int*)extra % 2 == 0)) {
        *(int*)extra = *(int*)extra + *(int*)dato;
        printf("Par clave-dato: [%s, %d]. Suma total %d\n", clave, *(int*)dato, *(int*)extra);
        return true;
    }
    return false;
}

static void prueba_abb_iterar_interno_con_corte() {
    abb_t *arbol = abb_crear(strcmp, NULL);
    const char *clave1 = "a", *clave2 = "b", *clave3 = "c", *clave4 = "d", *clave5 = "e", *clave6 = "o", *clave7 = "w", *clave8 = "y", *clave9 = "r", *clave10 = "z";
    int dato1 = 2, dato2 = 2, dato3 = 2, dato4 = 2, dato5 = 2, dato6 = 2, dato7 = 2, dato8 = 2, dato9 = 1, dato10 = 2;

    abb_guardar(arbol, clave3, &dato3);  /*              C                 */
    abb_guardar(arbol, clave1, &dato1);  /*            /   \               */
    abb_guardar(arbol, clave2, &dato2);  /*           A     D              */
    abb_guardar(arbol, clave4, &dato4);  /*            \     \             */
    abb_guardar(arbol, clave5, &dato5);  /*             B     E            */
    abb_guardar(arbol, clave8, &dato8);  /*                    \           */
    abb_guardar(arbol, clave9, &dato9);  /*                     Y          */
    abb_guardar(arbol, clave10, &dato10);/*                    / \         */
    abb_guardar(arbol, clave7, &dato7);  /*                   R   Z        */
    abb_guardar(arbol, clave6, &dato6);  /*                  / \           */
                                         /*                 O   W          */
    
    print_test("Cantidad de valores dentro del arbol son 10 [A, B, C, D, E, O, R, W, Y, Z]", abb_cantidad(arbol) == 10);
    printf("Cada uno tiene el número 2 como valor asociado, menos uno, la \"R\"\n");
    int contador = 0;
    void *puntero_a_contador = &contador;

    print_test("Contador antes de pasar por el recorrido es un valor par", contador % 2 == 0);
    abb_in_order(arbol, visitar_solo_pares, puntero_a_contador);

    print_test("Contador después de pasar por el recorrido es un valor par", contador % 2 == 0);
    print_test("Contador al iterar en los valores aumento a 12, considerando que el punto de corte es la \"R\"", contador == 12);

    abb_destruir(arbol);
}

//Hecho solamente para tratar de "simular" los errores de la prueba cátedra.
static void simular_pruebas_de_la_catedra() {
    abb_t *arbol = abb_crear(strcmp, NULL);
    char *c1 = "1", *c2 = "2", *c3 = "3", *c4 = "4", *c5 = "5", *c6 = "6", *c7 = "7";
    char *dato1 = "HOLA";

    printf("Primera ronda\n");
    //2 1 4 3 5 6 7

    print_test("Pude guardar el dato, clave 2", abb_guardar(arbol, c2, dato1));
    print_test("Pude guardar el dato, clave 1", abb_guardar(arbol, c1, dato1));
    print_test("Pude guardar el dato, clave 4", abb_guardar(arbol, c4, dato1));
    print_test("Pude guardar el dato, clave 3", abb_guardar(arbol, c3, dato1));
    print_test("Pude guardar el dato, clave 5", abb_guardar(arbol, c5, dato1));
    print_test("Pude guardar el dato, clave 6", abb_guardar(arbol, c6, dato1));
    print_test("Pude guardar el dato, clave 7", abb_guardar(arbol, c7, dato1));

    print_test("Pude eliminar el dato, clave 2", abb_borrar(arbol, c2) == dato1);
    print_test("Pude eliminar el dato, clave 1", abb_borrar(arbol, c1) == dato1);
    print_test("Pude eliminar el dato, clave 4", abb_borrar(arbol, c4) == dato1);
    print_test("Pude eliminar el dato, clave 3", abb_borrar(arbol, c3) == dato1);
    print_test("Pude eliminar el dato, clave 5", abb_borrar(arbol, c5) == dato1);
    print_test("Pude eliminar el dato, clave 6", abb_borrar(arbol, c6) == dato1);
    print_test("Pude eliminar el dato, clave 7", abb_borrar(arbol, c7) == dato1);

    printf("Segunda ronda\n");
    //6 5 2 1 3 4 7

    print_test("Pude guardar el dato, clave 6", abb_guardar(arbol, c6, dato1));
    print_test("Pude guardar el dato, clave 5", abb_guardar(arbol, c5, dato1));
    print_test("Pude guardar el dato, clave 2", abb_guardar(arbol, c2, dato1));
    print_test("Pude guardar el dato, clave 1", abb_guardar(arbol, c1, dato1));
    print_test("Pude guardar el dato, clave 3", abb_guardar(arbol, c3, dato1));
    print_test("Pude guardar el dato, clave 4", abb_guardar(arbol, c4, dato1));
    print_test("Pude guardar el dato, clave 7", abb_guardar(arbol, c7, dato1));
    
    print_test("Pude eliminar el dato, clave 2", abb_borrar(arbol, c2) == dato1);
    print_test("Pude eliminar el dato, clave 1", abb_borrar(arbol, c1) == dato1);
    print_test("Pude eliminar el dato, clave 3", abb_borrar(arbol, c3) == dato1);
    print_test("Pude eliminar el dato, clave 4", abb_borrar(arbol, c4) == dato1);
    print_test("Pude eliminar el dato, clave 5", abb_borrar(arbol, c5) == dato1);
    print_test("Pude eliminar el dato, clave 6", abb_borrar(arbol, c6) == dato1);
    print_test("Pude eliminar el dato, clave 6", abb_borrar(arbol, c7) == dato1);

    printf("Tercera ronda\n");
    //7 2 1 3 4 5 6

    print_test("Pude guardar el dato, clave 7", abb_guardar(arbol, c7, dato1));
    print_test("Pude guardar el dato, clave 2", abb_guardar(arbol, c2, dato1));
    print_test("Pude guardar el dato, clave 1", abb_guardar(arbol, c1, dato1));
    print_test("Pude guardar el dato, clave 3", abb_guardar(arbol, c3, dato1));
    print_test("Pude guardar el dato, clave 4", abb_guardar(arbol, c4, dato1));
    print_test("Pude guardar el dato, clave 5", abb_guardar(arbol, c5, dato1));
    print_test("Pude guardar el dato, clave 6", abb_guardar(arbol, c6, dato1));
    
    print_test("Pude eliminar el dato, clave 2", abb_borrar(arbol, c2) == dato1);
    print_test("Pude eliminar el dato, clave 1", abb_borrar(arbol, c1) == dato1);
    print_test("Pude eliminar el dato, clave 4", abb_borrar(arbol, c4) == dato1);
    print_test("Pude eliminar el dato, clave 3", abb_borrar(arbol, c3) == dato1);
    print_test("Pude eliminar el dato, clave 5", abb_borrar(arbol, c5) == dato1);
    print_test("Pude eliminar el dato, clave 6", abb_borrar(arbol, c6) == dato1);
    print_test("Pude eliminar el dato, clave 7", abb_borrar(arbol, c7) == dato1);

    print_test("Árbol vacío", abb_cantidad(arbol) == 0);

    abb_destruir(arbol);
}

void pruebas_abb_estudiante(void) {
    printf("Prueba crear un arbol vacío\n");
    prueba_crear_abb_vacio();
    printf("-------------------------\n");
    printf("Prueba iterar externamente un arbol vacío\n");
    prueba_iterar_abb_vacio();
    printf("-------------------------\n");
    printf("Prueba guardar\n");
    prueba_abb_guardar();
    printf("-------------------------\n");
    printf("Prueba reemplazar\n");
    prueba_abb_reemplazar();
    printf("-------------------------\n");
    printf("Prueba reemplazar con destruir\n");
    prueba_abb_reemplazar_con_destruir();
    printf("-------------------------\n");
    printf("Prueba borrar\n");
    prueba_abb_borrar();
    printf("-------------------------\n");
    printf("Prueba guardar clave vacía\n");
    prueba_abb_clave_vacia();
    printf("-------------------------\n");
    printf("Prueba guardar valor nulo\n");
    prueba_abb_valor_null();
    printf("-------------------------\n");
    printf("Prueba iterar árbol con iterador externo\n");
    prueba_abb_iterar();
    printf("-------------------------\n");
    printf("Prueba iterar árbol con iterador externo mientras borro\n");
    prueba_abb_iterar_borrando();
    printf("-------------------------\n");
    printf("Prueba iterar árbol con iterador interno\n");
    prueba_abb_iterar_interno();
    printf("-------------------------\n");
    printf("Prueba iterar árbol con iterador interno con condición de corte\n");
    prueba_abb_iterar_interno_con_corte();
    printf("-------------------------\n");
    printf("Simular las pruebas fallidas de la cátedra\n");
    simular_pruebas_de_la_catedra();
    printf("-------------------------\n");
}

#ifndef CORRECTOR

int main(void) {
    pruebas_abb_estudiante();
    return 0;
}

#endif