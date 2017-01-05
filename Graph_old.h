#ifndef GRAPH_H
#define GRAPH_H

#include <vector>


using std::vector;

struct Edge
{
	int x;
	int y;
	double w;

public:
	Edge(int i, int j) : x(i), y(j), w(1) {}
	Edge(int i, int j, int weight) : x(i), y(j), w(weight) {}
	Edge() : x(0), y(0), w(0) {}

	bool operator<(Edge & e);
	bool thisEdgeIsInNodesList(const vector<int> &v);	//两个点都在列表中
	bool thisEdgeIsInNodesList1(const vector<int> &v);	//至少一个点在列表中
	bool thisEdgeHasNode(int n);
};
struct Node
{
	int id;
	int degree;
	int indegree;
	int outdegree;

	Node(int n, int degree = 1, int indegree = 0, int outdegree = 0)
		:id(n), degree(degree), indegree(indegree), outdegree(outdegree) {}
	Node(const Node & node);
	bool operator<(const Node & n);
	bool operator==(const Node & n);
};

class Graph
{
	bool is_directed_graph = false;
	bool is_weighted_graph = false;

	vector<Edge> edges;
	vector<Node> nodes;
	

public:
	Graph(){}
	Graph(char filename[], bool is_directed = false, bool is_weighted = false);

	bool load(const char filename[]);
	bool save(const char filename[]);

	int nodes_size() { return nodes.size(); }

	void addEdge(int x, int y, double w = 1);
	vector<Edge>::iterator removeEdge(vector<Edge>::iterator iter);

	void subGraph(vector<int> nodes, Graph &sub);
	void removeNodes(vector<int> &nodes);
	void bfs(vector<int> & v, int id, int depth = 1);
	

	bool isClique();


	void clear();

	void calcDegree();

	void print();

	int getMinDegreeNodes(vector<int> &v);
	void getNodes(vector<int> &v);
};


#endif