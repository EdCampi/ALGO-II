#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INDICE_INICIAL 0
#define CANTIDAD_INICIAL 0
#define LONGITUD_INICIAL 1

/* Funciones del módulo permitidas:
 - strlen, strchr
 - strcpy/strncpy
 - strdup/strndup
 - snprintf
*/

//Dados un tamaño y una cadena, devuelve una subcadena con el tamaño deseado.
char *substr(const char *str, size_t n) {
	if (n > strlen(str)) { n = strlen(str); }
	char *res =  malloc(sizeof(char) * (n+1));
	if (!res) { return NULL; }
	size_t i = INDICE_INICIAL;
	while (i < n) {
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return res;
}

//Función auxiliar para conseguir la longitud del arreglo que devolverá la función split.
size_t cantidad_de_separaciones(const char *str, char sep, size_t longitud_cadena) {
	size_t i = INDICE_INICIAL, cantidad = CANTIDAD_INICIAL;
	while (i < longitud_cadena) {
		if (str[i] == sep) cantidad++;
		i++;
	}
	return cantidad+1;
}

//Función auxiliar que agrega en una cadena el carácter dado por parámetro junto con carácter de final de línea y aumenta el indice.
void agregar_a_la_cadena(char *cadena, size_t *indice, char caracter) {
	cadena[*indice] = caracter;
	cadena[*indice + 1] = '\0';
	*indice = *indice + 1;
}

//Función auxiliar que arma un arreglo con la respectivas longitudes de las subcadenas de cada elemento del arreglo que devuelve la función split.
void armar_arreglo_longitudes(size_t longitudes[], const char *str, char sep, size_t longitud) {
	size_t i = INDICE_INICIAL,
		indice_subcadena = INDICE_INICIAL,
		longitud_subcadena = CANTIDAD_INICIAL;
	while (i < longitud) {
		if (str[i] == sep) {
			longitudes[indice_subcadena] = longitud_subcadena;
			indice_subcadena++;
			longitud_subcadena = CANTIDAD_INICIAL;
		} else { longitud_subcadena++; }
		i++;
	}
	longitudes[indice_subcadena] = longitud_subcadena;
}

//Realiza la separación de una cadena en subcadenas dado un patrón de separación.
char **split(const char *str, char sep) {
	
	size_t longitud_cadena = strlen(str),
		indice_respuesta = INDICE_INICIAL,
		indice_subcadena = INDICE_INICIAL,
		separaciones = cantidad_de_separaciones(str, sep, longitud_cadena),
		longitudes[separaciones];
	
	armar_arreglo_longitudes(longitudes, str, sep, longitud_cadena);
	
	char **res = calloc(separaciones+2, sizeof(char*));
	if (!res) { return NULL; }
	res[indice_respuesta] = calloc(longitudes[indice_respuesta]+2, sizeof(char));
	for (size_t i = INDICE_INICIAL; i < longitud_cadena; i++) {
		if (str[i] != sep) {
			agregar_a_la_cadena(res[indice_respuesta], &indice_subcadena, str[i]);
		} else {
			indice_subcadena = INDICE_INICIAL;
			indice_respuesta++;
			res[indice_respuesta] = calloc(longitudes[indice_respuesta]+2, sizeof(char));
			res[indice_respuesta][indice_subcadena] = '\0';
		}
	}
	return res;
}

//Une las subacadenas del arreglo dado utilizando el separador dado entre medio de las mismas.
//PRECONDICIÓN: EL arreglo debe finalizar en NULL, tal como el arreglo devuelto por la función "split" hace.
char *join(char **strv, char sep) {
	size_t i = INDICE_INICIAL,
		indice_resultado = INDICE_INICIAL,
		longitud_total = LONGITUD_INICIAL;
	while (strv[i]) {
		longitud_total += (strlen(strv[i])+1);
		i++;
	}
	i = INDICE_INICIAL;
	char *res = calloc(longitud_total, sizeof(char));
	if (longitud_total == LONGITUD_INICIAL) {
		res[indice_resultado] = '\0';
		return res; 
	}
	while (strv[i]) {
		size_t longitud_palabra = strlen(strv[i]);
		for (int j = INDICE_INICIAL; j < longitud_palabra; j++) {
			agregar_a_la_cadena(res, &indice_resultado, strv[i][j]);
		}
		i++;
		if (strv[i] && sep != '\0') {
			res[indice_resultado] = sep;
			indice_resultado++;
		}
	}
	res[indice_resultado] = '\0';
	return res;
}

//Función para destruir el arreglo conseguido mediante la función "split".
void free_strv(char *strv[]) {
	size_t i = INDICE_INICIAL;
	while (strv[i]) {
		free(strv[i]);
		i++;
	}
	free(strv);
}