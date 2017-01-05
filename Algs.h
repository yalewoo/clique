#ifndef ALGS_H
#define ALGS_H

#include "Graph.h"
#include "Communities.h"

#include <string>
#include <map>
using std::string;
using std::map;

void systemCall(string cmd);


class Algs
{
	map<string, string> config;
	bool loadConfig(char* filename);
public:

	Algs(char filename[] = "config");
	void runInfoMap(Graph g, char *outdir);
	void runInfoMap(char *filename, char *outdir);
	void Algs::readInfoMapResult(char *filename, Communities &cs);
};

#endif