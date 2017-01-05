#include "Communities.h"
#include <iostream>
using std::cout;
using std::endl;
void Communities::print()
{
	cout << v.size() << " communities" << endl;

	for (int i = 0; i < v.size(); ++i)
	{
		for (int j = 0; j < v[i].v.size(); ++j)
		{
			//cout << v[i].v[j] << " ";
		}
		//cout << endl;
	}

}

vector<int> & Communities::getComminuty(int n)
{
	if (n < v.size())
		return v[n].v;
}