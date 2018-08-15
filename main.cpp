#include "class.hpp"
#include <ctime>
using namespace std;

int main()
{
	clock_t start, end;
	start = clock();
	Graph graph;
	//graph.print();
	
	BellmanFord bellmanford(graph, 0);
	bellmanford.get_Amatrix(graph);
	if(!bellmanford.is_cycle_exist()) {
		bellmanford.p_value(graph);
		bellmanford.reweight(graph);
		bellmanford.print();
		graph.delete_node0();
		graph.print();
	
		Dijkstra dijkstra(graph);
		dijkstra.min_dist();
		dijkstra.print(graph);
	}
	end = clock();
	
	cout << "running time:" << (double)(end-start) /CLOCKS_PER_SEC << "s" << endl;
	return 0;

}
