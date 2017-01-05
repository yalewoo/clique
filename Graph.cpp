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


bool Node::operator<(const Node & n)
{
	return id < n.id;
}
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


Graph::Graph(char filename[], bool is_directed, bool is_weighted)
{
	is_directed_graph = is_directed;
	is_weighted_graph = is_weighted;

	load(filename);
}

bool Graph::load(const char filename[])
{	
	FILE * fp = fopen(filename, "r");
	if (!fp)
		return false;

	int x, y;
	double w;

	t.resize(MAX_NODE_NUM);

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
			//cout << y / 334863.0 << endl;
		}
	}

}
void Graph::addEdge(int x, int y, double w)
{
	addEdge2(x, y, w);

}
void Graph::addEdge2(int x, int y, double w)
{
	Node *p1 = &t[x];
	Node *p = t[x].next;
	while (p && p->id < y)
	{
		p1 = p;
		p = p->next;
	}
	if (p && p->id == y)
		return;

	
	Node * node = new Node(y, w, p1, p);
	p1->next = node;
	if (p)
		p->pred = node;

	++t[x].degree;
	if (t[x].degree == 1)
	{
		nodes.push_back(x);
		++num_nodes;
	}
		


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
			++num_nodes;
		}
			


		node->another = node2;
		node2->another = node;
	}
}

/*
vector<Edge>::iterator Graph::removeEdge(vector<Edge>::iterator it)
{
	vector<Edge>::iterator res;


	int x = it->x;
	int y = it->y;
	double w = it->w;

	res = edges.erase(it);

	if (is_directed_graph) //有向图 直接存
	{
		vector<Node>::iterator iter = find(nodes.begin(), nodes.end(), x);
		if (iter == nodes.end())	//之前没有
		{
			
		}
		else
		{
			--iter->outdegree;
			--iter->degree;
		}
		iter = find(nodes.begin(), nodes.end(), y);
		if (iter == nodes.end())	//之前没有
		{
			
		}
		else
		{
			--iter->indegree;
			--iter->degree;
		}

	}
	else //无向图 让x<y
	{
		vector<Node>::iterator iter = find(nodes.begin(), nodes.end(), x);
		if (iter == nodes.end())	//之前没有
		{
			
		}
		else
		{
			--iter->degree;
		}
		iter = find(nodes.begin(), nodes.end(), y);
		if (iter == nodes.end())	//之前没有
		{
			
		}
		else
		{
			--iter->degree;
		}
	}

	return res;
}
*/

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
					if (i < p->id)
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
					if (i < p->id)
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

//子图中度为0的结点被忽略了
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
				sub.addEdge2(nodes[i], p->id, p->w);
			}
			p = p->next;
		}
	}

}


void Graph::removeNodes(vector<int> &v)
{
	num_nodes -= v.size();

	for (int i = 0; i < v.size(); ++i)
	{
		vector<int>::iterator iter = find(nodes.begin(), nodes.end(), v[i]);
		if (iter != nodes.end())
		{
			nodes.erase(iter);
		}


		Node *p = t[v[i]].next;


		deleteNodes(p);
		clear(t[v[i]]);

	}

	
}

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

void Graph::getNodes(vector<int> &v)
{
	
	v.clear();
	
	for (int i = 0; i < t.size(); ++i)
	{
		if (t[i].degree != 0)
			v.push_back(i);
	}
	
}

void Graph::deleteNodes(Node * p)
{
	while (p)
	{
		Node * p1 = p;
		p = p->next;
		deleteNode(p1);
	}
}
void Graph::deleteNode(Node *p)
{
	if (!is_directed_graph)
	{
		if (p)
		{
			p->pred->next = p->next;
			if (p->next)
				p->next->pred = p->pred;

			Node *p1 = p->another;
			Node * phead = p->pred;
			while (phead && phead->pred)
				phead = phead->pred;
			delete p;
			--phead->degree;

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
void Graph::clear()
{
	for (int i = 0; i < t.size(); ++i)
	{
		deleteNodes(t[i].next);
	}
	t.clear();
	nodes.clear();
}
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

void Graph::calcDegree()
{

}

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