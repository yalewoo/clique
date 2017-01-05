#include "Graph.h"

#include <algorithm>
#include <iostream>
using std::swap;
using std::sort;
using std::unique;
using std::find;
using std::cout;
using std::endl;

bool Edge::operator<(Edge & e)
{
	if (x < e.x)
		return true;
	else if (x == e.x)
		return y < e.y;
	else
		return false;
}
bool Edge::thisEdgeIsInNodesList(const vector<int> &v)
{
	if (find(v.begin(), v.end(), x) != v.end())
		if (find(v.begin(), v.end(), y) != v.end())
			return true;
	return false;
}
bool Edge::thisEdgeIsInNodesList1(const vector<int> &v)
{
	if (find(v.begin(), v.end(), x) != v.end())
		return true;
	if (find(v.begin(), v.end(), y) != v.end())
		return true;
	return false;
}
bool Edge::thisEdgeHasNode(int n)
{
	if (x == n || y == n)
		return true;
	else
		return false;
}

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
			cout << y / 334863.0 << endl;
		}
	}

	sort(nodes.begin(), nodes.end());

}
void Graph::addEdge(int x, int y, double w)
{
	if (is_directed_graph) //有向图 直接存
	{
		edges.push_back(Edge(x, y, w));

		vector<Node>::iterator iter = find(nodes.begin(), nodes.end(), x);
		if (iter == nodes.end())	//之前没有
		{
			nodes.push_back(Node(x, 1, 0, 1));
		}
		else
		{
			++iter->outdegree;
			++iter->degree;
		}
		iter = find(nodes.begin(), nodes.end(), y);
		if (iter == nodes.end())	//之前没有
		{
			nodes.push_back(Node(y, 1, 1, 0));
		}
		else
		{
			++iter->indegree;
			++iter->degree;
		}

	}
	else //无向图 让x<y
	{
		if (x > y)
			swap(x, y);
		edges.push_back(Edge(x, y, w));

		vector<Node>::iterator iter = find(nodes.begin(), nodes.end(), x);
		if (iter == nodes.end())	//之前没有
		{
			nodes.push_back(Node(x, 1));
		}
		else
		{
			++iter->degree;
		}
		iter = find(nodes.begin(), nodes.end(), y);
		if (iter == nodes.end())	//之前没有
		{
			nodes.push_back(Node(y, 1));
		}
		else
		{
			++iter->degree;
		}
	}
	

}
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

bool Graph::save(const char filename[])
{
	FILE * fp = fopen(filename, "w");
	if (!fp)
		return false;

	sort(edges.begin(), edges.end());
	for (int i = 0; i < edges.size(); ++i)
	{
		if (is_weighted_graph)
		{
			fprintf(fp, "%d\t%d\t%lf\n", edges[i].x, edges[i].y, edges[i].w);
		}
		else
		{
			fprintf(fp, "%d\t%d\n", edges[i].x, edges[i].y);
		}
	}
	fclose(fp);
	

	sort(nodes.begin(), nodes.end());
	vector<Node>::iterator iter = unique(nodes.begin(), nodes.end());
	nodes.erase(iter, nodes.end());
}

//子图中度为0的结点被忽略了
void Graph::subGraph(vector<int> nodes, Graph &sub)
{
	sub.clear();
	sub.is_directed_graph = this->is_directed_graph;
	sub.is_weighted_graph = this->is_weighted_graph;

	for (int i = 0; i < edges.size(); ++i)
	{
		if (edges[i].thisEdgeIsInNodesList(nodes))
			sub.addEdge(edges[i].x, edges[i].y, edges[i].w);
	}
	sort(nodes.begin(), nodes.end());
}
void Graph::removeNodes(vector<int> &v)
{
	vector<Edge>::iterator iter = edges.begin();
	while (iter != edges.end())
	{
		if (iter->thisEdgeIsInNodesList1(v))
		{
			iter = removeEdge(iter);
		}
		else
		{
			++iter;
		}
	}

	vector<Node>::iterator it = nodes.begin();
	while (it != nodes.end())
	{
		if (find(v.begin(), v.end(), it->id) != v.end())
		{
			it = nodes.erase(it);
		}
		else
		{
			++it;
		}
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
		vector<Edge>::iterator iter;
		for (iter = edges.begin(); iter != edges.end(); ++iter)
		{
			if (iter->x == id)
				v.push_back(iter->y);
			else if (iter->y == id)
				v.push_back(iter->x);
		}
	}

	sort(v.begin(), v.end());
	vector<int>::iterator iter = unique(v.begin(), v.end());
	v.erase(iter, v.end());
}

bool Graph::isClique()
{
	int d = nodes.size() - 1;
	vector<Node>::iterator iter = nodes.begin();
	while (iter != nodes.end())
	{
		if (iter->degree != d)
			return false;

		++iter;
	}
	return true;
}

void Graph::getNodes(vector<int> &v)
{
	v.clear();
	vector<Node>::iterator iter = nodes.begin();
	while (iter != nodes.end())
	{
		v.push_back(iter->id);

		++iter;
	}
}


void Graph::clear()
{
	edges.clear();
	nodes.clear();
}

void Graph::calcDegree()
{

}

int Graph::getMinDegreeNodes(vector<int> &v)
{
	vector<Node>::iterator iter = nodes.begin();
	int mindegree = INT_MAX;
	for (iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		if (iter->degree < mindegree)
			mindegree = iter->degree;
	}
	v.clear();
	for (iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		if (iter->degree == mindegree)
			v.push_back(iter->id);
	}

	return mindegree;
}

void Graph::print()
{
	vector<Node>::iterator iter;
	for (iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		cout << iter->id << " " << iter->degree << endl;
	}
	cout << endl;
}