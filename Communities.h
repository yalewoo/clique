#ifndef COMMUNITIES_H
#define COMMUNITIES_H

#include <vector>

using std::vector;

struct Community
{
	vector<int> v;

	Community()
	{
		v.clear();
	}
	void add(int n)
	{
		v.push_back(n);
	}
};
struct Communities
{
	vector< Community > v;


	Communities()
	{
		v.clear();
	}
	void add(Community c)
	{
		v.push_back(c);
	}
	void print();
	int size() { return v.size();  }

	vector<int> &  getComminuty(int n);
};


#endif