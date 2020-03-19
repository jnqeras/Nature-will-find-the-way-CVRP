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
1. Open terminal in the directoriy "/src", from the root directory.
2. Execute "make main".
3. There are two options:
	- Option A: Execute "./main HEURISTIC", where HEURISTIC represents the heuristic or metaheuristic that will be used. It must be a number from 0 to 4. Then open a .vrp file form /src/instancias/Vrp-Set-A or /src/instancias/Vrp-Set-B, copy it's content and paste it on the terminal.

	-Option B: Execute "./main < FILE_DIRECTORY", where FILE_DIRECTORY is the relative directory from the folder "./src" of a file (with the TSPLIB95 specification). There are many files of this kind in the directory (relative to "./src") "instancias/Vrp-Set-A/".

##### Testing and experimentation:
1. Open terminal in the directory "src/".
2. Execute "make main".
3. For each of the heuristics (except simulated annealing FIXME: ver si esto sigue siendo así), the following tests can be executed:
	1. Cualitative control:
		- In terminal, execute ./main HEURISTIC < instancias/Vrp-Set-A/A\A-nX-kY.vrp > experimentos/rutas_heuristic_nX_kY.out, replacing X, Y with N and K according to the instances that are available. Replace, "heuristic" withthe name of the desired heuristic (greedy, savings, sweep, kmeans) and HEURISTIC with the corresponding number.
	2. Time according to the number of nodes and comparison against theoretical complexity.
		- In terminal, execute ./main HEURISTIC 0 N_MÁXIMO SALTO_N > experimentos/nodos_heuristic.out, and replace HEURISTIC with the number of the heristic to be tested, N_MÁXIMO with the maximum quantity of nodes to be evaluated, SALTO_N is the increment number of nodes from one instance to the next (i.e. if this number is 3, and one instance has n nodes, the next will have n+3 nodes). Replace "heuristic" with the name of the heuristic to be tested (greedy, savings, sweep, kmeans).
	3. Time according to the capacity of truck load capacity, for K-Means.
		- In console, execute ./main 3 1 CAPACIDAD_MÁXIMA SALTO_CAPACIDAD N > experimentos/capacidad_HEURÍSTICA_N, where CAPACIDAD_MAXIMA is the maximum load capacity to be tested, SALTO_CAPACIDAD  is the increment number of the truck's load capacity from one instance to the next (i.e. if this number is 3, and the load capacity of the trucks in a given instance is n, the next will have n+3 as the load capacity of it's tucks) and N is the number of nodes of every instance.
	4. Corst according to the capacity of truck load capacity, for K-Means.
		- In console, execute ./main HEURISTIC 2 CAPACIDAD_MÁXIMA SALTO_CAPACIDAD N > experimentos/capacidad_costo_HEURÍSTICA_N, where CAPACIDAD_MAXIMA is the maximum load capacity to be tested, SALTO_CAPACIDAD is the increment number of the truck's load capacity from one instance to the next (i.e. if this number is 3, and the load capacity of the trucks in a given instance is n, the next will have n+3 as the load capacity of it's tucks) and N is the number of nodes of every instance.
4. Afther this, execute "jupyter notebook" and click on Kernel -> Restart & Run All. This should show the same plots used on the report.
	
### Clarifications.
1. The file .gitignore ignores the executables that can be errased with "make clean".
2. The solutions given to the instances provided on this page http://vrp.
atd-lab.inf.puc-rio.br/index.php/en/ (this page can be fond here: ./doc/tp3.pdf) do not match with the problem that this work tryes to solve. First off, the require the ussage of a mínimum quantity of vehicles. Also, they return the index of the nodes of a route, and not their ID.
3. The code reads files that have the type specificted by this assignment (./doc/tp3.pdf). Nevertheless, it reads them through the standard input. In order to read a file with "main", the user must execute "./main < ARCHIVO", replacing ARCHIVO with the the file directory of the file (relative to the directory where the command it's been executed).
4. The results are returned through the standard output.
5. It's assumed that every instance has only one depot, this will be the node with the ID 1. What is indicated by the files that describe the instances of the problem (in this regard) will be ignored.
6. The ID of a given node will be *p + 1*, where *p* is it's position in the vector of nodes.
7. The output will be the IDs of the nodes (not the index).
8. The implementations are adapations of the papers, they are the adaptations.
9. The implementation uses a representation of the graphs by nodes and demands (of each node). The edges are implicit, because the graphs are complete graphs.
10. The heuristic K-MEANS has a temporal complexity of O(n\*K + n^3). K is calculated as the total sum of the capacities of the clients divided by the load capacity of the vehicles. If the load capacity of the vehicles is bounded -and it's reasonable to think that they are bounded for real life problems-, so K turns O(n), and the time complexity of the heuristic becomes O(n^3).

####Next, a breef explanation of the heuristics and metaheuristics used on this project: FIXME: VER SI ESTO SE VE MÁS GRANDE POR HABERLE PUESTO CUATRO NUMERALES.

### Greedy Heuristic (Heurística golosa -nearest neighbour-.
1. Begins at the depot and goes to the closest node (let's call it node *a*). Set the node *a* as "visited".
2. Choose the closest new node to *a*, let's call it node *p*. A node is new if it isn't set to visited (it has never been visited before).
3. As long as the sum of the demand of *p* and the demand of the visited nodes isn't bigger than the load capacity of the truck, move to *p* and set it as "visited".
4.If the capacity is exceeded, don't move to *p* and return to the depot.
5. If all the nodes have been visited, return to the depot.
6. If there are nodes that haven't been visited, a empty truck (with it's full load capacity), begins from the depot and repeats the process from step 2.

### Simulated annealing.
- Explanation in section 3 of Osman's paper [[1]](#1).

- Osman proposes  a non-monotonous cooling function [[1]](#1).
- This paper explains simulated annealing in a broader way: [[2]](#2)
- Cooling schedule's parameters:
	- *Ts* = Initial temperature.
	- *Tf* = Final temperature.
	- *Tr* = Rule for temperature reset.
	- *Alpha* & *Gamma* = empirical values. The paper proposes to initialize them in a specific way.
	- *R* = Stopping criterion. The paper proposes R to be the ammount of temperature resets made since the last best solution was found.
	- *Sb* = Best solution found so far.
	- *k* = Indicates the how many times the temperature was reduced or indicates the current iteration of the algorithm.
	- *n* = Number of clients (nodes).
	- *Tk* = current temperature.

### Cluster first, route second (alternative solution).
1. Cluster the nodes using K-Means (each time it returns different yet similar results becaus it'srandomized).
	1. Approximate K  as the total sum of demands divided by the load capacity of the truck.
	2. Initialize K points (called "means") randomly located on the graph space. The range of the points to be clustered must be respected.
	3. Repeat the following two steps a fixed number of iterations:
		1. Calculate, for each node to be clustered, which is the nearest "mean". 
		2. Update the positions of the "means" as the average of the positions of the points that where closer to them (closer to the "means").
	4. This will generate K clusters. Each point will bellong to the cluster of the "mean" that is the closest. 
2. The demand of a cluster is the sum of the demand of its nodes. If any given cluster has more demand than the load capacity of the truck, then divide the cluster using a greedy algorithm.
3. For each cluster, solve the problems of TSP using the nearest insertion heuristic. Now d(k,j) will be the distance between two cities k and j.
	1. Begin with a route between two clients. Choose one client randomly and then the closest to it.
	2. Repeat the following process as long as the route doesn't connect all the clients:
		1. Pick a pair of clients (k,j) such as:
			- The client k doesn't bellong to the road, but j does.
			- Minimize d(k,j) between every pair.
		2. Find the (i,j) edge in the road such that minimizes (d(k,i) + d(k,j) - d(i,j)).
		3. Remove (i, j) from the road. Add (k,i) y (k,j).

### Experimentation ideas.
1. Make a cualitative control, depicting the generated roads for multiple instances in easy to visualize plots. Use this test to evaluate the feasibility of the generated solutions. Implemented on the Jupyter Notebook.
2. Increment the number of nodes and observate how this impacts on the time that the algorithm takes to execute. Calculate the average of many executions for each instance generated. Implemented on the Jupyter Notebook.
3. Plot a jointplot and messure Pearson's correlation coefficient between the theoretical complexity and the execution time of each algorithm. Implemented on the Jupyter Notebook.
4. Vary the number of vehicles and verify the impact on the execution time of the algorithms. Implemented on the Jupyter Notebook.
5. Vary the load capacity of the vehicles and verify the impact on the execution time of the algorithms. Implemented on the Jupyter Notebook.
6. Vary the distance in between nodes and verify the impact on the execution time of the algorithms. Implemented on the Jupyter Notebook.
7. Calculate the distance between the costs of the heuristic solutions of each algorithm. Implemented on the Jupyter Notebook.
8. Compare the costs of execution for different kind of instances for each algorithm:
	- With clusters.
	- Without clusters.
	- With unfavorable clusters for para Sweep.


## References
<a id="1">[1]</a> 
I. Osman (1993). Metastrategy simulated annealing and tabu search
algorithms for the vehicle routing problem. DOI: 10.1007/BF02023004


<a id="2">[2]</a> 
I. Osman and N Christofides (1994). Capacitated Clustering Problems by Hybrid
Simulated Annealing and Tabu Search. DOI: 0969401 6(94)E00012-H
