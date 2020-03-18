#include "main.h"

/* **** K-MEANS CLUSTERING ******************** */

/* Inicializa K puntos aleatorios en el rango de los puntos a clusterizar. O(n+K) */
vector<Node> inicializarMeans(Graph &graph, unsigned int K){
	/* Tiempo inicial para la seed randomizada. O(1). */
	typedef chrono::high_resolution_clock reloj;
  	reloj::time_point inicio = reloj::now();

	/* Buscar el máximo y mínimo valor en cada eje. O(n). */
	vector<Node> nodos = graph.nodes();
	int maxX = nodos[0].x, minX = nodos[0].x;
	int maxY = nodos[0].y, minY = nodos[0].y;
	for(Node nodo : nodos){
		if(nodo.x > maxX) maxX = nodo.x;
		if(nodo.x < minX) minX = nodo.x;
		if(nodo.y > maxY) maxY = nodo.y;
		if(nodo.y < minY) minY = nodo.y;
	}

	/* Generar los K puntos aleatoriamente en el rango encontrado. O(K). */
	reloj::duration duracion = reloj::now() - inicio;
  	unsigned seed = duracion.count();
	default_random_engine generador(seed);
  	uniform_int_distribution<int> distribucionX(minX + 1, maxX - 1);
  	uniform_int_distribution<int> distribucionY(minY + 1, maxY - 1);
  	vector<Node> means;
  	for(unsigned int i = 0; i < K; i++){
  		int randX = distribucionX(generador);
  		int randY = distribucionY(generador);
  		means.push_back(Node(0,randX,randY));
  	}

  	return means;
}


/* Actualiza la posición de una K mean según sus puntos más cercanos. O(1). */
Node actualizarMean(unsigned int cantItemsCluster, Node mean, Node puntoAgregado){
	/* Como se agrega un nodo a la vez, no hace falta recalcular la media con todos 
	 * los nodos. Alcanza con hacer de cuenta que tenemos todos los nodos del cluster 
	 * con posición igual a la media, y calcular el promedio entre esos y el nuevo 
	 * punto agregado. */
	int x = round((mean.x * (cantItemsCluster - 1) + puntoAgregado.x)/cantItemsCluster);
	int y = round((mean.y * (cantItemsCluster - 1) + puntoAgregado.y)/cantItemsCluster);
	return Node(0, x, y);
}


/* Devuelve el índice de la mean más cercana de un punto dado. O(K). */
unsigned int encontrarIndiceDeMean(vector<Node> &means, Node punto){
	double distMin = INFINITY;
	unsigned int indice = 0;

	/* Buscar la mean más cercana al punto. O(K). */
	for(unsigned int i = 0; i < means.size(); i++){
		double dist = distancia(punto, means[i]);
		if(dist < distMin){
			distMin = dist;
			indice = i;
		}
	}

	return indice;
}


/* Calcula las means finales para clusterizar con ellas. O(n*K). */
vector<Node> calcularMeans(Graph &graph, unsigned int K, 
	unsigned int cantidadDeIteraciones){

	/* Inicializar K puntos aleatorios en el rango de los clientes. O(n+K). */
	vector<Node> means = inicializarMeans(graph, K);

	/* Inicializar los clusters. O(K+n) */
	vector<unsigned int> cantItemsCluster(means.size(), 0);
	vector<unsigned int> clusterDeItem(graph.amountNodes(), 0);
	const vector<Node> nodos = graph.nodes();

	/* Total ciclo: O(n*K). */
	/* Itera una cantidad fija de veces, tal que cantidadDeIteraciones es O(1). */
	for(unsigned int x = 0; x < cantidadDeIteraciones; x++){
		bool huboCambio = false;

		/* Encontrar la mean adecuada para cada nodo y la actualiza. Itera n veces. */
		for(unsigned int i = 0; i < nodos.size(); i++){
			Node nodo = nodos[i];

			/* Encontrar el cluster al que pertenece el nodo. O(K). */
			unsigned int indiceMean = encontrarIndiceDeMean(means, nodo);

			/* Actualizo la mean correspondiente. O(1). */
			cantItemsCluster[indiceMean]++;
			means[indiceMean] = actualizarMean(cantItemsCluster[indiceMean], 
				means[indiceMean], nodo);

			/* Revisar si es necesario seguir iterando. O(1). */
			if(indiceMean != clusterDeItem[i]) huboCambio = true;

			/* Actualizar el cluster del nodo. O(1). */
			clusterDeItem[i] = indiceMean;
		}

		/* Revisar si es necesario seguir iterando. O(1). */
		if(!huboCambio) break;
	}

	return means;
}


/* Clusteriza los puntos con el método K-Means. O(n*K). */
vector<vector<Node>> clusterizar(Graph &graph, unsigned int K, 
	unsigned int cantidadDeIteraciones){

	/* Obtener puntos. O(n). */
	vector<Node> puntos = graph.nodes();

	/* Calcular las means finales para los puntos. O(n*K). */
	vector<Node> means = calcularMeans(graph, K, cantidadDeIteraciones);

	/* Crear nuevo vector de means. O(K). */
	vector<vector<Node>> clusters(means.size());

	/* Total ciclo: O(n*K). */
	/* Asignar clusters a cada nodo. Itera n veces. */
	for(Node punto : puntos){
		/* Buscar la mean más cercana al punto. O(K). */
		unsigned int indice = encontrarIndiceDeMean(means, punto);
		clusters[indice].push_back(punto);
	}

	/* Sólo guardaa los clusters no vacíos. Divide aquellos que tengan 
	 * demanda mayor a la capacidad de un camión. Elimina el depósoto de 
	 * los clusters. O(n). */
	vector<vector<Node>> clustersFinales;
	for(unsigned int i = 0; i < clusters.size(); i++){
		if(clusters[i].size() > 0){
			int demandaCluster = 0;
			vector<Node> newCluster;
			for(unsigned int j = 0; j < clusters[i].size(); j++){
				if((unsigned int)(demandaCluster + graph.demand((clusters[i][j]).id)) > graph.capacity()){
					demandaCluster = 0;
					clustersFinales.push_back(newCluster);
					newCluster.clear();
					newCluster.push_back(clusters[i][j]);
					demandaCluster += graph.demand((clusters[i][j]).id);
				}else{
					if(clusters[i][j].id != 1){
						demandaCluster += graph.demand((clusters[i][j]).id);
						newCluster.push_back(clusters[i][j]);
					}
				}
			}
			if(demandaCluster > 0) clustersFinales.push_back(newCluster);
		}
	}

	return clustersFinales;
}

/* ******************************************** */


/* **** NEAREST INSERTION TSP ***************** */

/* Heurística de resolución para el problema TSP con "nearest instertion". O(n^2). */
Ruta nearestInsertion(Graph &graph, vector<Node> &nodos){

	vector<bool> enRuta(nodos.size(), false);

	Ruta ruta;
	ruta.push_back(nodos[0].id);
	enRuta[0] = true;
	double distMin = INFINITY;
	unsigned int indexMin = 1;
	for(unsigned int i = 1; i < nodos.size(); i++){
		double dist = distancia(nodos[0], nodos[i]);
		if(distMin > dist){
			distMin = dist;
			indexMin = i;
		}
	}
	if(ruta.size() > 1){
		ruta.push_back(nodos[indexMin].id);
		enRuta[indexMin] = true;
	}

	for(unsigned int l = 1; l < nodos.size(); l++){
		distMin = INFINITY;
		indexMin = 1;
		unsigned int indexMinRuta = 1;
		for(unsigned int j = 0; j < ruta.size(); j++){
			int idNodo = ruta[j];
			for(unsigned int k = 0; k < nodos.size(); k++){
				if(!enRuta[k]){
					double dist = distancia(graph.node(idNodo), nodos[k]);
					if(distMin > dist){
						distMin = dist;
						indexMin = k;
						indexMinRuta = j;
					}
				}
			}
		}

		unsigned int indexMinVecino = 0;
		if(ruta.size() > 1){
			if(indexMinRuta == 0){
				indexMinVecino = indexMinRuta + 1;
			}else if(indexMinRuta == ruta.size() - 1){
				indexMinVecino = indexMinRuta - 1;
			}else{
				double dA = distancia(
					graph.node(ruta[indexMinRuta]), 
					graph.node(ruta[indexMinRuta + 1])
				);
				double dB = distancia(
					graph.node(ruta[indexMinRuta]), 
					graph.node(ruta[indexMinRuta - 1])
				);
				if(dA < dB){
					indexMinVecino = indexMinRuta + 1;
				}else{
					indexMinVecino = indexMinRuta - 1;
				}
			}
		}

		/* Insertar k entre i y j. O(n) por función insert de vectores. */
		ruta.insert(ruta.begin()+indexMinVecino, nodos[indexMin].id);
		enRuta[indexMin] = true;
	}

	return ruta;
}

/* ******************************************** */


/* Heurística de resolución para el problema con cluster first, 
 * route second alternativos. O(n*K + n^2). */
ListaDeRutas kmeans(Graph &graph){

	ListaDeRutas listaDeRutas;

	/* Calcular la suma total de demandas. O(n). */
	int sumaTotalDeDemandas = 0;
	for(unsigned int id = 1; id <= graph.amountNodes(); id++){
		sumaTotalDeDemandas += graph.demand(id);
	}

	/* Estimar K como la suma total de demandas sobre la capacidad del camión. O(1). */
	unsigned int K = max(sumaTotalDeDemandas/graph.capacity(), (unsigned int)2);
	unsigned int cantidadDeIteraciones = 1000;	// La cantidad de iteraciones será siempre fija y O(1).
	
	/* Clusterizar puntos con K-Means. O(n*K). */
	vector<vector<Node>> clusters = clusterizar(graph, K, cantidadDeIteraciones);

	/* Impresión de clusters de prueba en consola. */
	/*for(unsigned int i = 0; i < clusters.size(); i++){
		cout << "Cluster " << i << ": ";
		for(unsigned int j = 0; j < clusters[i].size(); j++){
			cout << clusters[i][j].x << " " << clusters[i][j].y;
			if(j < clusters[i].size() - 1) cout << ", ";
		}
		cout << endl;
	}*/

	/* Ahora hay O(n) clusters. No necesariamente K, pues se eliminaron los vacíos. */

	/* Cada cluster es abastecible por un camión. Resolverlos como TSPs. O(n^2). */
	for(unsigned int c = 0; c < clusters.size(); c++){
		listaDeRutas.push_back(nearestInsertion(graph, clusters[c]));
	}

	return listaDeRutas;
}