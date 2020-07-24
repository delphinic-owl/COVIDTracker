#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
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
#pragma once
using namespace std;

class CSVReader
{
private:
	vector<string> fileNames;
	string delimiter;
	int linesRead;
	map<string, int> countyVecMap;
	int testNum = 0;

public:

	vector<vector<string>> dataList;
	vector<vector<string>> incompleteList;

	CSVReader(string censusFileName, string activityFileName, string deathFileName, string delm = ",")
	{
		fileNames = { censusFileName, activityFileName, deathFileName };
		delimiter = delm;
	};

	void getData(int fileNum);
	vector<string> getActiveData(ifstream &file);
	void printData() 
	{
		//census data:		0 census2010Pop, 1 stateName, 2 countyName
		//activity data:	3 totalCases, 4 weeklyCases, 5 monthlyCases, 6 totalDeaths, 7 weeklyDeaths, 8 monthlyDeaths, 9 stateBoolean
		cout << "Census 2010 Pop\tState Name\tCounty Name\tTotal Cases\tWeekly Cases\tMonthly Cases\tTotal Deaths\tWeekly Deaths\tMonthly Deaths\tState Boolean" << endl;
		for (auto it = dataList.begin(); it != dataList.end(); it++)
		{
			for (auto it2 = it->begin(); it2 != it->end(); it2++)
			{
				cout << *it2 << "\t";
			}
			cout << "\n";
		}
	}
};
