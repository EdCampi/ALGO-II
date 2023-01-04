import random

class Grafo:
    """
    Grafo representado como diccionario de diccionarios.
    """

    def __init__(self, dirigido=False):
        """
        Inicia el grafo y declara si el mismo es dirigido o no.
        """

        self.grafo = {}
        self.grados_entrada = {}
        self.dirigido = dirigido
    
    def __str__(self):
        return str(self.grafo)
    
    def __len__(self):
        return len(self.grafo)

    def __contains__(self, v):
        return v in self.grafo
    
    def __iter__(self):
        return iter(self.grafo)

    def agregar_vertice(self, v):
        """
        Agrega un vértice al grafo.
        """
        if not v in self.grafo:
            self.grafo[v] = {}
    def borrar_vertice(self, v):
        """
        Elimina un vértice del diccionario que representa el grafo 
        y elimina todas sus aparaciones en los demas vértices.
        Si el vértice en cuestión no pertenece al grafo se levantará una excepción.
        """
        try:
            self.grafo.pop(v)
        except:
            pass

        for vertice in self.grafo:
            if v in vertice.keys():
                vertice.pop(v)

    def agregar_arista(self, v, w, peso=1):
        """
        Dados dos vértices agrega una unión entre ellos con el peso indicado.
        En caso de no haber peso, el valor del mismo por defecto es 1.
        """

        self.grafo[v][w] = peso
        if not self.dirigido:
            self.grafo[w][v] = peso

    def borrar_arista(self, v, w):
        """
        Dado los puntos de inicio y final de una arista,
        si la arista dada existe, se elimina la misma del grafo.
        """
        
        del self.grafo[v][w]
        if not self.dirigido:
            del self.grafo[w][v]

    def estan_unidos(self, v, w):
        """
        Determina devolviendo verdadero o falso, si los vértices
        dados están unidos por una arista.
        """

        return w in self.grafo[v]

    def peso_arista(self, v, w):
        """
        Devuelve el peso de la arista, dados sus vertices de ida y llegada.
        """

        return self.grafo[v][w]

    def obtener_vertices(self):
        """
        Devuelve una lista con los vértices del grafo.
        """
        return list(self.grafo.keys())

    def vertice_aleatorio(self):
        """
        Devuelve la clave de un vértice aleatorio.
        """
        vertices = list(self.grafo.keys())
        indice = random.randint(0, len(self.grafo)-1)
        return vertices[indice]
   
    def adyacentes(self, v):
        """
        Devuelve una lista con los vértices adyacentes al dado.
        """

        return list(self.grafo[v].keys())
    