#include <utility> 
#include "main.h"
#include "math.h"

Ruta greedyAux(Graph graph){
    Ruta route;
    vector<pair<Node,int>> distancesFromDepot;
    Node depot = graph.nodes()[0];
    for (Node n : graph.nodes()){
        double dx = n.x - depot.x;
        double dy = n.y - depot.y;
        distancesFromDepot.emplace_back(make_pair(n, sqrt(dx * dx + dy * dy)));
    }

    sort(distancesFromDepot.begin(), distancesFromDepot.end(), [](const pair<Node, double> & a, const pair<Node,double> & b) -> bool{
        return a.second < b.second;
    });

    for (unsigned int i = 1; i < distancesFromDepot.size(); ++i){
        pair<Node, double> p = distancesFromDepot[i];
        route.emplace_back(p.first.id);
    }

    return route;
}

ListaDeRutas sweep(Graph &graph){
	//Calculo los angulos polares respecto al depot
	vector<pair<Node, double>> angles = vector<pair<Node,double>>();
	Node depot = graph.nodes()[0];
	for (unsigned int i = 1; i < graph.amountNodes(); ++i){
		Node n = graph.nodes()[i];
		int xDistance = n.x - depot.x;
		int yDistance = n.y - depot.y;
		double angulo = atan2(yDistance, xDistance) * 180 / M_PI;
		angles.emplace_back(make_pair(n, angulo));
	}

	//Ordeno según el ángulo
	sort(angles.begin(), angles.end(), [](const pair<Node, double> & a, const pair<Node,double> & b) -> bool{ 
    	return a.second < b.second;
	});

	//Separo en clusters de nodos por ángulo
    vector<Graph> clusters;
    int routeDemand = 0;
    vector<Node> route;
    vector<int> routeDemands;
    for (unsigned int i = 0; i < angles.size(); ++i){
    	Node node = angles[i].first;
    	int nodeDemand = graph.demands()[node.id-1];
    	if ((unsigned int)(routeDemand + nodeDemand) > graph.capacity()){
    	    route.emplace(route.begin(), depot);
    	    routeDemands.emplace(routeDemands.begin(), 0);
    		Graph cluster = Graph(route, routeDemands, graph.capacity());
    		clusters.push_back(cluster);
    		route.clear();
    		routeDemands.clear();
            routeDemand = 0;
        }
        route.push_back(node);
        routeDemands.push_back(nodeDemand);
        routeDemand+= nodeDemand;
    }
    if (!route.empty()){
        route.emplace(route.begin(), depot);
        routeDemands.emplace(routeDemands.begin(), 0);
    	Graph cluster = Graph(route, routeDemands, graph.capacity());
    	clusters.push_back(cluster);
        route.clear();
        routeDemands.clear();
        routeDemand = 0;
    }

	//Para cada cluster llamo a un TSP
    ListaDeRutas res;
	for (Graph c : clusters){
		Ruta clusterRes = greedyAux(c);
		res.emplace_back(clusterRes);
	}
	return res;
}

