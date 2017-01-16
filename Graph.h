#ifndef GRAPH_H
#define GRAPH_H

/*
 * 图 包含结点和边 采用邻接表存储
*/
#include <cstdio>
#include <vector>



using std::vector;

#define MAX_NODE_NUM 500000
/*
边:(1,3),(1,4),(2,5),(3,4),(4,5)
表     表中结点相连的边
[1] -> [3] -> [4]
[2] -> [5]
[3] -> [4]
[4] -> [5]
[5]
*/
struct Node
{
	int id;	//这个node的id 表结点专用
	int degree;	//结点的度 表头结点专用
	int indegree;	//入度 表头结点专用
	int outdegree;	//出度	表头结点专用
	double w;	//边的权值 表结点专用
	Node * next;
	Node *pred;
	Node * another;	//指向和这个边对称的边 (x,y)和(y,x)

	//默认构造函数，全部成员初始化为0
	Node() :id(0), w(0), next(0), degree(0), indegree(0), outdegree(0), pred(0), another(0){ }

	Node(int n, double w = 1, Node *pred = 0, Node *next = 0, Node * another = 0, int degree = 1, int indegree = 0, int outdegree = 0)
		:id(n), w(w), pred(pred), next(next), another(another), degree(degree), indegree(indegree), outdegree(outdegree) {}

	Node(const Node & node);


	bool operator<(const Node & n);	//按照结点编号比较大小
	bool operator==(const Node & n);	//判断结点编号是否相等
};

class Graph
{
	bool is_directed_graph = false;	//有向图
	bool is_weighted_graph = false;	//有权图

	vector<Node> t;	//邻接表
	vector<int> nodes;	//保存图中的结点编号
	

public:
	Graph(){}
	Graph(char filename[], bool is_directed = false, bool is_weighted = false);	//从文件打开图

	bool load(const char filename[]);	//从文件加载图
	bool save(const char filename[]);	//保存到文件

	int nodes_size()	//返回结点个数
	{ return nodes.size(); }

	void addEdge(int x, int y, double w = 1);	//添加一条边(x,y)

	void subGraph(vector<int> nodes, Graph &sub);	//根据结点提取子图 保存在sub中

	void removeNodes(vector<int> &nodes);	//从图中删去结点


	void bfs(vector<int> & v, int id, int depth = 1);	//从id开始广度优先遍历的结点保存到v中 目前只支持depth为1
	

	bool isClique();	//判断图是不是一个团

	void deleteNodes(Node * p);
	void deleteNode(Node * p);
	void clear();
	void clear(Node & n);

	

	int getMinDegreeNodes(vector<int> &v);	//得到图中度最小的结点 保存到v中 返回结点个数

	void getNodes(vector<int> &v);	//得到图的所有结点id
	
	
	
	void print();
	bool create_dot_file(char *outfilename);
};


#endif