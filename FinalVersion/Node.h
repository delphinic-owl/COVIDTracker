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
#include "data_importation.h"
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

public:

    string name;
    map<string, Node*> nodeMap = {};
    vector<Node*> nodeVector = {};
    vector<string> stateNames = { "Alabama", "Alaska", "American Samoa", "Arizona", "Arkansas", "California", "Colorado", "Connecticut", "Delaware", "District of Columbia", "Florida", "Georgia", "Guam", "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas", "Kentucky", "Louisiana", "Maine", "Maryland", "Massachusetts", "Michigan", "Minnesota", "Minor Outlying Islands", "Mississippi", "Missouri", "Montana", "Nebraska", "Nevada", "New Hampshire", "New Jersey", "New Mexico", "New York", "North Carolina", "North Dakota", "Northern Mariana Islands", "Ohio", "Oklahoma", "Oregon", "Pennsylvania", "Puerto Rico", "Rhode Island", "South Carolina", "South Dakota", "Tennessee", "Texas", "U.S. Virgin Islands", "Virgin Islands", "Utah", "Vermont", "Virginia", "Washington", "West Virginia", "Wisconsin", "Wyoming" };
    //Vector contains all names of states for input

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

    ~Node()
    {
        for (auto it = nodeVector.begin(); it != nodeVector.end(); it++)
        {
            delete *it;
        }
    }

    //Accessors
    Node* stateNode(string staname) { return nodeMap[staname]; };
    Node* countyNode(string couname) { return nodeMap[couname]; };
    vector<int> stats() { return this->statistics; }
    double prime() { return this->primeVar; }
    double severity() { return this->relSeverity; }
    void assignSeverity(double severity) { this->relSeverity = severity; }

    //for assigning stats to a preexisting node
    void assignStats(vector<int> stats)
    {
        //Stats vector should be initialized with a size equal to the max number of stats. Stats can be placed into vector on importation in the correct order (i.e. "stats[0] = storedInput; stats[3] = storedInput;")
        //This function uses vectors with 6 INTS ONLY. No less.
        int i = 0;
        this->statistics.clear();
        this->statistics.push_back(stats[i]);
        this->pop = stats[i++];
        this->statistics.push_back(stats[i]);
        this->cases = stats[i++];
        this->statistics.push_back(stats[i]);
        this->deaths = stats[i++];
        this->statistics.push_back(stats[i]);
        this->primeVar = stats[i++];
        this->statistics.push_back(stats[i]);
        this->monthlyCases = stats[i++];
        this->statistics.push_back(stats[i]);
        this->weeklyDeaths = stats[i++];
        this->statistics.push_back(stats[i]);
        this->monthlyDeaths = stats[i++];
        this->statistics.push_back(stats[i]);
        this->density = stats[i++];
        this->relSeverity = 0.0;
    };

    void assignStats(Node* root, string name, string stateName, vector<int> stats)
    {
        //Stats vector should be initialized with a size equal to the max number of stats. Stats can be placed into vector on importation in the correct order (i.e. "stats[0] = storedInput; stats[3] = storedInput;")
        //This function uses vectors with 6 INTS ONLY. No less.
        int i = 0;
        root->statistics.clear();
        root->name = name;
        root->stateName = stateName;
        root->statistics.push_back(stats[i]);
        root->pop = stats[i++];
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
        root->statistics.push_back(stats[i]);
        root->density = stats[i++];
        root->relSeverity = 0.0;
    };

    void insertNode(Node* root, string name, string stateName, vector<int> stats)
    {
        Node* temp = new Node();
        assignStats(temp, name, stateName, stats);
        temp->isCountry = false;
        temp->rootNode = root;

        bool hasStateName = (find(root->stateNames.begin(), root->stateNames.end(), name) != root->stateNames.end());
        bool namesMatch = (stateName == name);
        bool doesNotExist = (root->nodeMap.find(name) == root->nodeMap.end());

        if (hasStateName && namesMatch && doesNotExist)
        {
            temp->isState = true;
            root->nodeVector.push_back(temp);
            root->nodeMap[name] = temp;
        }
        else //Not a state, is a county
        {
            temp->isState = false;
            //In rootNode's vector, find state node pointer and push temp back into its node pointer vector
            root->nodeMap[stateName]->nodeVector.push_back(temp);
            root->nodeMap[stateName]->nodeMap[name] = temp;
        }
    };

    void calcTotals(Node* root, string stateName);
    void calcTotalSeverity(Node* root);
    void printSeverity(Node* root, bool country, bool state);
    void createNodes(CSVReader1* data, Node* root);
    //Prints total stats for one region 
    void PrintTotals();      
    //Prints out all counties under a state
    void PrintAllCounties();  
    //Compares total stats of two states
    void CompareRegions(Node& region1, Node& region2);   
};
