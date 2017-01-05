#include <iostream>

#include "Graph.h"
#include "Communities.h"
#include "Algs.h"

#include <iostream>
using std::cout;
using std::endl;
#include <cstdio>
#include <ctime>

void print(const vector<int> &v)
{
	for (int i = 0; i < v.size(); ++i)
		cout << v[i] << " ";
	cout << "*****" << endl;
}
void output(FILE *fp, vector<int> &v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		fprintf(fp, "%d ", v[i]);
		cout << v[i] << " ";
	}
	fprintf(fp, "\n");
	cout << endl;

	//if (v.size() == 0)
	{
		//getchar();
	}
}

void findClique()
{
	Graph g("graph/my.txt");

	cout << "load ok" << endl;

	//g.print();


	FILE *fp = fopen("output.txt", "w");
	vector<int> v;
	vector<int> subnodes;
	Graph g2;
	g.getNodes(v);

	while (g.nodes_size() > 0)
	{
		int i;
		for (i = 0; i < v.size(); ++i)
		{
			g.bfs(subnodes, v[i]);
			g.subGraph(subnodes, g2);
			if (g2.isClique())
			{
				output(fp, subnodes);
				g.removeNodes(subnodes);
				g.getNodes(v);
				break;
			}
		}
		if (v.size() == 0 || i == v.size() + 1)
			break;
	}

	fclose(fp);
}
int main()
{
	Graph g("graph/graph.pairs");
	//g.print();
	

	

	/*
	vector<int> v;
	//g.bfs(v, 3);
	g.getMinDegreeNodes(v);
	print(v);

	g.removeNodes(v);
	g.print();

	g.getMinDegreeNodes(v);
	print(v);

	g.removeNodes(v);
	g.print();

	g.getMinDegreeNodes(v);
	print(v);
	g.removeNodes(v);
	g.print();
	*/
	


	//Graph g1;
	//vector<int> v = {7,2,3,4,5};
	//g.subGraph(v, g1);

	//g1.print();

	Algs algs;
	
	Communities c;

	//algs.runInfoMap("graph/my.txt", "tmp");
	algs.readInfoMapResult("tmp/graph.tree", c);
	c.print();
	//cout << g1.isClique();
	FILE *fp = fopen("output.txt", "w");


	time_t start, end;
	start = time(NULL);//or time(&start);  
	//…calculating…  
	

	for (int i = 0; i < c.size(); ++i)
	{
		//依次处理每个社团

		//subnodes是一个社团的所有点
		vector<int> & subnodes = c.getComminuty(i);
		//print(v);
		if (subnodes.size() < 3)
			continue;

		//g2是这个社团节点构成的子图
		Graph g2;
		g.subGraph(subnodes, g2);

		vector<int> v;
		//g.bfs(v, 3);
		g2.getMinDegreeNodes(v);
		while (v.size() < g2.nodes_size())
		{
			g2.removeNodes(v);
			g2.getMinDegreeNodes(v);
		}
		if (g2.nodes_size() > 2 && g2.isClique())
		{
			//print(subnodes);
			end = time(NULL);
			

			printf("time=%.0lf, %.3lf%%:", difftime(end, start), 100 * double(i) / c.size());
			output(fp, v);
		}
			


	}
	

	return 0;
}