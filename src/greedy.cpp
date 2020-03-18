#include "main.h"


/* Devuelve el ID del nodo más cercano que no fue visitado 
 * y no eleva la capacidad de la ruta por sobre la del 
 * camión. Si todos los nodos fueron visitados, devuelve 
 * -2. Si hay nodos visitables, pero todos superan la 
 * capacidad, devuelve -1. */
int idNodoMasCercanoVisitable(Graph &graph, int idNodoActual, 
	const unsigned int demandaTotalRuta, 
	vector<bool> visitados){

	/* Inicialización de variables a utilizar. O(1). */
	int res = -1;
	Node nodoActual = graph.node(idNodoActual);
	double menorDistancia = INFINITY;

	/* Elegir el nodo más cercano al nodo actual. O(n). */
	for(unsigned int idNodoNuevo = 1; idNodoNuevo <= graph.amountNodes(); idNodoNuevo++){
		Node nodoNuevo = graph.node(idNodoNuevo);
		if(!visitados[idNodoNuevo - 1]){
			double distanciaActual = distancia(nodoActual, nodoNuevo);
			if((distanciaActual < menorDistancia) && 
				(demandaTotalRuta + graph.demand(idNodoNuevo) <= graph.capacity())){
				res = idNodoNuevo;
				menorDistancia = distanciaActual;
			}
		}
	}

	return res;
}


/* Heurística golosa de resolución para el problema. O(n^2). */
ListaDeRutas greedy(Graph &graph){	
	/* Inicialización de variables a utilizar. O(n). */
	ListaDeRutas listaDeRutas;						// Lista de los índices de los nodos visitados.
	vector<bool> visitados(graph.amountNodes(), false);
	Ruta ruta;
	unsigned int demandaTotalRuta = 0;
	visitados[ID_DEPOSITO - 1] = true;

	/* Elegir el nodo más cercano al depósito. O(n). */
	int idNodoActual = idNodoMasCercanoVisitable(graph, ID_DEPOSITO, 0, visitados);

	/* Recorrer todos los nodos del grafo con uno o más vehículos. O(n^2). */
	unsigned int nodosVisitados = 1;
	while(nodosVisitados < graph.amountNodes()){	// O(n) iteraciones.
		if(idNodoActual < 1){							// Si no puedo añadirlo a la ruta.
			listaDeRutas.push_back(ruta);					// Agrego la ruta.
			/* Limpiar la ruta a usar. O(n). */
			ruta.clear();									// Creo una ruta nueva.
			demandaTotalRuta = 0;							// Reinicio la demanda de la ruta.
			/* Elegir el nodo más cercano al depósito de los no visitados aún. O(n). */
			idNodoActual = idNodoMasCercanoVisitable(graph, ID_DEPOSITO, 0, visitados);
		}else{
			ruta.push_back(idNodoActual);					// Añado el nodo a la ruta.
			demandaTotalRuta += graph.demand(idNodoActual);
			visitados[idNodoActual - 1] = true;
			/* Elegir el nodo más cercano al actual de los no visitados aún. O(n). */
			idNodoActual = idNodoMasCercanoVisitable(graph, idNodoActual, 
				demandaTotalRuta, visitados);				// Continúo desde el nodo más cercano.
			nodosVisitados++;
			if(nodosVisitados == graph.amountNodes()) listaDeRutas.push_back(ruta);			// Agrego la ruta.
		}
	}

	return listaDeRutas;
}