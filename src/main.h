#ifndef MAIN_H
#define MAIN_H

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>
#include <random>

using namespace std;


/* **** DEFINICIONES ************************** */

#define ID_DEPOSITO         1
#define Ruta                vector<int>
#define ListaDeRutas        vector<Ruta>

/* ******************************************** */


/* **** CLASES ******************************** */

class Node {
    public:
        int id;                                 // Id del nodo
        int x;                                  // Posición del nodo en el eje X.
        int y;                                  // Posición del nodo en el eje Y.

        Node(int id, int posX, int posY);               // Construye un nodo en la posición dada.
        bool operator==(const Node& nodeB);     // Sobrecarga de operador de igualdad.
        bool operator!=(const Node& nodeB);     // Sobrecarga de operador de desigualdad.
};

class Graph {
    private:
        vector<Node> _nodes;                    // Lista de nodos.
        vector<int> _demands;                   // Lista de demandas.
        unsigned int _capacity;                 // Capacidad de los camiones.

    public:
        Graph(vector<Node> nodos,
            vector<int> demandas,
            unsigned int capacidad);            // Construye un grafo completo con un conjunto de nodos y demandas dado.
        unsigned int amountNodes();             // Devuelve la cantidad de nodos del grafo.
        void addNode(Node node, int demand);    // Añade un nodo con la posición y la demanda especificada al grafo.
        void removeNode(int nodeID);            // Elimina del grafo el nodo con el ID dado.
        int demand(int nodeID);                 // Devuelve la demanda del nodo con el ID dado.
        vector<Node> nodes();                   // Devuelve la lista de nodos del grafo.
        Node node(int nodeID);                  // Devuelve la posición de un nodo con el ID dado.
        vector<int> demands();                  // Devuelve la lista de demandas del grafo.
        unsigned int capacity();                // Devuelve la capacidad de los camiones.
};

typedef ListaDeRutas (*cvrp_heur_ptr)(Graph &); // Puntero a función (heurística) que aproxima solución óptima de CVRP para un grafo pasado por referencia.

/* ******************************************** */


/* **** FUNCIONES ***************************** */

double distancia(Node nodoA, Node nodoB);

ListaDeRutas greedy(Graph &graph);

ListaDeRutas savings(Graph &graph);

ListaDeRutas sweep(Graph &graph);

ListaDeRutas kmeans(Graph &graph);

ListaDeRutas annealing(Graph &graph);

int costoSolucion(Graph &graph, ListaDeRutas solucion);

bool esSolucionFactible(Graph &graph, ListaDeRutas candidataASolucion);

/* ******************************************** */

#endif //MAIN_H
