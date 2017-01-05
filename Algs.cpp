#include "Algs.h"


#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void systemCall(string cmd)
{
	fprintf(stderr, "%s\n", cmd.c_str());
	system(cmd.c_str());
}

Algs::Algs(char filename[])
{
	loadConfig(filename);
}


bool Algs::loadConfig(char* filename)
{
	FILE* fin = fopen(filename, "r");
	config.clear();
	char name[1000], value[1000];
	while (fscanf(fin, "%s%s", name, value) == 2)
		config[name] = value;
	fclose(fin);

	return true;
}


void Algs::runInfoMap(Graph g, char *outdir)
{
	
	string inputfile = config["Temp_Dir"] + "graph";
	g.save(inputfile.c_str());
	string outputdir = outdir;

	string tags = " -i link-list";

#ifdef WIN32
	systemCall((config["Infomap_Path"] + " " + inputfile + " " + outputdir + tags).c_str());
#else
	systemCall((config["Infomap_Dir"] + "Infomap " + inputfile + " " + outputdir + tags).c_str());
#endif;	

}

void Algs::runInfoMap(char *filename, char *outdir)
{

	string inputfile = filename;
	string outputdir = outdir;

	string tags = " -i link-list";

#ifdef WIN32
	systemCall((config["Infomap_Path"] + " " + inputfile + " " + outputdir + tags).c_str());
#else
	systemCall((config["Infomap_Dir"] + "Infomap " + inputfile + " " + outputdir + tags).c_str());
#endif;

}


void Algs::readInfoMapResult(char *filename, Communities &cs)
{
	FILE *fp = fopen(filename, "r");
	if (!fp)
		return;
	char buff[512];
	fgets(buff, 512, fp);
	fgets(buff, 512, fp);

	int n;
	int lastn = 0;
	int node;
	int nc = -1;

	while (fgets(buff, 512, fp))
	{
		//定位到倒数第二个数字 三层时1:2:3 会读出2
		int i = 0;
		while (buff[i] != ' ')
			++i;
		while (buff[i] != ':')
			--i;
		--i;
		while (isdigit(buff[i]))
		{
			--i;
			if (i == -1)
				break;
		}

		++i;
		//社团编号
		sscanf(buff + i, "%d", &n);

		i = strlen(buff);
		while (buff[i] != ' ')
			--i;
		++i;
		//节点号
		sscanf(buff + i, "%d", &node);

		if (n != lastn)	//开始新的社团
		{
			++nc;
			Community c;
			c.add(node);
			cs.add(c);
		}
		else
		{
			cs.v[nc].add(node);
		}
		lastn = n;
	}
}