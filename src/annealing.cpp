#include "main.h"
//#include "savings.cpp"
#include <cmath> 
#include <random>

/* Devuelve el vecindario (ver tipo de vecindario) de la solución. */
vector<ListaDeRutas> obtenerVecindario(Graph& graph, ListaDeRutas solucion){
	vector<ListaDeRutas> vecindario;
	//Itero por las rutas de la solucion original
	for (unsigned int i = 0; i < solucion.size(); ++i){
		Ruta rutaActual = solucion[i];
		//Para cada una, itero sobre sus elementos
		for (unsigned int j = 0; j < rutaActual.size(); ++j){
			//Itero sobre las rutas con las que no swapee todavia
			for (unsigned int k = i+1; k < solucion.size(); ++k){
				Ruta otraRuta = solucion[k];
				//Para cada una, itero sobre sus elementos
				for (unsigned int l = 0; l < otraRuta.size(); ++l){
					//Genero una solucion igual a la original
					ListaDeRutas solucionVecina = ListaDeRutas(solucion);
					//Swapeo un par de elementos
					int primerElementoASwapear = solucionVecina[i][j];
					int segundoElementoASwapear = solucionVecina[k][l];
					solucionVecina[i][j] = segundoElementoASwapear;
					solucionVecina[k][l] = primerElementoASwapear;
					//Si es valida la agrego al vecindario
					if (esSolucionFactible(graph, solucionVecina)){
						vecindario.push_back(solucionVecina);
					}
				}
			}
		}
	}
	return vecindario;
}


/* Obtiene algunos parámetros que el paper propone para inicializar la cooling schedule. */
tuple<int,int,int> obtenerParametrosVecindario(Graph &graph, 
	ListaDeRutas& mejorSolucionEncontrada, vector<ListaDeRutas> vecindario){

	/* Calculo el costo de la mejor solución entcontrada. */
	int costo = costoSolucion(graph, mejorSolucionEncontrada);

	/* Calcular la cantidad de vecinos factibles. */
	int nFeas = vecindario.size();		// El vecindario está restringido a soluciones factibles.

	/* Calcular deltaMax y deltaMin. */
	int deltaMin = 0, deltaMax = 0;
	for(ListaDeRutas vecino : vecindario){
		int costoDeVecino = costoSolucion(graph, vecino);
		int delta = abs(costoDeVecino - costo);
		if(delta > deltaMax) deltaMax = delta;
		if(delta < deltaMin) deltaMin = delta;
	}

	return make_tuple(deltaMax, deltaMin, nFeas);
}


/* Metaheurística para CVRP basada en simulated annealing. */
ListaDeRutas annealing(Graph &graph){

	//Genero solución inicial con alguna heurística.
	ListaDeRutas S = savings(graph);

	/* Experimentos. */
	//ListaDeRutas S = greedy(graph);

	//Genero el vecindario inicial de S
	vector<ListaDeRutas> initialNeighbourhood = obtenerVecindario(graph, S);
	
	//2. Inicialización de parámetros de enfriamiento
	// Obtener deltaMax, deltaMin y nFeas.
	tuple<int,int,int> parametros = obtenerParametrosVecindario(graph, S, initialNeighbourhood);

	//cerr << get<0>(parametros) << " " << get<1>(parametros) << " " << get<2>(parametros) << endl;

	/* Experimentos. */
	//tuple<int,int,int> parametros = make_tuple((int)400,(int)0,(int)300);
	//tuple<int,int,int> parametros = make_tuple((int)300,(int)0,(int)300);
	//tuple<int,int,int> parametros = make_tuple((int)400,(int)0,(int)400);
	//tuple<int,int,int> parametros = make_tuple((int)500,(int)0,(int)200);
	//tuple<int,int,int> parametros = make_tuple((int)200,(int)0,(int)500);

	int Ts = get<0>(parametros);
	//int Tf = get<1>(parametros);
	int Tf = round(Ts/5);
	int Tr = Ts;
	int Tk = Ts;
	int Tb = Ts;
	int n = graph.amountNodes() - 1;
	int alfa = get<2>(parametros) * n;
	int gamma = n;
	int maxAmountOfLapsWithoutImproving = 10;
	ListaDeRutas Sb = S;
	int k = 1;
	int amountOfLapsWithoutImproving = 0;

	while (Tk > Tf){
		//Genero el vecindario de S
		vector<ListaDeRutas> neighbourhoodS = obtenerVecindario(graph, S);

		//Elijo al azar un vecino
		int neighbourToGet = rand() % neighbourhoodS.size();
		ListaDeRutas SBis = neighbourhoodS[neighbourToGet];

		//Calculo el delta
		int delta = abs(costoSolucion(graph, SBis) - costoSolucion(graph, S));
	
		std::default_random_engine generator;
	    std::uniform_real_distribution<double> distribution(0.0,1.0);
	    double tita = 0;
	    while (tita == 0){
	        tita = distribution(generator);
		}

		//Si la función de aceptación da positivo
		if(pow(exp(1),-delta/Tk) >= tita){
			//Acepto la solución
			S = SBis;
			//Si el costo de la solución es menor al mejor hasta ahora
			if(costoSolucion(graph, S) < costoSolucion(graph, Sb)){
				//Reemplazo el mejor
				Sb = S;
				Tb = Tk;
				//Reinicio la cantidad de vueltas sin mejoras
				amountOfLapsWithoutImproving = 0;
			} else {
				//Aumento la cantidad de vueltas sin mejoras
				amountOfLapsWithoutImproving++;
			}
			//Actualizo la temperatura en caso de haber aceptado
			double gammaPorRaizK = gamma * sqrt(k);
			double denominador1 = alfa + gammaPorRaizK;
			double denominador2 = denominador1*Ts*Tf;
			double betaK = (Ts - Tf) / denominador2;
			Tk = Tk / (1 + betaK * Tk);
		} else {
			//Actualizo la temperatura en caso de no haber aceptado
			Tr = max(Tr/2,Tb);
			Tk = Tr;
		}
		//Actualizo k
		k = k + 1;

		//Si corresponde, vuelvo a la mejor solución hasta ahora para seguir por otro camino 
		if(amountOfLapsWithoutImproving == maxAmountOfLapsWithoutImproving){
			amountOfLapsWithoutImproving = 0;
			S = Sb;
		}
	}
	return Sb;
}