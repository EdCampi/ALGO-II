class _Nodo:
    """
    Nodo básico, unidireccional.
    """
    def __init__(self, dato, prox=None):
        self.dato = dato
        self.prox = prox

class Pila:
    """
    Pila común, LIFO (self.prim es siempre el tope de la pila, lo que entra ultimo, pasa a ser el tope).
    """
    def __init__(self):
        self.prim = None

    def __str__(self):
        cadena = []
        act = self.prim
        i = 0
        while act:
            cadena.append(act.dato)
            act = act.prox
            i += 1
        return str(cadena)
    
    def esta_vacia(self):
        """True si la pila está vacía."""
        return self.prim == None
    
    def apilar(self, elemento):
        """Apila el elemento dado dejandolo en el tope de la pila."""
        elemento = _Nodo(elemento)

        if self.esta_vacia():
            self.prim = elemento
        else:
            elemento.prox = self.prim
            self.prim = elemento

    def desapilar(self):
        """Devuelve el dato que se encontraba en el tope de la pila."""
        if self.esta_vacia():
            #raise Exception('La pila está vacia.')
            return None
        dato_aux = self.prim.dato
        self.prim = self.prim.prox
        return dato_aux

    def ver_tope(self):
        """Muestra el tope de la pila"""
        return self.prim.dato