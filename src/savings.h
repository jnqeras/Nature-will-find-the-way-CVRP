#ifndef SAVINGS_H
#define SAVINGS_H

typedef unsigned int uint;

struct saving {
    uint nodoA; // ID(nodoA) = nodoA + 1
    uint nodoB; // ID(nodoB) = nodoB + 1
    uint indice_punt_rutas_nodoA; // Índice en punt_rutas_c correspondiente al nodo A (da puntero a la ruta_con_costos que contiene a A)
    uint indice_punt_rutas_nodoB; // Índice en punt_rutas_c correspondiente al nodo B (da puntero a la ruta_con_costos que contiene a B)
    double valor_saving;
};

struct ruta_con_costos {
    list<int> ruta;
    int costo_ruta;
};

bool comparar_savings (saving &s1, saving &s2) {
    return (s1.valor_saving > s2.valor_saving);
} // Usándose en std::sort(), ordena un vector de savings de mayor a menor en base a sus valores.

typedef list<int>* p_rutaL;
typedef ruta_con_costos* p_ruta_c;

#endif //SAVINGS_H
