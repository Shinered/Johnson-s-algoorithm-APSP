#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#define infty 100000000
using namespace std;
/*********************
 *All-Pairs Shortest Paths
 *(APSP)
 running time: 4.4s -O2
 */


/*********十字链表***********/

/************边表*************/
class EdgeNode 
{
public:
	int tail;
	int head;
	int weight;
	EdgeNode *headlink = NULL; //入边表指针域
	EdgeNode *taillink = NULL; //出边表指针域
	EdgeNode(int _tail, int _head, int _weight) :
	tail(_tail), head(_head), weight(_weight) {};

};

/**********顶点表*******/
class VertexNode 
{
public:
	int data;
	EdgeNode *firstin; //入度
	EdgeNode *firstout;//出度
	int pValue;
	int dist = infty;
};


//typedef pair<VertexNode, int>pii;
	
/**********图的数据结构***********/
class Graph
{
public:
	Graph()
	{
		ifstream fin("g3.txt");
		string line;
		stringstream stream;
		if(getline(fin, line))
		{
			stream.clear();
			stream << line;
			stream >> numVertex;
			stream >> numEdge;
		}
		
		init_adjList();
		while(getline(fin, line))
		{
			int _head;
			int _tail;
			int _weight;
			stream.clear();
			stream << line;
			stream >> _tail;
			stream >> _head;
			stream >> _weight;
			addEdge(_tail, _head, _weight);
		}
	}
/***********加0点为Bellmanford算法准备************/
	void init_adjList()
	{
		adjList.resize(numVertex+1);
		for(int i = 0; i < numVertex+1; i++)
		{	
			adjList[i].data = i;
		}
		
		for(int i = 1; i < numVertex+1; i++)
		{
			addEdge(0,i,0);
		}

	}
/*******************加边，头插法*********************/	
	void addEdge(int a, int b, int wei)
	{
		EdgeNode *enode = new EdgeNode(a, b, wei);
		enode->taillink = adjList[a].firstout;
		enode->headlink = adjList[b].firstin;
		adjList[a].firstout = enode;
		adjList[b].firstin = enode;
	}
	
	int get_numVertex()
	{
		return numVertex+1;
	}

/***********删除新增加的0点************/	
	void delete_node0()
	{
		adjList.erase(adjList.begin());
		for(int i = 0; i < numVertex; i++)
		{
			EdgeNode *p = adjList[i].firstin;
			while(p->headlink->tail != 0)
			{
				p = p->headlink;				
			}
			p->headlink = p->headlink->headlink;
		}
		
		numVertex--;
	}

	
	void print()
	{
		ofstream fout;
		fout.open("node.txt");
		for(int i = 0; i < numVertex+1; i++)
		{
			fout << adjList[i].data << " ";
			EdgeNode *p = adjList[i].firstout;
			while(p)
			{
				fout << p->head << " " << p->weight << " ";
				p = p->taillink;
			}
			fout << endl;
			
		}
		
	}

public:
	vector<VertexNode> adjList;
	int numVertex;
	int numEdge;
};
/**************Bellmanford算法，解决负权重边***************/
class BellmanFord
{
public:
	BellmanFord(Graph graph, int _source);
	
	void get_Amatrix(Graph &graph);
	int min_wv(Graph graph, int i, int v);
	int min_value(int a, int b);

	bool is_cycle_exist();
	void p_value(Graph &graph);
	void reweight(Graph &graph);
	void print();

public:
	vector<int> distTo;
	vector< vector<int> > Amatrix;
	int numVertex;
};
/********优先队列自定义比较函数********/
class mycompare
{
	bool reverse;
public:
	mycompare(const bool &re = true)
	{
		reverse = re;
	}
	
	bool operator() (VertexNode a, VertexNode b)
	{
		if(reverse) return (a.dist > b.dist); //从小到大排列
		else {
			return (a.dist < b.dist); //从大到小排列
		}
	}
};

/***************最短路径*****************/
class Dijkstra 
{
public:
	Dijkstra(Graph &graph)
	{
	
		numVertex = graph.get_numVertex();
		distTo.resize(numVertex);
		cout << "number:" << numVertex << endl;
		for(int i = 0; i < numVertex; i++)
		{
			dijkstra(graph, i);	
			redistance(graph, i);
			distTo[i].resize(numVertex);
			for(int j = 0; j < numVertex; j++)
			{
				distTo[i][j] = graph.adjList[j].dist;
				alldist.push_back(graph.adjList[j].dist);
			}
		}


	}
		
	void min_dist()
	{
		vector<int>	::iterator min = min_element(alldist.begin(), alldist.end());
		cout << "Shortest path:" << *min << endl;
	//	return *min;
	}

	void dijkstra(Graph &graph, int s)		
	{
	//	numVertex = graph.get_numVertex();
	//	cout << "number:" << numVertex << endl;
		init_marked();
	//	distTo[s].resize(numVertex);
		for(int i = 0; i < numVertex; i++)
		{
			if(i == s) graph.adjList[i].dist = 0;
			else graph.adjList[i].dist = infty;
		}


//		graph.adjList[s].dist = 0;
		pq.push(graph.adjList[s]);
		int n = 0;
		while(!pq.empty())
		{
			n++;
			VertexNode node = pq.top();
			pq.pop();
			marked[node.data-1] = false; //在外边false
			deleted[node.data-1] = true; //删除的node
//			cout << "pair-.first:" <<  node.first.data -1 << endl;
			EdgeNode *p = graph.adjList[node.data-1].firstout;
			while(p)
			{
				if(graph.adjList[p->head-1].dist > graph.adjList[p->tail-1].dist+p->weight)
				{
					/*if(n==1) {cout << "dist:" << graph.adjList[p->head-1].dist << " ";
						graph.adjList[p->head-1].dist = graph.adjList[p->tail-1].dist+p->weight;
						cout << "dist:" << graph.adjList[p->head-1].dist << " ";
						graph.adjList[p->head-1].dist = graph.adjList[p->tail-1].dist+p->weight;
					}*/
					if(marked[p->head-1]) {
						graph.adjList[p->head-1].dist = graph.adjList[p->tail-1].dist + p->weight;
						//cout << "dist_to_head:" << distTo[p->head-1] << endl;
					}
					if(!marked[p->head-1] && !deleted[p->head-1]){
						graph.adjList[p->head-1].dist = graph.adjList[p->tail-1].dist + p->weight;
					//	cout << "p->head-1:" << graph.adjList[p->head-1].dist << " ,";
						pq.push(graph.adjList[p->head-1]);					
						marked[p->head-1] = true;
					}
				}
			//	cout << p->head << ",";
				p = p->taillink;
			}
					
		}


	}
	
	void init_marked()
	{
		marked.resize(numVertex);
		deleted.resize(numVertex);
		for(int i = 0; i < numVertex; i++)
		{
			marked[i] = false;
			deleted[i] = false;
		}

	}
	
	void redistance(Graph &graph, int s)
	{
		for(int i = 0; i < numVertex; i++)
		{
			graph.adjList[i].dist = graph.adjList[i].pValue - graph.adjList[s].pValue + graph.adjList[i].dist;
		}
		
	}
	void print(Graph graph)
	{
		ofstream fout2;
		fout2.open("distance.txt");
		for(int i = 0; i < numVertex; i++)
		{
			fout2 << i << ": ";
			for(int j = 0; j < numVertex; j++)
			{
				fout2 << distTo[i][j] << ",";
			}
			fout2 << endl;
		}
	}
public:
	int numVertex;
	priority_queue<VertexNode, vector<VertexNode>, mycompare> pq;
	vector< vector<int> > distTo;
	vector<int> alldist;

	vector<bool> marked;
	vector<bool> deleted;
};




	
