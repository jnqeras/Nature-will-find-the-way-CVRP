#include "main.h"


/* **** INSTANCIAS ****************************************************** */

vector<string> instancias = {   "instancias/Vrp-Set-A/A\\A-n32-k5.vrp",
                                "instancias/Vrp-Set-A/A\\A-n33-k5.vrp",
                                "instancias/Vrp-Set-A/A\\A-n34-k5.vrp",
                                "instancias/Vrp-Set-A/A\\A-n36-k5.vrp",
                                "instancias/Vrp-Set-A/A\\A-n37-k5.vrp",
                                "instancias/Vrp-Set-A/A\\A-n38-k5.vrp",
                                "instancias/Vrp-Set-A/A\\A-n39-k5.vrp",
                                "instancias/Vrp-Set-A/A\\A-n44-k6.vrp",
                                "instancias/Vrp-Set-A/A\\A-n45-k6.vrp",
                                "instancias/Vrp-Set-A/A\\A-n46-k7.vrp",
                                "instancias/Vrp-Set-A/A\\A-n48-k7.vrp",
                                "instancias/Vrp-Set-A/A\\A-n53-k7.vrp",
                                "instancias/Vrp-Set-A/A\\A-n54-k7.vrp",
                                "instancias/Vrp-Set-A/A\\A-n55-k9.vrp",
                                "instancias/Vrp-Set-A/A\\A-n60-k9.vrp",
                                "instancias/Vrp-Set-A/A\\A-n61-k9.vrp",
                                "instancias/Vrp-Set-A/A\\A-n62-k8.vrp",
                                "instancias/Vrp-Set-A/A\\A-n63-k9.vrp",
                                "instancias/Vrp-Set-A/A\\A-n64-k9.vrp",
                                "instancias/Vrp-Set-A/A\\A-n65-k9.vrp",
                                "instancias/Vrp-Set-A/A\\A-n69-k9.vrp",
                                "instancias/Vrp-Set-A/A\\A-n80-k10.vrp" };

vector<string> instancias2 = {  "instancias/Vrp-Set-B/B\\B-n31-k5.vrp",
                                "instancias/Vrp-Set-B/B\\B-n34-k5.vrp",
                                "instancias/Vrp-Set-B/B\\B-n35-k5.vrp",
                                "instancias/Vrp-Set-B/B\\B-n38-k6.vrp",
                                "instancias/Vrp-Set-B/B\\B-n39-k5.vrp",
                                "instancias/Vrp-Set-B/B\\B-n41-k6.vrp",
                                "instancias/Vrp-Set-B/B\\B-n43-k6.vrp",
                                "instancias/Vrp-Set-B/B\\B-n44-k7.vrp",
                                "instancias/Vrp-Set-B/B\\B-n45-k5.vrp",
                                "instancias/Vrp-Set-B/B\\B-n50-k7.vrp",
                                "instancias/Vrp-Set-B/B\\B-n51-k7.vrp",
                                "instancias/Vrp-Set-B/B\\B-n52-k7.vrp",
                                "instancias/Vrp-Set-B/B\\B-n56-k7.vrp",
                                "instancias/Vrp-Set-B/B\\B-n57-k7.vrp",
                                "instancias/Vrp-Set-B/B\\B-n63-k10.vrp",
                                "instancias/Vrp-Set-B/B\\B-n64-k9.vrp",
                                "instancias/Vrp-Set-B/B\\B-n66-k9.vrp",
                                "instancias/Vrp-Set-B/B\\B-n67-k10.vrp",
                                "instancias/Vrp-Set-B/B\\B-n68-k9.vrp",
                                "instancias/Vrp-Set-B/B\\B-n78-k10.vrp" };

/* ********************************************************************** */

/* **** NODOS *********************************************************** */

/* Construye un nodo en la posición dada. */
Node::Node(int id, int posX, int posY){
    this->id = id;
    this->x = posX;
    this->y = posY;
}


/* Sobrecarga de operador de igualdad. */
bool Node::operator==(const Node& nodeB){
    bool condition1 = (this->x == nodeB.x);
    bool condition2 = (this->y == nodeB.y);
    return condition1 && condition2;
}


/* Sobrecarga de operador de desigualdad. */
bool Node::operator!=(const Node& nodeB){
    bool condition1 = (this->x != nodeB.x);
    bool condition2 = (this->y != nodeB.y);
    return condition1 || condition2;
}

/* ********************************************************************** */


/* **** GRAFOS *********************************************************** */

/* Construye un grafo completo con un conjunto de nodos dado. */
Graph::Graph(vector<Node> ns, vector<int> ds, unsigned int capacidad){
    if(ns.size() != ds.size()){
        cerr << "[ERROR] GRAFO CREADO CON DIFERENTE CANTIDAD DE NODOS Y DEMANDAS." << endl;
    }
    this->_nodes = ns;
    this->_demands = ds;
    this->_capacity = capacidad;
}


/* Devuelve la cantidad de nodos del grafo. */
unsigned int Graph::amountNodes(){
    return this->_nodes.size();
}


/* Añade un nodo al grafo. */
void Graph::addNode(Node node, int demand){
    this->_nodes.push_back(node);
    this->_demands.push_back(demand);
}


/* Elimina del grafo el nodo con el ID dado. */
void Graph::removeNode(int nodeID){
    unsigned int index = nodeID - 1;
    (this->_nodes).erase((this->_nodes).begin() + index);
    (this->_demands).erase((this->_demands).begin() + index);
}


/* Devuelve la demanda del nodo con el ID dado. */
int Graph::demand(int nodeID){
    unsigned int index = nodeID - 1;
    return (this->_demands)[index];
}


/* Devuelve la lista de nodos del grafo. */
vector<Node> Graph::nodes(){
    return this->_nodes;
}


/* Devuelve la posición de un nodo con el ID dado. */
Node Graph::node(int nodeID){
    unsigned int index = nodeID - 1;
    return (this->_nodes)[index];
}


/* Devuelve la lista de demandas del grafo. */
vector<int> Graph::demands(){
    return this->_demands;
}


/* Devuelve la capacidad de los camiones. */
unsigned int Graph::capacity(){
    return this->_capacity;
}

/* ********************************************************************** */


/* **** FUNCIONES ******************************************************* */

/* Calcula la distancia euclídea entre dos puntos. */
double distancia(Node nodoA, Node nodoB){
    double xA = nodoA.x;
    double yA = nodoA.y;
    double xB = nodoB.x;
    double yB = nodoB.y;

    double deltaX = abs(xA - xB);
    double deltaY = abs(yA - yB);

    double distanciaCuadrada = (deltaX * deltaX) + (deltaY * deltaY);
    double distance = sqrt(distanciaCuadrada);

    return distance;
}


/* Lee la entrada estándar y traduce los datos recibidos a un grafo. */
Graph readGraph(){
    string dato;
    unsigned int cantidad_de_nodos = 0;
    unsigned int capacidad_de_camiones = 0;
    vector<Node> nodos;
    vector<int> demandas;

    /* Lectura de datos. */
    cin >> dato;
    while(dato != "EOF"){

        if(dato == "DIMENSION"){                    // Cantidad de nodos.
            cin >> dato >> cantidad_de_nodos;

        }else if(dato == "CAPACITY"){               // Capacidad de camiones.
            cin >> dato >> capacidad_de_camiones;

        }else if(dato == "NODE_COORD_SECTION"){     // Posiciones de nodos.
            for(unsigned int i = 1; i <= cantidad_de_nodos; i++){
                int id, x, y;
                cin >> id >> x >> y;
                nodos.push_back(Node(id, x, y));
            }

        }else if(dato == "DEMAND_SECTION"){         // Demandas de nodos.
            for(unsigned int i = 1; i <= cantidad_de_nodos; i++){
                int demanda;
                cin >> dato >> demanda;
                demandas.push_back(demanda);
            }
        }

        cin >> dato;
    }

    /* Generación del grafo. */
    return Graph(nodos, demandas, capacidad_de_camiones);
}


/* Lee un archivo en el directorio dado y traduce los datos recibidos a un grafo. */
Graph readGraphFromFile(string directorio){
    string dato;
    unsigned int cantidad_de_nodos = 0;
    unsigned int capacidad_de_camiones = 0;
    vector<Node> nodos;
    vector<int> demandas;

    ifstream archivo(directorio);

    /* Lectura de datos. */
    archivo >> dato;
    while(dato != "EOF"){

        if(dato == "DIMENSION"){                    // Cantidad de nodos.
            archivo >> dato >> cantidad_de_nodos;

        }else if(dato == "CAPACITY"){               // Capacidad de camiones.
            archivo >> dato >> capacidad_de_camiones;

        }else if(dato == "NODE_COORD_SECTION"){     // Posiciones de nodos.
            for(unsigned int i = 1; i <= cantidad_de_nodos; i++){
                int id, x, y;
                archivo >> id >> x >> y;
                nodos.push_back(Node(id, x, y));
            }

        }else if(dato == "DEMAND_SECTION"){         // Demandas de nodos.
            for(unsigned int i = 1; i <= cantidad_de_nodos; i++){
                int demanda;
                archivo >> dato >> demanda;
                demandas.push_back(demanda);
            }
        }

        archivo >> dato;
    }

    archivo.close();

    /* Generación del grafo. */
    return Graph(nodos, demandas, capacidad_de_camiones);
}


/* Devuelve el resultado. */
void writeResult(ListaDeRutas listaDeRutas, int costo){
    cout <</* "Vehículos:    " <<*/ listaDeRutas.size() << endl;

    for(unsigned int i = 0; i < listaDeRutas.size(); i++){
        /*cout << "Ruta # " <<
            (i + 1) << ":   ";*/
        Ruta ruta = listaDeRutas[i];
        for(unsigned int j = 0; j < ruta.size(); j++){
            cout << ruta[j] << " ";
        }
        cout << endl;
    }

    cout <</* "Costo total:    " <<*/ costo << endl;
    return;
}


/*Dada una solución, devuelve su costo total (i.e. la suma del costo de todas sus rutas).*/
int costoSolucion(Graph &graph, ListaDeRutas solucion){
    int costoDeRuta = 0;
    for(Ruta ruta : solucion){
        // Del depósito al primer cliente.
        costoDeRuta += distancia(graph.node(ID_DEPOSITO), graph.node(ruta[0]));
        // De un cliente alsiguiente.
        for(unsigned int i = 0; i < ruta.size() - 1; i++){
            costoDeRuta += distancia(graph.node(ruta[i]), graph.node(ruta[i + 1]));//
        }
        // Del último cliente al depósito.
        costoDeRuta += distancia(graph.node(ruta[ruta.size() - 1]), graph.node(ID_DEPOSITO));
    }
    return costoDeRuta;
}


/* Dados un grafo y una solución (lista de rutas), determina si la solución es válida
(ninguna de las demandas de sus rutas supera la capacidad del camión).*/
bool esSolucionFactible(Graph &graph, ListaDeRutas candidataASolucion){
    bool hayIDMenorOIgualA1 = false;
    bool rutaSuperaCapacidad = false;
    bool todosLosClientesIncluidos = true;
    bool cantidadDeNodosAdecuada = true;
    vector<bool> clientesIncluidos(graph.amountNodes());
    clientesIncluidos[0] = true;    // No cuento el depósito.
    unsigned int cantidadDeNodos = 0;
    for(Ruta ruta : candidataASolucion){
        int demandaDeRuta = 0;
        for(int clientID : ruta){

            /* Revisa que el depósito no se incluya en ninguna ruta, y que todos 
             * los ID en la ruta sean válidos. */
            if(clientID <= 1){
                hayIDMenorOIgualA1 = true;
                //cerr << "[ERROR] ID DE CLIENTE EN LA SOLUCIÓN MENOR O IGUAL A 1." << endl;
                break;
            }

            demandaDeRuta += graph.demand(clientID);    // Suma las demandas.
            clientesIncluidos[clientID - 1] = true;     // Registra los clientes incluídos.
        }

        /* Revisa que las demandas de cada ruta no superen la capacidad de un camión. */
        if (demandaDeRuta > (int)(graph.capacity())){
            rutaSuperaCapacidad = true;
            //cerr << "[ERROR] UNA RUTA EN LA SOLUCIÓN SUPERA LA CAPACIDAD DE UN CAMIÓN." << endl;
            continue;
        }

        cantidadDeNodos += ruta.size();     // Suma la cantidad de nodos.
    }

    /* Revisa que todos los clientes estén incluídos en la solución. */
    for(unsigned int clientIndex =1; clientIndex < graph.amountNodes(); clientIndex++){
        if(!(clientesIncluidos[clientIndex])){
            todosLosClientesIncluidos = false;
            //cerr << "[ERROR] FALTAN INCLUÍR CLIENTES EN LA SOLUCIÓN." << " " << clientIndex + 1 << endl;
            break;
        }
    }

    /* Revisa que no haya clientes repetidos (o faltantes). */
    if(cantidadDeNodos != graph.amountNodes() - 1){
        cantidadDeNodosAdecuada = false;
        //cerr << "[ERROR] HAY UNA CANTIDAD INCORRECTA DE NODOS EN LA SOLUCIÓN." << endl;
    }

    return (!hayIDMenorOIgualA1 && !rutaSuperaCapacidad 
        && todosLosClientesIncluidos && cantidadDeNodosAdecuada);
}


/* Dadas algunas restricciones, genera un caso de prueba aleatorio para el problema. */
void genCasoRand(vector<Node> &ns, vector<int> &ds, unsigned int n, unsigned int C) {
    int desde = 2, hasta = 100;
    unsigned int seed = 123;
    mt19937 generator1(seed);
    uniform_int_distribution<int> distr1(desde, hasta);

    //Se llena el vector "ns" con n nodos cuyas coordenadas (tanto x como y) están entre 0 y 100.
    for (unsigned int i = 1; i <= n; ++i) ns.push_back(Node(/*ID=*/i, /*x=*/distr1(generator1), /*y=*/distr1(generator1)));

    desde = 1, hasta = (int) C/2;
    seed = 456;// No es necesario.
    mt19937 generator2(seed);
    uniform_int_distribution<int> distr2(desde, hasta);

    //Se llena el vector "ds" con n demandas cuyos valores están entre 1 y K/2 (la mitad de la capacidad).
    ds.push_back(0);// La demanda del depot.
    for (unsigned int i = 1; i < n; ++i) ds.push_back(distr2(generator2));// La demanda del resto de los nodos/clientes.
}


/* Dadas algunas restricciones, genera un caso de prueba aleatorio para el problema. */
void genCasoRand2(vector<Node> &ns, vector<int> &ds, unsigned int n, unsigned int C) {
    int desde = 2, hasta = 100;
    unsigned int seed = 123;
    mt19937 generator1(seed);
    uniform_int_distribution<int> distr1(desde, hasta);

    //Se llena el vector "ns" con n nodos cuyas coordenadas (tanto x como y) están entre 0 y 100.
    for (unsigned int i = 1; i <= n; ++i) ns.push_back(Node(/*ID=*/i, /*x=*/distr1(generator1), /*y=*/distr1(generator1)));

    desde = 1, hasta = min((int)C/2, 10);
    seed = 456;// No es necesario.
    mt19937 generator2(seed);
    uniform_int_distribution<int> distr2(desde, hasta);

    //Se llena el vector "ds" con n demandas cuyos valores están entre 1 y K/2 (la mitad de la capacidad).
    ds.push_back(0);// La demanda del depot.
    for (unsigned int i = 1; i < n; ++i) ds.push_back(distr2(generator2));// La demanda del resto de los nodos/clientes.
}


/* Modulariza los casos de test y los experimentos. */
int testing(int argc, char *argv[]){
    if (argc < 3){
        cerr << "Error de argumentos." << endl;
        return 1;
    }

    vector<cvrp_heur_ptr> cvrp_heurs = {greedy, savings, sweep, kmeans, annealing};
    vector<string> str_heurs = {"greedy", "savings", "sweep", "kmeans", "annealing"};
    unsigned int eleccion_heur = atoi(argv[1]) % 5;     // Eleción de la heurística con que se aproxima CVRP. Es tal que 0 <= eleccion_heur <= 3 (cuatro heurísticas).

    /* Elección entre los diferentes experimentos. */
    switch(atoi(argv[2])){
        /* Tiempo en función de cantidad de nodos. Cada dato se obtiene promediando "cantidadAPromediar" mediciones. */
        case 0:{
            /* Formato: ./main <heurística 0 a 4> 0 <cantidad máxima de nodos> <salto entre cálculos> */
            if(argc != 5){
                cerr << "Error de argumentos." << endl;
                return 1;
            }

            // Transformo strings de entrada a enteros (argv[0] es el nombre del ejecutable):
            int hasta = max(1, atoi(argv[3]));              // Máximo valor de n. Valor mínimo 1.
            int salto = max(1, atoi(argv[4]));              // Valor del salto para n en cada iteración. Valor mínimo 1.
            unsigned int K = 50;
            for (int n = 1 ; n <= hasta ; n += salto){
                /* Generación de grafo de capacidad K con n nodos aleatorios (con demandas acotadas por K). */
                vector<Node> nodos;
                vector<int> demandas;
                genCasoRand(nodos, demandas, n, K);
                Graph G(nodos, demandas, K);

                int cantidadAPromediar = 10;
                chrono::duration<double, std::milli> tiempo_milisegundos = (chrono::duration<double, std::milli>)0;
                for(int i = 0; i < cantidadAPromediar; i++){
                    auto inicio = std::chrono::steady_clock::now();         // Comienzo del conteo.
                    cvrp_heurs[eleccion_heur](G);
                    auto fin = std::chrono::steady_clock::now();            // Finalización del conteo.
                    tiempo_milisegundos += (fin - inicio);
                }
                tiempo_milisegundos = tiempo_milisegundos/cantidadAPromediar;
                cout << n << " " << tiempo_milisegundos.count();
                if(n+salto <= hasta) cout << endl;
            }
            break;
        }
        /* Tiempo en función de capacidad de vehículos. Cada dato se obtiene promediando "cantidadAPromediar" mediciones. */
        case 1:{
            /* Formato: ./main <heurística> 1 <capacidad máxima> <salto entre cálculos> <cantidad de nodos con la que probar> */
            if(argc != 6){
                cerr << "Error de argumentos." << endl;
                return 1;
            }

            // Transformo strings de entrada a enteros (argv[0] es el nombre del ejecutable):
            int hasta = max(1, atoi(argv[3]));              // Máximo valor de capacidad. Valor mínimo 1.
            int salto = max(1, atoi(argv[4]));              // Valor del salto para la capacidad en cada iteración. Valor mínimo 1.
            int n = max(1, atoi(argv[5]));                  // Cantidad de nodos. Valor mínimo 1.
            for (int capacidad = 2 ; capacidad <= hasta ; capacidad += salto){
                /* Generación de grafo de capacidad "capacidad" con n nodos aleatorios (con demandas acotadas por "capacidad"). */
                vector<Node> nodos;
                vector<int> demandas;
                genCasoRand(nodos, demandas, n, capacidad);
                Graph G(nodos, demandas, capacidad);

                int cantidadAPromediar = 10;
                chrono::duration<double, std::milli> tiempo_milisegundos = (chrono::duration<double, std::milli>)0;
                for(int i = 0; i < cantidadAPromediar; i++){
                    auto inicio = std::chrono::steady_clock::now();         // Comienzo del conteo.
                    cvrp_heurs[eleccion_heur](G);
                    auto fin = std::chrono::steady_clock::now();            // Finalización del conteo.
                    tiempo_milisegundos += (fin - inicio);
                }
                tiempo_milisegundos = tiempo_milisegundos/cantidadAPromediar;
                cout << capacidad << " " << tiempo_milisegundos.count();
                if(capacidad+salto <= hasta) cout << endl;
            }
            break;
        }
        /* Costo en función de capacidad de vehículos. */
        case 2:{
            /* Formato: ./main <heurística> 2 <capacidad máxima> <salto entre cálculos> <cantidad de nodos con la que probar> */
            if(argc != 6){
                cerr << "Error de argumentos." << endl;
                return 1;
            }

            // Transformo strings de entrada a enteros (argv[0] es el nombre del ejecutable):
            int hasta = max(1, atoi(argv[3]));              // Máximo valor de capacidad. Valor mínimo 1.
            int salto = max(1, atoi(argv[4]));              // Valor del salto para la capacidad en cada iteración. Valor mínimo 1.
            int n = max(1, atoi(argv[5]));                  // Cantidad de nodos. Valor mínimo 1.
            for (int capacidad = 2 ; capacidad <= hasta ; capacidad += salto){
                /* Generación de grafo de capacidad "capacidad" con n nodos aleatorios (con demandas acotadas por "capacidad"). */
                vector<Node> nodos;
                vector<int> demandas;
                genCasoRand2(nodos, demandas, n, capacidad);
                Graph G(nodos, demandas, capacidad);

                ListaDeRutas solucion = cvrp_heurs[eleccion_heur](G);
                int costo = costoSolucion(G, solucion);

                cout << capacidad << " " << costo;
                if(capacidad+salto <= hasta) cout << endl;
            }
            break;
        }
        /* Costos en instancias conocidas, para comparar entre heurísticas y con la solución óptima. */
        case 3:{
            /* Formato: ./main <heurística> 3 */
            if(argc != 3){
                cerr << "Error de argumentos." << endl;
                return 1;
            }

            for (unsigned int i = 0; i < instancias.size(); i++){
                Graph G = readGraphFromFile(instancias[i]);
                ListaDeRutas solucion = cvrp_heurs[eleccion_heur](G);
                int costo = costoSolucion(G, solucion);
                cout << i << " " << costo;
                if(i < instancias.size() - 1) cout << endl;
            }
            break;
        }
        /* Costos en instancias conocidas, para comparar entre heurísticas y con la solución óptima. */
        case 4:{
            /* Formato: ./main <heurística> 4 */
            if(argc != 3){
                cerr << "Error de argumentos." << endl;
                return 1;
            }

            for (unsigned int i = 0; i < instancias2.size(); i++){
                Graph G = readGraphFromFile(instancias2[i]);
                ListaDeRutas solucion = cvrp_heurs[eleccion_heur](G);
                int costo = costoSolucion(G, solucion);
                cout << i << " " << costo;
                if(i < instancias2.size() - 1) cout << endl;
            }
            break;
        }
        /* Costos en instancias aleatorias de varias cantidades de nodos, para comparar entre heurísticas. */
        case 5:{
            /* Formato: ./main <heurística 0 a 4> 5 <cantidad máxima de nodos> <salto entre cálculos> */
            if(argc != 5){
                cerr << "Error de argumentos." << endl;
                return 1;
            }

            // Transformo strings de entrada a enteros (argv[0] es el nombre del ejecutable):
            int hasta = max(1, atoi(argv[3]));              // Máximo valor de n. Valor mínimo 1.
            int salto = max(1, atoi(argv[4]));              // Valor del salto para n en cada iteración. Valor mínimo 1.
            unsigned int K = 100;
            for (int n = 1 ; n <= hasta ; n += salto){
                /* Generación de grafo de capacidad K con n nodos aleatorios (con demandas acotadas por K). */
                vector<Node> nodos;
                vector<int> demandas;
                genCasoRand(nodos, demandas, n, K);
                Graph G(nodos, demandas, K);

                ListaDeRutas solucion = cvrp_heurs[eleccion_heur](G);
                int costo = costoSolucion(G, solucion);
                cout << n << " " << costo;
                if(n+salto <= hasta) cout << endl;
            }
            break;
        }
    }
    return 0;
}


/* Resuelve el problema como se pide en el enunciado y provee 
 * un entorno de pruebas para experimentación. */
int main(int argc, char *argv[]){
    if(argc == 2){
        /* Resolución del problema como se pide en el enunciado. */
        if(atoi(argv[1]) < 0 || atoi(argv[1]) > 4){
            cerr << "Error de argumentos." << endl;
            return 1;
        }

        Graph graph = readGraph();
        ListaDeRutas listaDeRutas;

        switch (atoi(argv[1])) {
            case 0: {listaDeRutas = greedy(graph); break;}
            case 1: {listaDeRutas = savings(graph); break;}
            case 2: {listaDeRutas = sweep(graph); break;}
            case 3: {listaDeRutas = kmeans(graph); break;}
            case 4: {listaDeRutas = annealing(graph); break;}
        }

        //esSolucionFactible(graph, listaDeRutas);
        int costo = costoSolucion(graph, listaDeRutas);

        writeResult(listaDeRutas, costo);
    }else{
        /* Testing y experimentación. Formato: ./main <heurística 0 a 4> <tipo de experimentación> <parámetros de la experimentación> */
        int err_code = testing(argc, argv);
        return err_code;
    }
    return 0;
}

/* ********************************************************************** */
