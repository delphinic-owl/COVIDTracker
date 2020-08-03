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
using namespace std;
//Node functions





//The input n repesents the index of the statistic to sort by in descending order
// 0 = pop; 1 = density; 2 = cases; 3 = deaths; 4 = primeVar (weekly cases); 5 monthlyCases; 6 weeklyDeaths; 7 monthlyDeaths
// Can be changed to sort by severity score (see searchStat for example)
void Node::sort(int n, Node* root)
{
    for (int i = 0; i < root->nodeVector.size() - 1; i++)
    {
        int swapped = 0;
        for (int j = 0; j < root->nodeVector.size() - i - 1; j++)
        {
            if (root->nodeVector[j]->statistics[n] < root->nodeVector[j + 1]->statistics[n])
            {
                Node* temp = root->nodeVector[j];
                root->nodeVector[j] = root->nodeVector[j + 1];
                root->nodeVector[j + 1] = temp;
                swapped = 1;
            }
        }
        if (swapped == 0)
            break;
    }
}

void Node::calcTotals(string stateName)
{
    Node* temp = nullptr;
    if (stateName == rootNode->name || stateName == "country")  //If input is country, first calculate all state totals
    {
        temp = rootNode;
        for (auto it = stateNames.begin(); it != stateNames.end(); it++)
        {
            calcTotals(*it);
        }
    }
    else
        temp = stateNode(stateName);  //If input is state, calculate its totals

    vector<int> stats = { 0, 0, 0, 0, 0 };
    for (auto it = temp->nodeVector.begin(); it != temp->nodeVector.end(); it++)
    {
        stats[0] += (*it)->pop;
        stats[1] += (*it)->density;
        stats[2] += (*it)->cases;
        stats[3] += (*it)->deaths;
        stats[4] += (*it)->primeVar;
    }
    assignStats(temp, stateName, stateName, stats);
}

void Node::calcTotalSeverity(Node* root)
{
    auto states = root->nodeVector;
    for (auto it = states.begin(); it != states.end(); it++)    //Calls calctotals on every state first
    {
        calcTotals(it->name);
    }
    double primeTotal = rootNode->primeVar;
    for (auto it = states.begin(); it != states.end(); it++)
    {
        auto counties = (*it)->nodeVector;
        for (auto it2 = counties.begin(); it2 != counties.end(); it2++)
        {
            (*it2)->relSeverity = (((double) (*it2)->primeVar) / primeTotal) * 100.00; //Unsure if this is how we're doing relative severity. weekly cases in county divided by weekly cases everywhere multiplied by 100. Each severity acts as a percentage.
        }
    }
}

void Node::printSeverity(Node* root, bool country)
{
    if (root->isCountry) //if asking for country statistics, print headers, stats, and run function recursively for each state
    {
        cout << "Total Statistics for " << root->name << endl;
        cout << "Region Name\tPopulation\tDensity\tTotal Cases\tTotal Deaths\tAverage Weekly Cases\tAverage Monthly Cases\t Average Weekly Deaths\t Average Monthly Deaths\tRelative Severity Score" << endl;
        cout << root->name << "\t" << root->pop << "\t" << root->density << "\t" << root->cases << "\t" << root->deaths << "\t" << root->primeVar << "\t" << root->monthlyCases << "\t" << root->weeklyDeaths << "\t" << root->monthlyDeaths << "\t" << root->relSeverity << "\t" << endl;
        for (auto it = rootNode->nodeVector.begin(); it != rootNode->nodeVector.end(); it++)
        {
            printSeverity(*it, true);
        }
    }
    else if (!country && root->isState) //if not asking for country stats, print headers, stats, and run function recursively for each county
    {
        cout << "Total Statistics for " << root->name << endl;
        cout << "Region Name\tPopulation\tDensity\tTotal Cases\tTotal Deaths\tAverage Weekly Cases\tAverage Monthly Cases\t Average Weekly Deaths\t Average Monthly Deaths\tRelative Severity Score" << endl;
        cout << root->name << "\t" << root->pop << "\t" << root->density << "\t" << root->cases << "\t" << root->deaths << "\t" << root->primeVar << "\t" << root->monthlyCases << "\t" << root->weeklyDeaths << "\t" << root->monthlyDeaths << "\t" << root->relSeverity << "\t" << endl;
        for (auto it = rootNode->nodeVector.begin(); it != rootNode->nodeVector.end(); it++)
        {
            printSeverity(*it, false);
        }
    }
    else if (country && root->isState)  //if asking for country stats, headers have already been printed, print stats, and run recursively through each county
    {
        cout << "\nState of " << root->name << endl;
        cout << root->name << "\t" << root->pop << "\t" << root->density << "\t" << root->cases << "\t" << root->deaths << "\t" << root->primeVar << "\t" << root->monthlyCases << "\t" << root->weeklyDeaths << "\t" << root->monthlyDeaths << "\t" << root->relSeverity << "\t" << endl;
        for (auto it = rootNode->nodeVector.begin(); it != rootNode->nodeVector.end(); it++)
        {
            printSeverity(*it, false);
        }
    }
    else //if running through counties, simply print stats
    {
        cout << root->name << "\t" << root->pop << "\t" << root->density << "\t" << root->cases << "\t" << root->deaths << "\t" << root->primeVar << "\t" << root->monthlyCases << "\t" << root->weeklyDeaths << "\t" << root->monthlyDeaths << "\t" << root->relSeverity << "\t" << endl;
    }
}

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
