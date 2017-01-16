#include "Graph.h"

#include <algorithm>
#include <iostream>
using std::swap;
using std::sort;
using std::unique;
using std::find;
using std::cout;
using std::endl;

#include <cstdio>
#include <climits>

//���ս���űȽϴ�С
bool Node::operator<(const Node & n)
{
	return id < n.id;
}
//�жϽ�����Ƿ����
bool Node::operator==(const Node & n)
{
	return id == n.id;
}
Node::Node(const Node & node)
{
	id = node.id;
	degree = node.degree;
	indegree = node.indegree;
	outdegree = node.outdegree;
	next = node.next;
	pred = node.pred;
	another = node.another;
}

//���ļ���ͼ
Graph::Graph(char filename[], bool is_directed, bool is_weighted)
{
	is_directed_graph = is_directed;
	is_weighted_graph = is_weighted;

	load(filename);
}

//���ļ���ͼ
bool Graph::load(const char filename[])
{	
	FILE * fp = fopen(filename, "r");
	if (!fp)
		return false;

	int x, y;
	double w;

	t.resize(MAX_NODE_NUM);

	//��Ȩͼÿ�ж���3���� ��Ȩͼÿ�ж���2���� 
	if (is_weighted_graph)
	{
		while (fscanf(fp, "%d%d%lf", &x, &y, &w) == 3)
		{
			addEdge(x, y, w);
		}
	}
	else
	{
		while (fscanf(fp, "%d%d", &x, &y) == 2)
		{
			addEdge(x, y);
		}
	}

}
//���浽�ļ�
bool Graph::save(const char filename[])
{
	FILE * fp = fopen(filename, "w");
	if (!fp)
		return false;
	
	if (is_weighted_graph)
	{
		for (int i = 0; i < nodes.size(); ++i)
		{
			Node *p = t[nodes[i]].next;
			while (p)
			{
				if (!is_directed_graph)
				{
					if (nodes[i] < p->id)
						fprintf(fp, "%d\t%d\t%lf\n", nodes[i], p->id, p->w);
				}
				else
					fprintf(fp, "%d\t%d\t%lf\n", nodes[i], p->id, p->w);

				p = p->next;
			}
		}
		
	}
	else
	{
		for (int i = 0; i < nodes.size(); ++i)
		{
			Node *p = t[nodes[i]].next;
			while (p)
			{
				if (!is_directed_graph)
				{
					if (nodes[i] < p->id)
						fprintf(fp, "%d\t%d\n", nodes[i], p->id);
				}
				else
					fprintf(fp, "%d\t%d\n", nodes[i], p->id);

				p = p->next;
			}
		}
	}
	
	fclose(fp);
}


//���һ����(x,y) ��ȨֵΪw
void Graph::addEdge(int x, int y, double w)
{
	Node *p1 = &t[x];
	Node *p = t[x].next;
	while (p && p->id < y)
	{
		p1 = p;
		p = p->next;
	}
	if (p && p->id == y)		//Ҫ����ı��Ѿ�����
		return;


	Node * node = new Node(y, w, p1, p);
	p1->next = node;
	if (p)
		p->pred = node;

	++t[x].degree;
	if (t[x].degree == 1)	//��һ����Ӱ������x�ı�ʱ��x�������б�
	{
		nodes.push_back(x);
	}


	//������Ȩͼ ÿ���߶Գƴ洢���� ����֮�以������
	if (!is_directed_graph)
	{
		swap(x, y);

		p1 = &t[x];
		p = t[x].next;
		while (p && p->id < y)
		{
			p1 = p;
			p = p->next;
		}
		if (p && p->id == y)
			return;


		Node * node2 = new Node(y, w, p1, p);
		p1->next = node2;
		if (p)
			p->pred = node2;

		++t[x].degree;
		if (t[x].degree == 1)
		{
			nodes.push_back(x);
		}


		//һ���߶�Ӧ��������㻥������ ����Ϊ��ɾ��ʱ�ķ���
		node->another = node2;
		node2->another = node;
	}
}

//��ͼ��ɾȥ���
void Graph::removeNodes(vector<int> &v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		vector<int>::iterator iter = find(nodes.begin(), nodes.end(), v[i]);
		if (iter != nodes.end())
		{
			nodes.erase(iter);
		}

		//ɾ�����v[i]���ӵ����б�
		Node *p = t[v[i]].next;
		deleteNodes(p);
		clear(t[v[i]]);
	}
}




/*
���ݽ����ȡ��ͼ ��������պ��sub�� 
��ͼ�ж�Ϊ0�Ľ�㱻������
*/
void Graph::subGraph(vector<int> nodes, Graph &sub)
{
	sub.clear();
	sub.is_directed_graph = this->is_directed_graph;
	sub.is_weighted_graph = this->is_weighted_graph;

	sub.t.resize(MAX_NODE_NUM);


	for (int i = 0; i < nodes.size(); ++i)
	{
		Node *p = t[nodes[i]].next;
		while (p)
		{
			if (find(nodes.begin(), nodes.end(), p->id) != nodes.end())
			{
				sub.addEdge(nodes[i], p->id, p->w);
			}
			p = p->next;
		}
	}

}



//��id��ʼ������ȱ����Ľ�㱣�浽v�� Ŀǰֻ֧��depthΪ1
void Graph::bfs(vector<int> & v, int id, int depth)
{
	v.clear();
	v.push_back(id);
	if (is_directed_graph)
	{
		//todo

	}
	else
	{
		Node *p = t[id].next;
		while (p)
		{
			v.push_back(p->id);
			p = p->next;
		}
	}
}

//�ж�ͼ�ǲ���һ����
bool Graph::isClique()
{
	int d = nodes.size() - 1;
	
	for (int i = 0; i < nodes.size(); ++i)
	{
		if (t[nodes[i]].degree != d)
			return false;
	}
	return true;
}

//�õ�ͼ�����н��id
void Graph::getNodes(vector<int> &v)
{
	v = nodes;
}




//ɾ��p��p��������н��
void Graph::deleteNodes(Node * p)
{
	while (p)
	{
		Node * p1 = p;
		p = p->next;
		deleteNode(p1);
	}
}

//ɾ��һ������ ���������ͼ �ñ߶ԳƵı���Ҳ�ᱻɾ��
void Graph::deleteNode(Node *p)
{
	if (!is_directed_graph)
	{
		if (p)
		{
			p->pred->next = p->next;
			if (p->next)
				p->next->pred = p->pred;

			Node *p1 = p->another;	//p1ָ��ԳƵ���һ����

			Node * phead = p->pred;
			while (phead && phead->pred)
				phead = phead->pred;
			//���ˣ�pheadָ��p�ı�ͷ���

			delete p;
			--phead->degree;

			//ɾ��p1��㣨ɾ����һ���ߣ�
			if (p1)
			{
				p = p1;

				p->pred->next = p->next;
				if (p->next)
					p->next->pred = p->pred;

				phead = p->pred;
				while (phead && phead->pred)
					phead = phead->pred;
				delete p;
				--phead->degree;

			}
		}
	}
	
}
//�������ͼ �ͷ��ڴ� ��ձ�ͷ���ͽ������
void Graph::clear()
{
	for (int i = 0; i < t.size(); ++i)
	{
		deleteNodes(t[i].next);
	}
	t.clear();
	nodes.clear();
}
//���ĳ����ͷ��� ȫ������Ϊ0
void Graph::clear(Node & n)
{
	n.id = 0;
	n.next = 0;
	n.degree = 0;
	n.indegree = 0;
	n.outdegree = 0;
	n.pred = 0;
	n.another = 0;
}


//�õ�ͼ�ж���С�Ľ�� ���浽v�� ���ؽ�����
int Graph::getMinDegreeNodes(vector<int> &v)
{
	int mindegree = INT_MAX;
	for (int i = 0; i < nodes.size(); ++i)
	{
		if (t[nodes[i]].degree < mindegree)
			mindegree = t[nodes[i]].degree;
	}

	v.clear();
	for (int i = 0; i < nodes.size(); ++i)
	{
		if (t[nodes[i]].degree == mindegree)
			v.push_back(nodes[i]);
	}

	return mindegree;
}




void Graph::print()
{
	cout << nodes.size() << " nodes" << endl;
	for (int i = 0; i < t.size(); ++i)
	{
		if (t[i].degree != 0)
			cout << i << " " << t[i].degree << endl;
	}
	cout << endl;
}

bool Graph::create_dot_file(char *filename)
{
	FILE * fp = fopen(filename, "w");
	if (!fp)
		return false;

	if (is_weighted_graph)	//��Ȩͼ
	{
		if (!is_directed_graph)	//����ͼ ��Ȩ
		{
			for (int i = 0; i < nodes.size(); ++i)
			{
				Node *p = t[nodes[i]].next;
				while (p)
				{
					if (nodes[i] < p->id)
					{
						;
					}
					p = p->next;
				}
			}
		}
		else //����ͼ ��Ȩ
		{

		}
	}
	else
	{
		if (!is_directed_graph)	//����ͼ ��Ȩͼ
		{
			fprintf(fp, "graph g {\n");

			for (int i = 0; i < nodes.size(); ++i)
			{
				Node *p = t[nodes[i]].next;
				while (p)
				{
					if (nodes[i] < p->id)
					{
						fprintf(fp, "\t%d -- %d;\n", nodes[i], p->id);
					}
					p = p->next;
				}
			}
			fprintf(fp, "}");
		}
		else //����ͼ ��Ȩͼ
		{

		}

	}

	fclose(fp);
}