#include "main.h"
#include "savings.h"

// Cuando indique que "Acá i representa X" me refiero a que en el container
// que se completa/construye en el ciclo a continuación, la posición i-ésima
// corresponderá a una entidad asociada a X.

// Ej.: "rutas[i]" dará incialmente (tras el primer ciclo) la ruta que va y viene
// (inmediatamente) del depot al nodo i+1 ({ID(depot),ID(nodo i+1),ID(depot)}).
// Por ende, en el vector "rutas" se dice que "i representa al nodo i+1".

ListaDeRutas savings(Graph &graph) {
    uint n = graph.amountNodes();

    vector<ruta_con_costos> rutas_c(n-1);
    vector<p_ruta_c> punt_rutas_c(n-1); // Podría haber usado un diccionario.
    // Se completan los valores iniciales de "rutas_c" y "punt_rutas_c":

    for (uint i = 0; i < n-1; ++i) { // Acá i representa al nodo i+1
        rutas_c[i] = { {(int) i + 2}, graph.demands()[i+1]}; // El segundo elemento es la demanda de la ruta (equivalente a la demanda del nodo i+1, el depot no tiene demanda).
        // {i+2} = {ID(nodo i+1)} = {ID(depot), ID(nodo i+1), ID(depot)} es la ruta que va desde el depot al nodo i+1 y vuelve (los depots quedan implícitos en la representación).
        punt_rutas_c[i] = &rutas_c[i];
    }

    vector<vector<double>> distancias(n, vector<double>(n,0));
    // Se completa la matriz de distancias:
    for (uint i = 0; i < n; ++i) { // Acá i representa al nodo i
        for (uint j = i+1; j < n; ++j) {
            distancias[i][j] = distancia(graph.nodes()[i],graph.nodes()[j]);
        }
    }

    // Se construye el vector de savings:
    vector<saving> savings;
    for (uint i = 0; i < n-1; ++i) { // Acá i representa al nodo i+1
        for (uint j = i+1; j < n-1; ++j) {
            // En "distancias" i representa al nodo i (en lugar de i+1), por lo que se le suma 1 al índice para ir al nodo i+1 que se necesita:
            double valor_saving = distancias[0][i+1] + distancias[0][j+1] - distancias[i+1][j+1];
            savings.push_back({i+1,j+1,i,j,valor_saving});
        }
    }

    // Se ordena "savings" de mayor a menor:
    sort(savings.begin(), savings.end(), comparar_savings);

    for (auto &saving : savings) {
        uint indiceA = saving.indice_punt_rutas_nodoA, indiceB = saving.indice_punt_rutas_nodoB;
        int ID_nodoA = saving.nodoA + 1, ID_nodoB = saving.nodoB + 1;
        p_rutaL rutaA = &punt_rutas_c[indiceA]->ruta, rutaB = &punt_rutas_c[indiceB]->ruta;
        int costo_rutas_fusionadas = punt_rutas_c[indiceA]->costo_ruta + punt_rutas_c[indiceB]->costo_ruta;

        bool en_rutas_separadas = (rutaA != rutaB);
        bool no_excede_capacidad = ((uint) costo_rutas_fusionadas <= graph.capacity());

        if (en_rutas_separadas && no_excede_capacidad) {
            auto extremo_der_rutaA = --rutaA->end(), extremo_izq_rutaA = rutaA->begin();
            auto extremo_der_rutaB = --rutaB->end(), extremo_izq_rutaB = rutaB->begin(); // Todas estas variables son iteradores.

            // if (*extremo_der_rutaA == ID_nodoA && *extremo_izq_rutaB == ID_nodoB) no se hace nada.
            if (*extremo_der_rutaA == ID_nodoA && *extremo_der_rutaB == ID_nodoB) { rutaB->reverse(); } // rutaB = {...,ID(B)} -> {ID(B),...}}
            else if (*extremo_izq_rutaA == ID_nodoA && *extremo_izq_rutaB == ID_nodoB) { rutaA->reverse(); } // rutaA = {ID(A),...} -> {...,ID(A)}}
            else if (*extremo_izq_rutaA == ID_nodoA && *extremo_der_rutaB == ID_nodoB) { swap(rutaA,rutaB), swap(indiceA,indiceB); }
            else goto fin_ciclo; // Considered harmful.
            // Si no se cumple ninguna de las 4 condiciones de arriba, no se hace nada (se finaliza
            // la iteración corriente del ciclo) porque no se pueden unir las rutas como precisa el saving.

            rutaA->splice(rutaA->end(), *rutaB);
            // rutaA = {...,ID(A)} + {ID(B),...} = {...,ID(A),ID(B),...}, rutaB = vacía.
            // Ahora B está en rutaA. Actualizo punteros a ruta del nodo B:
            punt_rutas_c[indiceB] = punt_rutas_c[indiceA];
            // Actualizo el costo de las ruta (que contiene ahora a los nodos A y B) como el de la fusión de las rutas:
            punt_rutas_c[indiceA]->costo_ruta = costo_rutas_fusionadas;
        }
        fin_ciclo: ;
    }

    ListaDeRutas rutas_resultado;
    for (auto &ruta_c  : rutas_c) {
        auto &rutaL = ruta_c.ruta;
        if (rutaL.size() != 0) {
            Ruta ruta_vectorizada;
            ruta_vectorizada.reserve(rutaL.size());
            ruta_vectorizada.insert(ruta_vectorizada.end(), rutaL.begin(), rutaL.end());
            rutas_resultado.push_back(ruta_vectorizada);
        }
    }

    return rutas_resultado;
}
