#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include <list>
#include <iterator>
#include <vector>
#include <map>
#include <math.h>
#include <algorithm>
#include <cctype>
#include "m-ary_tree_structure.cpp"
#include "data_importation.cpp"
using namespace std;

void createNodes(CSVReader* data, Node* root)
{
	for (auto const it = data->dataList.begin(); it != data->dataList.end(); it++)
	{
		//census data:		0 census2010Pop, 1 stateName, 2 countyName
		//activity data:	3 totalCases, 4 weeklyCases, 5 monthlyCases, 6 totalDeaths, 7 weeklyDeaths, 8 monthlyDeaths, 9 stateBoolean

		string stateName;
		string countyName;
		string stateBoolean;
		vector<int> stats;

		stats.push_back(it[0]);
		stateName = it[1];
		countyName = it[2];
		stats.push_back(0);		//PLACEHOLDER FOR DENSITY. Still not sure how to calculate this?
		stats.push_back(it[3]);
		stats.push_back(it[6]);
		stats.push_back(it[4]);
		stats.push_back(it[5]);
		stats.push_back(it[7]);
		stats.push_back(it[8]);
		stateBoolean = (it[9] == "true");
		root->insert(root, countyName, stateName, stateBoolean, stats);
	}
}

int main()
{
	CSVReader* data = new CSVReader("Census Data and Pop. Estimates.csv", "COVID-19-Activity Cleaned.csv", "SoonToBeNamedDeathFile");
	Node* root = new Node();
	createNodes(data, root);
}