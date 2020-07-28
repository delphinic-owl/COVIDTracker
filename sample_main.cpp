#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <string>
#include <map>
#include <list>
#include <iterator>
#include <vector>
#include <map>
#include <math.h>
#include <algorithm>
#include <cctype>
#include "m-ary_tree_structure.h"
#include "data_importation.h"
using namespace std;

void createNodes(CSVReader* data, Node* root)
{
	int i = 0;
	for (auto it = data->dataList.begin(); it != data->dataList.end(); it++)
	{
		//census data:		0 census2010Pop, 1 stateName, 2 countyName
		//activity data:	3 totalCases, 4 weeklyCases, 5 monthlyCases, 6 totalDeaths, 7 weeklyDeaths, 8 monthlyDeaths, 9 stateBoolean

		string stateName;
		string countyName;
		bool stateBoolean;
		vector<int> stats;
		auto test = *it;
		stateBoolean = (test[2] == "true");
		stateName = test[3];
		countyName = test[4];
        if (!stateBoolean && test.size() < 6)
            stats.push_back(stoi(test[0]));
        else if (!stateBoolean)
		{
			stats.push_back(stoi(test[0]));
			stats.push_back(0);		//PLACEHOLDER FOR DENSITY. Still not sure how to calculate this?
			stats.push_back(stoi(test[5]));
			stats.push_back(stoi(test[6]));
			stats.push_back(stoi(test[7]));
			stats.push_back(stoi(test[8]));
			stats.push_back(stoi(test[9]));
			stats.push_back(stoi(test[10]));
		}
		else
			stats = { 0, 0, 0, 0, 0, 0, 0, 0 };	//fill state data vector with 0s since not all data is found yet
		root->insertNode(root, countyName, stateName, stateBoolean, stats);
		i++;
	}
}

int main()
{
	CSVReader* data = new CSVReader("Census Data and Pop. Estimates.csv", "COVID-19-Activity Cleaned.csv", "SoonToBeNamedDeathFile");
	data->getData(0);
	Node* root = new Node();
	createNodes(data, root);
	data->printData();
	//root->printSeverity(root, true);
}
