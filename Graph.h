#ifndef GRAPH_H
#define GRAPH_H

#include <cstdio>
#include <vector>



using std::vector;

#define MAX_NODE_NUM 500000

struct Node
{
	int id;
	int degree;
	int indegree;
	int outdegree;
	double w;
	Node * next;
	Node *pred;
	Node * another;	//指向和这个边对称的边 (x,y)和(y,x)

	Node() :id(0), w(0), next(0), degree(0), indegree(0), outdegree(0), pred(0), another(0){}
	Node(int n, double w = 1, Node *pred = 0, Node *next = 0, Node * another = 0, int degree = 1, int indegree = 0, int outdegree = 0)
		:id(n), w(w), pred(pred), next(next), another(another), degree(degree), indegree(indegree), outdegree(outdegree) {}
	Node(const Node & node);
	bool operator<(const Node & n);
	bool operator==(const Node & n);
};

class Graph
{
	bool is_directed_graph = false;
	bool is_weighted_graph = false;

	vector<Node> t;
	vector<int> nodes;
	int num_nodes = 0;
	

public:
	Graph(){}
	Graph(char filename[], bool is_directed = false, bool is_weighted = false);

	bool load(const char filename[]);
	bool save(const char filename[]);

	int nodes_size() { return nodes.size(); }

	void addEdge(int x, int y, double w = 1);
	void addEdge2(int x, int y, double w = 1);
	//vector<Edge>::iterator removeEdge(vector<Edge>::iterator iter);

	void subGraph(vector<int> nodes, Graph &sub);
	void removeNodes(vector<int> &nodes);
	void bfs(vector<int> & v, int id, int depth = 1);
	

	bool isClique();

	void deleteNodes(Node * p);
	void deleteNode(Node * p);
	void clear();
	void clear(Node & n);

	void calcDegree();

	void print();

	int getMinDegreeNodes(vector<int> &v);
	void getNodes(vector<int> &v);
};


#endif