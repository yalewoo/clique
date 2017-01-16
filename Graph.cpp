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

//按照结点编号比较大小
bool Node::operator<(const Node & n)
{
	return id < n.id;
}
//判断结点编号是否相等
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

//从文件打开图
Graph::Graph(char filename[], bool is_directed, bool is_weighted)
{
	is_directed_graph = is_directed;
	is_weighted_graph = is_weighted;

	load(filename);
}

//从文件打开图
bool Graph::load(const char filename[])
{	
	FILE * fp = fopen(filename, "r");
	if (!fp)
		return false;

	int x, y;
	double w;

	t.resize(MAX_NODE_NUM);

	//有权图每行读入3个数 无权图每行读入2个数 
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
//保存到文件
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


//添加一条边(x,y) ，权值为w
void Graph::addEdge(int x, int y, double w)
{
	Node *p1 = &t[x];
	Node *p = t[x].next;
	while (p && p->id < y)
	{
		p1 = p;
		p = p->next;
	}
	if (p && p->id == y)		//要插入的边已经有了
		return;


	Node * node = new Node(y, w, p1, p);
	p1->next = node;
	if (p)
		p->pred = node;

	++t[x].degree;
	if (t[x].degree == 1)	//第一次添加包含结点x的边时，x加入结点列表
	{
		nodes.push_back(x);
	}


	//对于无权图 每条边对称存储两份 两份之间互相连接
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


		//一条边对应的两个结点互相连接 这是为了删除时的方便
		node->another = node2;
		node2->another = node;
	}
}

//从图中删去结点
void Graph::removeNodes(vector<int> &v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		vector<int>::iterator iter = find(nodes.begin(), nodes.end(), v[i]);
		if (iter != nodes.end())
		{
			nodes.erase(iter);
		}

		//删除结点v[i]连接的所有边
		Node *p = t[v[i]].next;
		deleteNodes(p);
		clear(t[v[i]]);
	}
}




/*
根据结点提取子图 保存在清空后的sub中 
子图中度为0的结点被忽略了
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



//从id开始广度优先遍历的结点保存到v中 目前只支持depth为1
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

//判断图是不是一个团
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

//得到图的所有结点id
void Graph::getNodes(vector<int> &v)
{
	v = nodes;
}




//删除p和p后面的所有结点
void Graph::deleteNodes(Node * p)
{
	while (p)
	{
		Node * p1 = p;
		p = p->next;
		deleteNode(p1);
	}
}

//删除一个表结点 如果是无向图 该边对称的表结点也会被删除
void Graph::deleteNode(Node *p)
{
	if (!is_directed_graph)
	{
		if (p)
		{
			p->pred->next = p->next;
			if (p->next)
				p->next->pred = p->pred;

			Node *p1 = p->another;	//p1指向对称的另一条边

			Node * phead = p->pred;
			while (phead && phead->pred)
				phead = phead->pred;
			//至此，phead指向p的表头结点

			delete p;
			--phead->degree;

			//删除p1结点（删除另一条边）
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
//清空整个图 释放内存 清空表头结点和结点数组
void Graph::clear()
{
	for (int i = 0; i < t.size(); ++i)
	{
		deleteNodes(t[i].next);
	}
	t.clear();
	nodes.clear();
}
//清空某个表头结点 全部设置为0
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


//得到图中度最小的结点 保存到v中 返回结点个数
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

	if (is_weighted_graph)	//有权图
	{
		if (!is_directed_graph)	//无向图 有权
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
		else //有向图 有权
		{

		}
	}
	else
	{
		if (!is_directed_graph)	//无向图 无权图
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
		else //有向图 无权图
		{

		}

	}

	fclose(fp);
}