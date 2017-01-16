#ifndef GRAPH_H
#define GRAPH_H

/*
 * ͼ �������ͱ� �����ڽӱ�洢
*/
#include <cstdio>
#include <vector>



using std::vector;

#define MAX_NODE_NUM 500000
/*
��:(1,3),(1,4),(2,5),(3,4),(4,5)
��     ���н�������ı�
[1] -> [3] -> [4]
[2] -> [5]
[3] -> [4]
[4] -> [5]
[5]
*/
struct Node
{
	int id;	//���node��id ����ר��
	int degree;	//���Ķ� ��ͷ���ר��
	int indegree;	//��� ��ͷ���ר��
	int outdegree;	//����	��ͷ���ר��
	double w;	//�ߵ�Ȩֵ ����ר��
	Node * next;
	Node *pred;
	Node * another;	//ָ�������߶ԳƵı� (x,y)��(y,x)

	//Ĭ�Ϲ��캯����ȫ����Ա��ʼ��Ϊ0
	Node() :id(0), w(0), next(0), degree(0), indegree(0), outdegree(0), pred(0), another(0){ }

	Node(int n, double w = 1, Node *pred = 0, Node *next = 0, Node * another = 0, int degree = 1, int indegree = 0, int outdegree = 0)
		:id(n), w(w), pred(pred), next(next), another(another), degree(degree), indegree(indegree), outdegree(outdegree) {}

	Node(const Node & node);


	bool operator<(const Node & n);	//���ս���űȽϴ�С
	bool operator==(const Node & n);	//�жϽ�����Ƿ����
};

class Graph
{
	bool is_directed_graph = false;	//����ͼ
	bool is_weighted_graph = false;	//��Ȩͼ

	vector<Node> t;	//�ڽӱ�
	vector<int> nodes;	//����ͼ�еĽ����
	

public:
	Graph(){}
	Graph(char filename[], bool is_directed = false, bool is_weighted = false);	//���ļ���ͼ

	bool load(const char filename[]);	//���ļ�����ͼ
	bool save(const char filename[]);	//���浽�ļ�

	int nodes_size()	//���ؽ�����
	{ return nodes.size(); }

	void addEdge(int x, int y, double w = 1);	//���һ����(x,y)

	void subGraph(vector<int> nodes, Graph &sub);	//���ݽ����ȡ��ͼ ������sub��

	void removeNodes(vector<int> &nodes);	//��ͼ��ɾȥ���


	void bfs(vector<int> & v, int id, int depth = 1);	//��id��ʼ������ȱ����Ľ�㱣�浽v�� Ŀǰֻ֧��depthΪ1
	

	bool isClique();	//�ж�ͼ�ǲ���һ����

	void deleteNodes(Node * p);
	void deleteNode(Node * p);
	void clear();
	void clear(Node & n);

	

	int getMinDegreeNodes(vector<int> &v);	//�õ�ͼ�ж���С�Ľ�� ���浽v�� ���ؽ�����

	void getNodes(vector<int> &v);	//�õ�ͼ�����н��id
	
	
	
	void print();
	bool create_dot_file(char *outfilename);
};


#endif