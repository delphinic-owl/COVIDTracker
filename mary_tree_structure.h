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
#pragma once
using namespace std;

class Node
{
private:

    string stateName;
    int pop;
    int density;
    int cases;
    int deaths;
    int primeVar;   //Weekly cases
    int monthlyCases;
    int weeklyDeaths;
    int monthlyDeaths;
    vector<int> statistics;
    double relSeverity;
    bool isCountry;
    bool isState;
    Node* rootNode;
    vector<string> stateNames = { "Alabama", "Alaska", "American Samoa", "Arizona", "Arkansas", "California", "Colorado", "Connecticut", "Delaware", "District of Columbia", "Florida", "Georgia", "Guam", "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas", "Kentucky", "Louisiana", "Maine", "Maryland", "Massachusetts", "Michigan", "Minnesota", "Minor Outlying Islands", "Mississippi", "Missouri", "Montana", "Nebraska", "Nevada", "New Hampshire", "New Jersey", "New Mexico", "New York", "North Carolina", "North Dakota", "Northern Mariana Islands", "Ohio", "Oklahoma", "Oregon", "Pennsylvania", "Puerto Rico", "Rhode Island", "South Carolina", "South Dakota", "Tennessee", "Texas", "U.S. Virgin Islands", "Virgin Islands", "Utah", "Vermont", "Virginia", "Washington", "West Virginia", "Wisconsin", "Wyoming" };
    //Vector contains all names of states for input

public:

    string name;
    map<string, Node*> nodeMap;
    vector<Node*> nodeVector;

    //Constructor
    Node()
    {
        name = "";
        stateName = "";
        pop = 0;
        density = 0;     //What is density? Like population over land area? Population over cases?
        cases = 0;
        deaths = 0;
        primeVar = 0;
        monthlyCases = 0;
        relSeverity = 0;
        isCountry = 0;
        isState = 0;
        rootNode = nullptr;
    };

    Node(string name, Node* root)
    {
        this->name = name;
        stateName = "";
        pop = 0;
        density = 0;     //What is density? Like population over land area? Population over cases?
        cases = 0;
        deaths = 0;
        primeVar = 0;
        monthlyCases = 0;
        relSeverity = 0;
        isCountry = 0;
        isState = 0;
        rootNode = root;
    };

    //Copy constructor
    Node(const Node &node)
    {
        name = node.name;
        stateName = node.stateName;
        pop = node.pop;
        density = node.density;     //What is density? Like population over land area? Population over cases?
        cases = node.cases;
        deaths = node.deaths;
        primeVar = node.primeVar;
        monthlyCases = node.monthlyCases;
        relSeverity = node.relSeverity;
        isCountry = node.isCountry;
        isState = node.isState;
        rootNode = node.rootNode;
        nodeMap = node.nodeMap;
        nodeVector = node.nodeVector;
    }

    //Accessors
    Node* stateNode(string staname) { return rootNode->nodeMap[staname]; };
    Node* countyNode(string couname) { return rootNode->nodeMap[couname]->nodeMap[couname]; };

    void assignStats(Node* root, string name, string stateName, vector<int> stats)
    {
        //Stats vector should be initialized with a size equal to the max number of stats. Stats can be placed into vector on importation in the correct order (i.e. "stats[0] = storedInput; stats[3] = storedInput;")
        //This function uses vectors with 6 INTS ONLY. No less.
        int i = 0;
        root->statistics.clear();
        root->name = name;
        root->statistics.push_back(stats[i]);
        root->pop = stats[i++];
        root->statistics.push_back(stats[i]);
        root->density = stats[i++];
        root->statistics.push_back(stats[i]);
        root->cases = stats[i++];
        root->statistics.push_back(stats[i]);
        root->deaths = stats[i++];
        root->statistics.push_back(stats[i]);
        root->primeVar = stats[i++];
        root->statistics.push_back(stats[i]);
        root->monthlyCases = stats[i++];
        root->statistics.push_back(stats[i]);
        root->weeklyDeaths = stats[i++];
        root->statistics.push_back(stats[i]);
        root->monthlyDeaths = stats[i++];
        root->relSeverity = 0.0;
    };

    //This might need to be void?
    //Stats might need to be calculated when reading from file (i.e. density is a calculation, weekly cases might not be stated explicitly, and many reports are over time.
    //Stats may be changed to a vector of a vector of integers to represent stats over time (i.e. March, April, May, June)
    void insertNode(Node* root, string name, string stateName, bool state, vector<int> stats)
    {
        Node* temp = new Node();
        assignStats(temp, name, stateName, stats);
        if (root == nullptr)
        {
            temp->isCountry = true;
            temp->isState = false;
            temp->rootNode = temp;
        }

        temp->isCountry = false;
        temp->rootNode = root;

        if (state)
        {
            temp->isState = true;
            temp->rootNode = root;
            root->nodeVector.push_back(temp);
            root->nodeMap[name] = temp;
        }
        else
        {
            temp->isState = false;
            temp->rootNode = root;
            //In rootNode's vector, find state node pointer and push temp back into its node pointer vector
            root->nodeMap[stateName]->nodeVector.push_back(temp);
            root->nodeMap[stateName]->nodeMap[name] = temp;
        }
    };

    void sort(int n, Node* root);                                                               //Sorts all nodes under the root into descending order by primeVar (weekly cases)
    void calcTotals(string stateName);                                                          //Calculates the totals under the particular region. Accepts "country", country name, or state names.
    void calcTotalSeverity(Node* root);                                                         //Calls the country totals function to ensure all state and country totals are found, then runs through each state->county to find severities based on primeVar (weekly cases)
    void printSeverity(Node* root, bool country);                                               //Prints the name of the root node, its stats, and each region below it. If the rootNode is used, will print every state and county.
    void printSeverity(vector<Node*> vec, string arg);                                          //Similar to the above function, will print the stats for each region in a vector, with states above their counties (if included).
    vector<Node*> search(string arg, string region, int min, int max, int numberOfCounties);    //Search for a particular argument in a region. Only reports the max number of counties per state
    vector<Node*> searchStat(int arg, string region, int min, int max, int numCounties);        //Helper function for search
};
