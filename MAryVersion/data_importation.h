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
#include "timer.h"
#pragma once
using namespace std;

class CSVReader
{
private:
    vector<string> fileNames;
    string delimiter = "";
    int linesRead = 0;
    map<string, int> countyVecMap;
    int testNum = 0;

public:

    vector<vector<string>> dataList;
    vector<vector<string>> incompleteList;

    CSVReader(string censusFileName, string activityFileName, string delm = ",")
    {
        fileNames = { censusFileName, activityFileName };
        delimiter = delm;
    };

    void getData(int fileNum);
    vector<string> getActiveData(ifstream &file);
    void printData()
    {
        //census data:		0 stateName, 1 countyName, 2 2019 pop. Estimate
        //activity data:	3 totalCases, 4 weeklyCases, 5 monthlyCases, 6 totalDeaths, 7 weeklyDeaths, 8 monthlyDeaths, 9 stateBoolean
        int width = 25;
        cout << left << setw(width) << "State Name";
        cout << left << setw(width) << "County Name";
        cout << left << setw(width) << "Population";
        cout << left << setw(width) << "Total Cases";
        cout << left << setw(width) << "Weekly Cases";
        cout << left << setw(width) << "Monthly Cases";
        cout << left << setw(width) << "Total Deaths";
        cout << left << setw(width) << "Weekly Deaths";
        cout << left << setw(width) << "Monthly Deaths";
        cout << left << setw(width) << "State Boolean";
        cout << endl;
        int currentDataPoint = 0;
        for (auto it = dataList.begin(); it != dataList.end(); it++)
        {
            for (auto it2 = it->begin(); it2 != it->end(); it2++)
            {
                if (currentDataPoint != 1 && currentDataPoint != 11)
                    cout << left << setw(width) << *it2;
                currentDataPoint++;
            }
            currentDataPoint = 0;
            cout << "\n";
        }
    }
};
