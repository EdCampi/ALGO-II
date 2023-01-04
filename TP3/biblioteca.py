from collections import *
from grafo import *
from operator import *
from pila import *
import operator
import random
import sys

NO_HAY_DISTANCIA_ESTABLECIDA = -1
COEFICIENTE_AMORTIGUACION = 0.1
PAGERANK_ITERACIONES = 20
LABEL_PROPAGATION_ITERACIONES = 20

sys.setrecursionlimit(2000)

def bfs(grafo, origen, distancia_max=NO_HAY_DISTANCIA_ESTABLECIDA):
    """
    Dado un grafo la función recorre el mismo de manera BFS 
    teniendo en cuenta las distancias para sacar los caminos mínimos.
    """
    visitados = set()
    padres = {}
    orden = {}
    orden[origen] = 0
    padres[origen] = None
    visitados.add(origen)

    cola = deque()
    cola.append(origen)

    while len(cola):
        vertice = cola.popleft()
        if distancia_max == orden[vertice]:
            return padres, orden
        for adyacente in grafo.adyacentes(vertice):
            if adyacente in visitados: continue
            padres[adyacente] = vertice
            orden[adyacente] = orden[vertice] + 1
            visitados.add(adyacente)
            cola.append(adyacente)
    return padres, orden

def reconstruir_camino(grafo, origen, destino):
    """
    Realiza una construcción del camino minimo a partir del diccionario de padres
    conseguido a través del recorrido BFS.
    """
    if not origen in grafo or not destino in grafo:
        return None
    padres, _ = bfs(grafo, origen, destino)
    camino = []
    actual = destino
    if not destino in padres: return None
    while actual != origen:
        camino.append(actual)
        actual = padres[actual]
    camino.append(actual)
    return camino[::-1] if (len(camino) or (origen in camino and destino in camino)) > 1 else None

def pagerank(grafo):
    """
    Genera una aproximación a la centralidad de cada vértice usando el algoritmo de PageRank.
    El mismo hecho de forma iterativa aplicacando sobre cada vértice la fórmula de PageRank.
    Formula:
        PR(v) = (1-d)/N + d*(sum(PR(y)/L(y)))
    donde:
        PR(v) = PageRank del vértice en cuestión.
        d = Es el coeficiente de amortiguación.
        N = Es la cantidad de vértices presentes en el grafo.
        y = Son los vértices que apuntan hacia v.
        L(y) = Cantidad de aristas salientes de y.
    """
    #Genero la primer aproximación del PageRank, además de los diccionarios de entrada.
    estado_actual = {}
    vertices_de_entrada = {}
    for vertice in grafo:
        for adyacente in grafo.adyacentes(vertice):
            if not adyacente in vertices_de_entrada: vertices_de_entrada[adyacente] = []
            vertices_de_entrada[adyacente].append(vertice)
        estado_actual[vertice] = (1-COEFICIENTE_AMORTIGUACION)/len(grafo)

    #Genero un orden aleatorio para iterar en el grafo.
    vertices_aleatorios = random.sample(grafo.obtener_vertices(), k=len(grafo))
    for _ in range(PAGERANK_ITERACIONES):
        estado_anterior = estado_actual.copy()
        for vertice in vertices_aleatorios:
            segundo_termino = 0

            entrantes = vertices_de_entrada[vertice] if vertice in vertices_de_entrada else []
            for entrante in entrantes:
                segundo_termino += estado_anterior[entrante]/len(grafo.adyacentes(entrante))
            #Fórmula de PageRank
            estado_actual[vertice] = (1-COEFICIENTE_AMORTIGUACION)/len(grafo) + (COEFICIENTE_AMORTIGUACION*segundo_termino)
    return estado_actual

def max_freq(vecinos):
    """
    Dado un set revisa cual es mas repetido de todos los valores.
    """
    apariciones = {}
    for label in vecinos:
        if not label in apariciones:
            apariciones[label] = 1
        else:
            apariciones[label] += 1
    apariciones = sorted(apariciones.items(), key=operator.itemgetter(1), reverse=True)
    return apariciones[0][0]

def filtrar_e_imprimir_comunidades(comunidades, n):
    """
    Dado un diccionario de comunidades la función imprime, revisando si esta
    cumple o no con la longitud buscada, la comunidad ennumerandola.  
    """
    comunidad_nro = 1
    for comunidad in comunidades:
        if len(comunidades[comunidad]) >= n:
            respuesta = ", ".join(comunidades[comunidad])
            print(f"Comunidad {comunidad_nro}: {respuesta}") 
            comunidad_nro += 1

def label_propagation(grafo):
    """
    Detecta conmunidades usando el algoritmo de Label Propagation.
    El punto de convergencia del algoritmo está dada por una cantidad fija de iteraciones.
    """
    #Primer paso, dar a los labels los indices respectivos.
    labels = {}
    contador = 0
    vecinos_de_entrada = {}
    for vertice in grafo:
        for adyacente in grafo.adyacentes(vertice):
            if not adyacente in vecinos_de_entrada:
                vecinos_de_entrada[adyacente] = set(vertice)
            else:
                vecinos_de_entrada[adyacente].add(vertice)
        labels[vertice] = contador
        contador += 1

    for _ in range(LABEL_PROPAGATION_ITERACIONES): #Paso 2 incio la iteración
        #Genero (al caer en el paso 3, t=1) y voy actualizando el orden aleatorio al volver por el paso 5 (t = t+1).
        orden_aleatorio = random.choices(grafo.obtener_vertices(), k=len(grafo))
        for vertice in orden_aleatorio:
            labels_vecinos = []
            if not vertice in vecinos_de_entrada: continue
            for v in grafo.adyacentes(vertice):
                labels_vecinos.append(labels[v])
            if not len(labels_vecinos): continue
            #Paso 4, aplico la función en cada vértice hasta que el numero de iteraciones sea suficiente como para que la función llegue a un punto de convergencia.
            labels[vertice] = max_freq(labels_vecinos)
    comunidades = {}
    for vertice, label in labels.items():
        if not label in comunidades:
            comunidades[label] = [vertice]
        else:
            comunidades[label].append(vertice)
        
    return comunidades

def ciclo_n(grafo, vertice, n):
    """
    Genera un ciclo simple con n vértices.
    """
    if n < 1 or n > len(grafo): return None
    visitados = set()
    camino = deque()
    camino.append(vertice)

    if n == 1 and vertice in grafo.adyacentes(vertice):
        camino.append(vertice)
        return camino
    if n == 1 and not vertice in grafo.adyacentes(vertice): return None
    for adyacente in grafo.adyacentes(vertice):
        if _ciclo_n(grafo, adyacente, n, visitados, vertice, camino): return camino
    return None

def _ciclo_n(grafo, vertice, n, visitados, origen, camino):
    """
    Función auxiliar donde por backtraking ayuda a generar el ciclo más ideoneo.
    """
    if vertice == origen:
        return False
    if n <= 0 or (n == 1 and int(origen) < int(vertice)):
        return False
    if n == 1 and not origen in grafo.adyacentes(vertice):
        return False
    if n == 1 and vertice == origen:
        return True
    
    visitados.add(vertice)
    camino.append(vertice)

    for adyacente in grafo.adyacentes(vertice):
        if adyacente in visitados:
            continue
        if n == 3 and origen in grafo.adyacentes(adyacente):
            camino.append(adyacente)
            camino.append(origen)
            return True
        if _ciclo_n(grafo, adyacente, n-1, visitados, origen, camino):
            return True
    visitados.remove(vertice)
    camino.pop()
    return False

def calcular_cfc(grafo):
    """
    Mediante in recorrido similar a DFS calcula las componentes fuertemente conexas presentes en el grafo.
    """
    visitados = set()
    orden = {}
    mas_bajo = {}
    pila = Pila()
    apilados = set()
    en_cfs = set()
    todas_cfc = []

    for vertice in grafo:
        if vertice not in visitados:
            orden_n = 0
            tarjan_cfc(grafo, vertice, orden, orden_n, pila, mas_bajo, visitados, todas_cfc, apilados, en_cfs)
    return todas_cfc

def tarjan_cfc(grafo, vertice, orden, orden_n, pila, mas_bajo, visitados, todos_cfc, apilados, en_cfs):
    """
    Usando el algoritmo de tarjan detecta las componentes fuertemente conexas presentes en el grafo.
    """
    orden[vertice] = orden_n
    mas_bajo[vertice] = orden[vertice]
    visitados.add(vertice)
    pila.apilar(vertice)
    apilados.add(vertice)

    for adyacente in grafo.adyacentes(vertice):
        if adyacente not in visitados:
            tarjan_cfc(grafo, adyacente, orden, orden_n+1, pila, mas_bajo, visitados, todos_cfc, apilados, en_cfs)
            orden_n += 1
        elif adyacente in en_cfs:
            continue
        if adyacente in apilados:
            mas_bajo[vertice] = min(mas_bajo[vertice], mas_bajo[adyacente])
        
    if orden[vertice] == mas_bajo[vertice] and not pila.esta_vacia():
        nueva_cfc = []
        vertice_aux = None
        while vertice_aux != vertice:
            vertice_aux = pila.desapilar()
            apilados.remove(vertice_aux)
            nueva_cfc.append(vertice_aux)
            en_cfs.add(vertice_aux)
        todos_cfc.append(nueva_cfc)