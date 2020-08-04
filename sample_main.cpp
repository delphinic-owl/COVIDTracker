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
#include "mary_tree_structure.h"
#include "data_importation.h"
using namespace std;

void createNodes(CSVReader* data, Node* root)
{
    int i = 0;
    for (auto it = data->dataList.begin(); it != data->dataList.end(); it++)
    {
        //census data:		0 stateName, 1 countyName, 2 2019 pop. Estimate
        //activity data:	3 totalCases, 4 weeklyCases, 5 monthlyCases, 6 totalDeaths, 7 weeklyDeaths, 8 monthlyDeaths, 9 stateBoolean

        string stateName;
        string countyName;
        bool stateBoolean = false;
        vector<int> stats;
        auto test = *it;
        stateName = test[0];
        countyName = test[1];
        if (test.size() >= 4 && test[3] == "true")      //State data will get totaled later
        {
            stateBoolean = true;
            stats = { 0, 0, 0, 0, 0, 0, 0, 0 };
        }
        else if (test.size() < 4)                       //If county data is incomplete
            stats.push_back(stoi(test[2])); //pop.
        else                                            //If county has all data
        {
            stats.push_back(stoi(test[2])); //pop.
            stats.push_back(stoi(test[3])); //cases
            stats.push_back(stoi(test[4])); //weeklycases
            stats.push_back(stoi(test[5])); //monthlycases
            stats.push_back(stoi(test[6])); //deaths
            stats.push_back(stoi(test[7])); //weekly deaths
            stats.push_back(stoi(test[8])); //monthly deaths
            stats.push_back(((double)stats[3] / (double)stats[2]) * 1000.00);     //Density = total cases / population * 1000 people  AKA: Cases per 1000 people
        }

        if (stateBoolean || test.size() >= 4)   //Exclude counties with incomplete data
            root->insertNode(root, countyName, stateName, stats);
        i++;
    }
}

int main()
{
    Node* root = new Node();

    CSVReader* data = new CSVReader("Census Data and Pop. Estimates.csv", "COVID19 Activity Cleaned.csv");
    data->getData(0);
    createNodes(data, root);

    cout << "All done" << endl;
    //data->printData();
    //root->calcTotalSeverity(root);
    //root->printSeverity(root, true);
}