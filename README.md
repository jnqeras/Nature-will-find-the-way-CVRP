# CVRP

### Instrucciones.

- It might be neccesary to remove the "-std=gnu++11" flag from src/Makefile.

- HEURISTIC:
	- 0 = Greedy, O(n^2).
	- 1 = Savings. O(n^3).
	- 2 = Sweep. O(n^2).
	- 3 = K-Means, O(n\*K + n^2). Como la estimación K es O(n), entonces K-Means es O(n^2).
	- 4 = Simulated Annealing.

##### Ejecución:
1. Con una terminal, entrar al directorio "src/" desde la carpeta raíz.
2. Ejecutar el comando "make main".
3. Hay dos opciones, donde HEURISTIC representa la heurística o metaheurística de prueba a implementar. Debe ser un número entre 0 y 4 inclusive.
	- Ejecutar el comando "./main HEURISTIC" e ingresar los datos a mano, donde HEURISTIC representa la heurística o metaheurística de prueba a implementar. Debe ser un número entre 0 y 4 inclusive..
	- Ejecutar el comando "./main < DIRECTORIO_ARCHIVO", donde DIRECTORIO_ARCHIVO es la ubicación relativa a la carpeta "src/" de un archivo que cumple con el formato impuesto por la especificación formal de TSPLIB95 dada. Hay varios archivos de prueba del estilo en el directorio (relativo a "src/") "instancias/Vrp-Set-A/".


##### Testing y experimentaciones:
1. Con una terminal, entrar al directorio "src/" desde la carpeta raíz.
2. Ejecutar el comando "make main".
3. Para cada una de las heurísticas (no simulated annealing), se pueden correr los siguientes tests (los cuales promedian 10 ejecuciones de cada dato de tiempo que calculan):
	1. Control cualitativo.
		- En la consola, ejecutar ./main HEURISTIC < instancias/Vrp-Set-A/A\A-nX-kY.vrp > experimentos/rutas_heuristic_nX_kY.out, y reemplazar X, Y con N y K según las instancias disponibles. Reemplazar, también, "heuristic" con el nombre de la heurística en cuestión (greedy, savings, sweep, kmeans) y HEURISTIC con el número correspondiente.
	2. Tiempo en función de la cantidad de nodos y comparación contra la complejidad teórica.
		- En la consola, ejecutar ./main HEURISTIC 0 N_MÁXIMO SALTO_N > experimentos/nodos_heuristic.out, y reemplazar HEURISTIC con el número de la heurística en cuestión, N_MÁXIMO con la máxima cantidad de nodos a evaluar, SALTO_N con el salto entre cantidad de nodos. Reemplazar, también, "heuristic" con el nombre de la heurística en cuestión (greedy, savings, sweep, kmeans).
	3. Tiempo en función de la capacidad de los camiones, para K-Means.
		- En la consola, ejecutar ./main 3 1 CAPACIDAD_MÁXIMA SALTO_CAPACIDAD N > experimentos/capacidad_HEURÍSTICA_N, donde CAPACIDAD_MAXIMA es la máxima capacidad a probar, SALTO_CAPACIDAD es el salto entre cálculos y N es el número de nodos con el que experimentar.
	3. Costo en función de la capacidad de los camiones, para K-Means.
		- En la consola, ejecutar ./main HEURISTIC 2 CAPACIDAD_MÁXIMA SALTO_CAPACIDAD N > experimentos/capacidad_costo_HEURÍSTICA_N, donde CAPACIDAD_MAXIMA es la máxima capacidad a probar, SALTO_CAPACIDAD es el salto entre cálculos y N es el número de nodos con el que experimentar.
4. Luego, ejecutar el comando jupter notebook, y clickear en Kernel -> Restart & Run All. Deberían poder verse los gráficos tal cual fueron generados para el informe.
	
### Aclaraciones.
1. El archivo .gitignore ignora los ejecutables que se borran con el comando "make clean".
2. Las soluciones provistas a las instancias de la página web dada en el enunciado no son fieles al problema tal como se quiere resolver en este TP. Por un lado, requieren del uso de una cantidad mínima de vehículos. Por el otro, imprimen los índices de los nodos de una ruta, y no sus ID.
3. El programa lee archivos del tipo especificado en el enunciado del trabajo práctico. Sin embargo, los lee a través de la entrada estándar. Para leer un archivo con el programa "main", se debe ejecutar el comando "./main < ARCHIVO", reemplazando ARCHIVO con la ruta relativa del mismo a donde se ejecuta el comando.
4. Los resultados se devuelven por la salida estándar.
5. A priori, asumimos que absolutamente todas las instancias se deben resolver con exactamente un depósito, ubicado en el nodo con el ID 1. Ignoramos lo que indiquen los archivos describiendo las instancias del problema.
6. El ID de un nodo en la representación de grafo elegida será *p + 1*, si *p* es su posición en el vector de nodos.
7. Para la entrega de soluciones se devuelven los ID de los nodos (y no los índices).
8. Las implementaciones no tienen que ser fieles a los papers al pie de la letra.
9. A priori, se implementó una versión de grafos con nodos y demandas, y los ejes estan implícitos, pues el grafo que describe el problema es siempre completo.
10. La heurística K-MEANS tiene complejidad temporal O(n\*K + n^3). K es estimada como la suma total de las capacidades de los clientes sobre la capacidad de los vehículos. Si las capacidades de los vehículos son acotadas - y es razonable pensar que lo son para problemas cotidianos -, entonces K pasa a ser O(n), y la complejidad temporal de la heurística pasa a ser O(n^3).

### Heurística golosa (nearest neighbour).
1. Comenzar desde el depósito e ir al nodo más cercano. Marcarlo como visitado.
2. Elegir el nuevo nodo *p* más cercano al actual.
3. Mientras la suma de la demanda de *p* y las demandas de los nodos visitados hasta ahora no supere la capacidad del camión, ir a *p* y marcarlo como visitado.
4. Cuando supera la capacidad, no ir a *p*, ir directamente al depósito.
5. Si ya se revisaron todos los nodos, termina el algoritmo.
6. Si quedan nodos por revisar, nuevo camión y repetir el paso 2 desde el depósito.

### Simulated annealing.
- Explicación en la sección 3 del paper de Osman. Es corto, mide una carilla y media.
- Osman propone una función *no monótona* de reducción de temperatura.
- Hay un paper extra adjunto. Habla de simulated annealing de manera más generalizada.
- Parámetros de la cooling schedule:
	- *Ts* = Temperatura inicial.
	- *Tf* = Temperatura final.
	- *Tr* = Temperatura de reinicio.
	- *Alpha* y *Gamma* = Valores pura y exclusivamente empíricos. El paper propone inicializarlos de una manera en particular.
	- *R* = Criterio de finalización. El paper propone que R sea la cantidad de reinicios de temperatura que se hacen desde la última mejor solución fue encontrada.
	- *Sb* = Mejor solución encontrada hasta ahora.
	- *k* = Cantidad de veces que se redujo la temperatura o iteración actual el en algoritmo de Simulated Annealing.
	- *n* = Cantidad de clientes.
	- *Tk* = Temperatura actual.

### Cluster first, route second (alternativo).
1. Clusterizar con el algoritmo K-Means (da resultados similares pero diferentes cada vez, por ser randomizado).
	1. Estimar K como la suma total de demandas sobre la capacidad de un camión.
	2. Inicializar K puntos llamados "means" en el espacio aleatoriamente. Se debe respetar el rango de los puntos a clusterizar.
	3. Repetir el siguiente proceso una cantidad fija de veces:
		1. Calculamos, para cada punto a clusterizar, a qué mean es más cercano.
		2. Actualizamos las posiciones de las means como la media de las posiciones de los puntos que eran más cercanos a él.
	4. El resultado serán K clusters. El cluster al que pertenece cada punto es la mean al que es más cercano.
2. Si algún cluster tiene más demanda total que la que puede abastecer un camión, separarlo en clusters de manera golosa.
3. Resolver los problemas tipo TSP restantes con la heurística de nearest insertion. Llamo d(k,j) a la distancia entre dos ciudades k y j.
	1. Comenzar con un camino de dos ciudades. Elegir una aleatoria y luego la más cercana a ella.
	2. Repetir el siguiente proceso mientras el camino no cubra todos los clientes:
		1. Elegir el par de ciudades (k,j) tal que:
			- La ciudad k no pertenezca al camino, y j sí.
			- Se minimiza d(k,j) entre todos los pares.
		2. Encontrar el eje (i,j) en el camino tal que se minimiza (d(k,i) + d(k,j) - d(i,j)).
		3. Remover (i,j) del camino. Agregar (k,i) y (k,j).

### Ideas para la experimentación.
1. Hacer un control cualitativo, dibujando los caminos generados para varias instancias en gráficos fácilmente visibles. Aprovechar este control para ver la factibilidad de las soluciones generadas. HECHO EN NOTEBOOK.
2. Incrementar cantidad de nodos y ver cómo influye eso en el tiempo tardado promediar varias ejecuciones por cada dato generado. HECHO EN NOTEBOOK.
3. Hacer un jointplot y medir el coeficiente de correlación de Pearson entre las complejidades teóricas y los tiempos tardados para cada algoritmo. HECHO EN NOTEBOOK.
4. Variar la capacidad de los vehículos y ver si influye en los tiempos tardados. HECHO EN NOTEBOOK.
5. Variar la capacidad de los vehículos y ver si influye en los costos de los caminos resultantes. HECHO EN NOTEBOOK.
6. Variar la distancia entre nodos ver si influye en los tiempos tardados.
7. Calcular la distancia entre los costos de las soluciones heurísticas de cada algoritmo para las instancias dadas por la cátedra. Representarla en forma porcentual o multiplicativa. HECHO EN NOTEBOOK.
8. Comparar los costos de diversos casos favorables y desfavorables para distintos algoritmos.
	- Con clusters.
	- Sin clusters.
	- Con clusters desfavorables para Sweep.