#include <iostream>

#include "Graph.h"
#include "Communities.h"
#include "Algs.h"

#include <iostream>
using std::cout;
using std::endl;
#include <cstdio>
#include <ctime>

void showimg(Graph & g)
{
	cout << g.nodes_size() << " nodes " << endl;
	g.create_dot_file("tmp\\t.dot");
	g.save("tmp\\ggg.edge");
	Algs algs;
	algs.runGraphviz("tmp\\t.dot", "tmp\\t.gif");
	cout << "press enter to show img" << endl;
	getchar();
	algs.openfile("tmp\\t.gif");


}
void print(const vector<int> &v)
{
	for (int i = 0; i < v.size(); ++i)
		cout << v[i] << " ";
	cout << "*****" << endl;
}
void output(FILE *fp, vector<int> &v)
{
	cout << v.size() << " nodes : " ;
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
	Graph g("graph/graph.pairs");

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
		int lasti = 0;
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
		if (v.size() == 0/* || i == v.size() + 1*/)
			break;
	}

	fclose(fp);
}

void infomap_find_clique(char *file, char *outdir = "tmp")
{
	char treefile[256];
	strcpy(treefile, outdir);
	int n = strlen(treefile);
	
	treefile[n] = '\\';
	treefile[n+1] = '\0';
	
	char treef[256];
	strcpy(treef, treefile);

	strcpy(treefile, file);
	n = strlen(treefile);
	while (n >= 0 && treefile[n] != '.')
		--n;
	treefile[n + 1] = '\0';
	int n2 = n;
	while (n2 >= 0 && (treefile[n2] != '\\' && treefile[n2] != '/'))
		--n2;
	++n2;
	strcat(treefile, "tree");

	strcat(treef, treefile + n2);
	

	Graph g(file);
	

	Algs algs;

	Communities c;

	algs.runInfoMap(file, outdir);
	algs.readInfoMapResult(treef, c);
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
		//showimg(g2);

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

			showimg(g2);

			printf("time=%.0lf, %.3lf%%:", difftime(end, start), 100 * double(i) / c.size());
			output(fp, v);
		}



	}
}
int main()
{
	infomap_find_clique("graph/c125.9.11.12");
	//findClique();
	//Algs algs;
	//algs.runInfoMap("graph/c125.9.11.12", "tmp");
	//algs.runGraphviz("tmp\\g.dot", "tmp\\g.gif");
	//algs.openfile("tmp\\g.gif");

	return 0;
}