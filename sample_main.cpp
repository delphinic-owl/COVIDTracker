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

void calcTotals(Node* root, string stateName)
{
    Node* temp = nullptr;
    vector<int> stats = { 0, 0, 0, 0, 0, 0, 0, 0 };
    if (stateName == root->name || stateName == "country")  //If input is country, first calculate all state totals
    {
        temp = root;
        for (auto it = root->nodeVector.begin(); it != root->nodeVector.end(); it++)
        {
            calcTotals(root, (*it)->name);
            for (int i = 0; i < 8; i++)
                stats[i] += (*it)->stats()[i];    //Totals all state stats
        }
        root->assignStats(stats);
    }
    if (root->nodeMap.find(stateName) != root->nodeMap.end())
    {
        temp = root->nodeMap[stateName];  //If input is state, calculate its totals

        for (auto it = temp->nodeVector.begin(); it != temp->nodeVector.end(); it++)
        {
            vector<int> localStats = (*it)->stats();
            for (int i = 0; i < 8; i++)
                stats[i] += (*it)->stats()[i];    //Totals all county stats
        }
        temp->assignStats(stats);
    }
}

void calcTotalSeverity(Node* root)
{
    auto states = root->nodeVector;
    double primeTotal = 100.00;
    for (auto it = states.begin(); it != states.end(); it++)
    {
        auto counties = (*it)->nodeVector;
        for (auto it2 = counties.begin(); it2 != counties.end(); it2++)
        {
            (*it2)->assignSeverity((((double)(*it2)->prime()) / primeTotal) * 100.00); //Weekly cases in county divided by weekly cases everywhere multiplied by 100. Each severity acts as a percentage.
        }
    }
}

void printSeverity(Node* root, bool country, bool state)
{
    if (country || state)   //For country or state, run through nodeVector
    {
        cout << "Total Statistics for " << root->name << endl;
        cout << "Region Name\tPopulation\tTotal Cases\tTotal Deaths\tAverage Weekly Cases\tAverage Monthly Cases\t Average Weekly Deaths\t Average Monthly Deaths\tCase Density\tRelative Severity Score" << endl;
        for (auto it = root->nodeVector.begin(); it != root->nodeVector.end(); it++)
        {
            if (country)
                printSeverity(*it, false, true);
            else
                printSeverity(*it, false, false);
        }
        cout << endl;
    }
    else //if running through counties, simply print stats
    {
        int i = 0;
        auto stats = root->stats();
        cout << root->name << "\t" << stats[i++] << "\t" << stats[i++] << "\t" << stats[i++] << "\t" << stats[i++] << "\t" << stats[i++] << "\t" << stats[i++] << "\t" << stats[i++] << "\t" << stats[i++] << "\t" << root->severity() << "\t" << endl;
    }
}
/*
void Node::printSeverity(vector<Node*> vec, string arg)
{
    //Header defines where the list was retrieved from (search argument). This may be adjusted to print the values from the search as well.
    cout << "Displaying all regions within the " << arg << " specifications" << endl;
    cout << "Region Name\tPopulation\tDensity\tTotal Cases\tTotal Deaths\tAverage Weekly Cases\tAverage Monthly Cases\t Average Weekly Deaths\t Average Monthly Deaths\tRelative Severity Score" << endl;

    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        if ((*it)->isState) //if state, print state header
        {
            cout << "\nState of " << (*it)->name << endl;
        }
        //Function can be adjusted to only print the relevant statistics, or can continue to display all.
        cout << (*it)->name << "\t" << (*it)->pop << "\t" << (*it)->density << "\t" << (*it)->cases << "\t" << (*it)->deaths << "\t" << (*it)->primeVar << "\t" << (*it)->monthlyCases << "\t" << (*it)->weeklyDeaths << "\t" << (*it)->monthlyDeaths << "\t" << (*it)->relSeverity << "\t" << endl;
    }
}

//This function is exclusively used to convert a string into an integer for the searchStat function. If a string is not used, this function can be bypassed
vector<Node*> Node::search(string arg, string region, int min, int max, int numberOfCounties)
{
    vector<Node*> nullVec;
    vector<string> opt = { "population", "density", "cases", "deaths", "weekly cases", "monthly cases" , "weekly deaths", "monthly deaths" , "severity score" };
    auto indexIt = opt.begin();
    while (*indexIt != arg || indexIt != opt.end()) { indexIt++; }
    int stat;
    if (indexIt != opt.end())
        stat = distance(opt.begin(), indexIt);
    else
        return nullVec; //Return null vector if input arg was not found
    return searchStat(stat, region, min, max, numberOfCounties);
}

vector<Node*> Node::searchStat(int arg, string region, int min, int max, int numCounties)
{
    vector<Node*> vecList;
    if (region == rootNode->name || region == "country")  //if region is country
    {
        vector<Node*> vec = rootNode->nodeVector;
        vecList.push_back(rootNode);
        for (auto it = vec.begin(); it != vec.end(); it++)
        {
            vector<Node*> temp = searchStat(arg, (*it)->name, min, max, numCounties);   //Runs through each state, then runs through the returned vector to add each county and state to the total list
            for (auto it2 = temp.begin(); it2 != temp.end(); it2++)
            {
                vecList.push_back(*it2);
            }
        }
    }
    auto indexIt = stateNames.begin();
    while (*indexIt != region || indexIt != stateNames.end()) { indexIt++; };
    if (indexIt != stateNames.end()) //if region is a state
    {
        Node* temp = stateNode(region);
        vector<Node*> vec = temp->nodeVector;
        vecList.push_back(temp);
        for (auto it = vec.begin(); it != vec.end(); it++)
        {
            //first argument tests if arg equals 5 to determine whether to check for double severity score or integer stats. Second ensures only the max number of counties are added (not including state).
            if (arg == 8 && vecList.size() < numCounties + 1 && (*it)->relSeverity >= min && (*it)->relSeverity <= max)
                vecList.push_back(*it);
            if (arg != 8 && vecList.size() < numCounties + 1 && (*it)->statistics[arg] >= min && (*it)->statistics[arg] <= max)
                vecList.push_back(*it);
        }
    }
    return vecList;
}
*/
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
            stats.push_back(((double)stats[3] / (double)stats[2]) * 10000.00);     //Density = total cases / population * 10,000 people  AKA: Cases per 10,000 people
        }

        if (stateBoolean || test.size() >= 4)   //Exclude counties with incomplete data
            root->insertNode(root, countyName, stateName, stats);
        i++;
    }
}

void testPrint(Node* root)
{
    for (auto it = root->nodeVector.begin(); it != root->nodeVector.end(); it++)
    {
        cout << (*it)->name << endl;
        for (auto it2 = (*it)->nodeVector.begin(); it2 != (*it)->nodeVector.end(); it2++)
        {
            cout << (*it2)->name << endl;
        }
        cout << "\n" << endl;
    }

}

int main()
{
    Node* root = new Node();
    root->name = "USA";

    CSVReader* data = new CSVReader("Census Data and Pop. Estimates.csv", "COVID19 Activity Cleaned.csv");
    root->nodeVector.clear();
    root->nodeMap.clear();
    data->getData(0);
    createNodes(data, root);

    calcTotals(root, "country");
    calcTotalSeverity(root);
    printSeverity(root, true, false);
    cout << "All done" << endl;
    //data->printData();
    //root->calcTotalSeverity(root);
    //root->printSeverity(root, true);
}