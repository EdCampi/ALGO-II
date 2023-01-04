#!/usr/bin/python3

from os import close
from grafo import *
from biblioteca import *
import sys
import operator

ORIGEN, DESTINO = 1, 2
CANTIDAD = 1
DELINCUENTES, K = 1, 2
N_COMUNIDADES = 1
DELINCUENTE, N_DELINCUENTES = 1, 2

def generar_grafo(archivo):
    """
    Dado un archivo de extension tsv se genera un grafo con
    los recorridos mostrados por el mismo archivo.
    """
    grafo = Grafo(True)
    with open(archivo) as mensajes:
        for linea in mensajes:
            vertices = linea.rstrip().split("\t")
            
            origen = vertices[0].rstrip()
            destino = vertices[1].rstrip()
            grafo.agregar_vertice(origen)
            grafo.agregar_vertice(destino)
            grafo.agregar_arista(origen, destino, abs(int(origen)-int(destino)))
    return grafo

def imprimir_respuesta(arreglo, s, limite=None):
    """
    Imprime el arreglo pasado por parámetro unienodolo con la cadena dada.
    PRE: Dar límite si se quiere imprimir solo hasta un índice en particular del arreglo.
    """
    if not limite: limite = len(arreglo)
    respuesta = s.join(arreglo[:limite])
    print(respuesta)

def min_seguimientos(grafo, delincuente_1, delincuente_2):
    """
    Genera e imprime el camino más rápido para ir de un delincuente a otro.
    """
    camino = reconstruir_camino(grafo, delincuente_1, delincuente_2)
    if camino: 
        imprimir_respuesta(camino, " -> ")
    else:
        print("Seguimiento imposible")

def mas_imp(grafo, cantidad, mas_importantes, imprimir=False):
    """
    Genera una lista de tuplas (donde tupla es del tipo (clave, rango))
    ordenada con todos los delincuentes según si importancia,
    utilizando el alogirtmo de PageRank. Imprime solo las K claves más importantes. 
    """
    if not mas_importantes:
        mas_importantes = pagerank(grafo)
        mas_importantes = sorted(mas_importantes.items(), key=operator.itemgetter(1), reverse=True)
    
    claves = []
    for i in range(cantidad):
        claves.append(mas_importantes[i][0])
    if imprimir:
        imprimir_respuesta(claves, ", ", cantidad)
    return mas_importantes, claves


def calcular_camino_mas_rapido(caminos, grafo, claves):
    """
    Revisa el diccionario de camino y decide cual es el camino más ideoneo para realizar una
    persecución teniendo en cuenta el camino más rápido y el que va a un delincuente de mayor importancia
    en caso de caminos iguales.
    """
    minimo = float("inf")
    delincuente_del_minimo = 0
    for delincuente in caminos:
        rapidez = 0
        if not delincuente or not caminos[delincuente]:
            continue
        for i in range(len(caminos[delincuente])-1):
            rapidez += grafo.peso_arista(caminos[delincuente][i], caminos[delincuente][i+1])
        #Se da solo si la rapidez es menor, o en el caso de ambas iguales, si el delincuente esta antes en el arreglo ue marca las importancias de menor a mayor.
        if rapidez < minimo or (rapidez == minimo and claves.index(caminos[delincuente_del_minimo][-1]) > claves.index(caminos[delincuente][-1])):
            minimo = rapidez
            delincuente_del_minimo = delincuente
    return delincuente_del_minimo

def persecucion(grafo, delincuentes, mas_importantes):
    """
    Calcula el camino más rápido entre varios delincuentes a uno de los K más buscados.
    """
    caminos = {}
    indice = 0
    for importante in mas_importantes:
        for delincuente in delincuentes:
            caminos[indice] = reconstruir_camino(grafo, delincuente, importante)
            indice+=1

    delincuente_a_perseguir = calcular_camino_mas_rapido(caminos, grafo, mas_importantes)
    imprimir_respuesta(caminos[delincuente_a_perseguir], " -> ")
    
def comunidades(grafo, n):
    """
    Detecta e imprime comunidades de delincuentes de largo n, dado un grafo.
    """
    diccionario_de_comunidades = label_propagation(grafo)
    filtrar_e_imprimir_comunidades(diccionario_de_comunidades, n)

def divulgar_rumor(grafo, delincuente, n):
    """
    Permite ver hasta donde puede llegar un rumor en la red de delincuentes, según a cuantas personas (profundidad)
    se transmita el mismo.
    """
    _, ordenes = bfs(grafo, delincuente, n)
    ordenes = list(ordenes.keys())[1::]
    imprimir_respuesta(ordenes, ", ")

def divulgar_ciclo(grafo, delincuente, n):
    """
    Permite encontrar un ciclo donde un delincuente puede divulgar un rumor y hacer que el mismo vuelva a el.
    """
    camino = ciclo_n(grafo, delincuente, n)
    if not camino:
        print("No se encontro recorrido")
    else:
        respuesta = None
        while len(camino)-1:
            respuesta = camino.popleft()
            print(f"{respuesta} -> ", end='')
        respuesta = camino.popleft()
        print(f"{respuesta}")

def cfc(grafo):
    """
    Permite detectar grupos de delincuentes altamente conectados entre sí.
    """
    lista = calcular_cfc(grafo)
    for i in range(len(lista)):
        print(f"CFC {i+1}:", end=' ')
        imprimir_respuesta(lista[i], ", ")

#FUNCIÓN MAIN

def main():
    """
    Función principal que inicia el programa y permite tanto recibir comandos
    por entrada estándar como escribir los mismos. 
    """
    grafo = generar_grafo(sys.argv[1])
    archivo = open(sys.argv[2]) if len(sys.argv) > 2 else sys.stdin
    top_mas_importantes = []
    claves_mas_importantes = []
    for linea in archivo:
        parametros = linea.rstrip().split(" ")
        comando = parametros[0]
        
        if comando == "min_seguimientos":
            min_seguimientos(grafo, parametros[ORIGEN], parametros[DESTINO])
        elif comando == "mas_imp":
            top_mas_importantes, claves_mas_importantes = mas_imp(grafo, int(parametros[CANTIDAD]), top_mas_importantes, True)
        elif comando == "persecucion":
            top_mas_importantes, claves_mas_importantes = mas_imp(grafo, int(parametros[K]), top_mas_importantes)
            persecucion(grafo, parametros[DELINCUENTES].split(','), claves_mas_importantes)
        elif comando == "comunidades":
            comunidades(grafo, int(parametros[N_COMUNIDADES]))
        elif comando == "divulgar":
            divulgar_rumor(grafo, parametros[DELINCUENTE], int(parametros[N_DELINCUENTES].rstrip()))
        elif comando == "divulgar_ciclo":
            divulgar_ciclo(grafo, parametros[DELINCUENTE], int(parametros[N_DELINCUENTES].rstrip()))
        elif comando == "cfc":
            cfc(grafo)

main()