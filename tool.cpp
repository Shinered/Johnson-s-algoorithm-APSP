#include "class.hpp"
#include <algorithm>
using namespace std;

BellmanFord::BellmanFord(Graph graph, int s)
{
	numVertex = graph.get_numVertex();
	Amatrix.resize(numVertex+1);

	for(int i = 0; i < numVertex+1; i++)
	{
		Amatrix[i].resize(numVertex);
	}

	Amatrix[0][s] = 0;

	for(int i = 1; i < numVertex; i++)
	{
		if(i != s) {
			Amatrix[0][i] = infty;
		}
	}
}

int BellmanFord::min_wv(Graph graph, int i, int j)
{	
	int min = infty;
	EdgeNode *p = graph.adjList[j].firstin;
	while(p)
	{
		if(min > Amatrix[i-1][p->tail] + p->weight)
		{
			min = Amatrix[i-1][p->tail] + p->weight;
		}
		p = p->headlink;
	}
	return min;

}

int BellmanFord::min_value(int a, int b)
{
	return a < b ? a : b;
}


void BellmanFord::get_Amatrix(Graph &graph)
{
	for(int i = 1; i < numVertex+1; i++)
	{
		for(int j = 0; j < numVertex; j++)
		{
			Amatrix[i][j] = min_value(Amatrix[i-1][j], min_wv(graph, i, j));
		}

	}
}

bool BellmanFord::is_cycle_exist()
{
	int flag = 0;
	for(int i = 0; i < numVertex; i++)
	{
		if(Amatrix[numVertex-1][i] != Amatrix[numVertex][i])
		{
			cout << "Negative cycle exists!" << endl;
			flag = 1;
			return true;
			break;
		}
	}
	if(!flag){
		cout << "No negative cycle!!" << endl;
		return false;
	}
}

void BellmanFord::p_value( Graph &graph)
{
	for(int i = 0; i < numVertex; i++)
	{
		vector<int> temp;
		for(int j = 0; j < numVertex; j++)
		{
			temp.push_back(Amatrix[j][i]);
		}
		vector<int>::iterator min = min_element(temp.begin(), temp.end());
		graph.adjList[i].pValue = *min;
	}
}

void BellmanFord::reweight(Graph &graph)
{
	ofstream fout;
	fout.open("node1.txt");
	for(int i = 0; i < numVertex; i++)
	{
		fout << graph.adjList[i].data << " ";
		EdgeNode *p = graph.adjList[i].firstin;
		while(p)
		{
			p->weight = graph.adjList[p->tail].pValue - graph.adjList[p->head].pValue + p->weight;

			fout << p->tail << " " <<  p->weight << " ";
			p = p->headlink;
		}
		fout << endl;
	}
}

void BellmanFord::print()
{
	ofstream fout;
	fout.open("matrix.txt");

	for(int i = 0; i < numVertex+1; i++)
	{
		for(int j = 0; j < numVertex; j++)
		{
			fout << "(" << i << "," << j << ")" << Amatrix[i][j] << " ";
		
		}
		fout << endl;
	}

}






