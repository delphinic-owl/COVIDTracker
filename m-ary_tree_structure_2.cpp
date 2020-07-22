#include <iostream>
#include <iomanip>
#include <cstring>
#include <list>
#include <iterator>
#include <vector>
#include <map>
#include <math.h>
#include <algorithm>
using namespace std;

class Node
{
private:

public:
    string name;
    string stateName;
    int pop;
    int density;
    int cases;
    int deaths;
    int primeVar;   //Weekly cases
    double relSeverity;
    bool isCountry;
    bool isState;
    Node* rootNode;
    map<string, Node*> nodeMap;
    vector<Node*> nodeVector;
    vector<string> stateNames = { "Alabama", "Alaska", "American Samoa", "Arizona", "Arkansas", "California", "Colorado", "Connecticut", "Delaware", "District of Columbia", "Florida", "Georgia", "Guam", "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas", "Kentucky", "Louisiana", "Maine", "Maryland", "Massachusetts", "Michigan", "Minnesota", "Minor Outlying Islands", "Mississippi", "Missouri", "Montana", "Nebraska", "Nevada", "New Hampshire", "New Jersey", "New Mexico", "New York", "North Carolina", "North Dakota", "Northern Mariana Islands", "Ohio", "Oklahoma", "Oregon", "Pennsylvania", "Puerto Rico", "Rhode Island", "South Carolina", "South Dakota", "Tennessee", "Texas", "U.S. Virgin Islands", "Utah", "Vermont", "Virginia", "Washington", "West Virginia", "Wisconsin", "Wyoming" };
    //Vector contains all names of states for input

    //Copy constructor
    Node(const Node & node) 
    {
        string name = node.name;
        string stateName = node.stateName;
        int pop = node.pop;
        int density = node.density;
        int cases = node.cases;
        int deaths = node.deaths;
        int primeVar = node.primeVar;
        double relSeverity = node.relSeverity;
        bool isCountry = node.isCountry;
        bool isState = node.isState;
        Node* rootNode = node.rootNode;
        map<string, Node*> nodeMap = node.nodeMap;
        vector<Node*> nodeVector = node.nodeVector;
    }

    //Accessors
    Node* stateNode(string name) { return rootNode->nodeMap[name]; };
    Node* countyNode(string name) { return rootNode->nodeMap[name]->nodeMap[name]; };

    //May want to overwrite this with another function for states
    void assignStats(Node* root, string name, vector<int> stats)
    {
        //Stats vector should be initialized with a size equal to the max number of stats. Stats can be placed into vector on importation in the correct order (i.e. "stats[0] = storedInput; stats[3] = storedInput;")
        //This function uses vectors with 6 INTS ONLY. No less.
        int i = 0;
        root->name = name;
        root->pop = stats[i++];
        root->density = stats[i++];
        root->cases = stats[i++];
        root->deaths = stats[i++];
        root->primeVar = stats[i++];
        root->relSeverity = stats[i++];
    };

    //This might need to be void?
    Node* insert(Node* root, string name, string stateName, bool state, vector<int> stats)
    {
        if (root == nullptr)
        {
            Node* temp = new Node();
            assignStats(temp, name, stats);
            temp->isCountry = true;
            temp->isState = false;
            temp->rootNode = temp;
            return temp;
        }
        if (state)
        {
            Node* temp = new Node();
            assignStats(temp, name, stats);
            temp->isCountry = false;
            temp->isState = true;
            temp->rootNode = root;
            rootNode->nodeVector.push_back(temp);
            rootNode->nodeMap.emplace(name, temp);
            return temp;
        }
        else
        {
            Node* temp = new Node();
            assignStats(temp, name, stats);
            temp->isCountry = false;
            temp->isState = false;
            temp->rootNode = root;
            //In rootNode's vector, find state node pointer and push temp back into its node pointer vector
            stateNode(stateName)->nodeVector.push_back(temp);
            stateNode(stateName)->nodeMap.emplace(name, temp);
            return temp;
        }
        return root;
    };

    void sort(vector<Node*>& vec);                                      //Sorts all nodes within a vector into descending order by primeVar (weekly cases)
    void calcStateTotals(string stateName);                             //Finds the sum of each variable across all counties within a state and assigns the totals to the state stats. *Reverts relSeverity to 0*
    void calcCountryTotals();                                           //Calls the function to calculate state totals, then finds their sums to assign to the country variables
    void calcTotalSeverity();                                           //Calls the country totals function to ensure all state and country totals are found, then runs through each state->county to find severities based on primeVar (weekly cases)
    void printSeverity(Node* root);                                     //Prints the name of the root node, its stats, and each region below it. If the rootNode is used, will print every state and county.
    void printSeverity(vector<Node*> vec);                              //Similar to the above function, will print the stats for each region in a vector, with states above their counties (if included).
    vector<Node*> search(string arg, string region, int min, int max);  //Search for a particular argument in a region

    //Helper functions for previous search
    vector<Node*> searchPop(string region, int min, int max);
    vector<Node*> searchDensity(string region, int min, int max);
    vector<Node*> searchCases(string region, int min, int max);
    vector<Node*> searchDeaths(string region, int min, int max);
    vector<Node*> searchPrime(string region, int min, int max);
    vector<Node*> searchSeverity(string region, int min, int max);

};



//Node functions



void Node::sort(vector<Node*>& vec)
{
    for (int i = 0; i < vec.size() - 1; i++)
    {
        int swapped = 0;
        tempVal = it->primeVar;
        for (int j = 0; j < vec.size() - i - 1; j++)
        {
            if (vec[j]->primeVar < vec[j + 1]->primeVar)
            {
                Node* temp = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = temp;
                swapped = 1;
            }
        }
        if (swapped == 0)
            break;
    }
}

void Node::calcStateTotals(string stateName)
{
    Node* temp = stateNode(stateName);
    vector<int> stats = { 0, 0, 0, 0, 0, 0 };
    //in order: pop, density, cases, deaths, primeVar, relSeverity
    for (auto const it = temp->nodeVector.begin(); it != temp->nodeVector.end(); it++)
    {
        stats[0] += it->pop;
        stats[1] += it->density;
        stats[2] += it->cases;
        stats[3] += it->deaths;
        stats[4] += it->primeVar;
        //6th entry is relative severity and is left at 0 to be calculated separately
    }
    assignStats(temp, stateName, stats);
}

void Node::calcCountryTotals()
{
    for (auto const it = stateNames.begin(); it != stateNames.end(); it++)
    {
        calcStateTotals(*it);
    }
    vector<int> stats = { 0, 0, 0, 0, 0, 0 };
    for (auto const it2 = rootNode->nodeVector.begin(); it2 != rootNode->nodeVector.end(); it2++)
    {
        stats[0] += it->pop;
        stats[1] += it->density;
        stats[2] += it->cases;
        stats[3] += it->deaths;
        stats[4] += it->primeVar;
        //Relative severity is once again kept at 0
    }
    assignStats(rootNode, rootNode->name, stats);
}

void Node::calcTotalSeverity()
{
    auto const states = rootNode->nodeVector;
    double primeTotal = rootNode->primeVar;
    for (auto const it = states.begin(); it != states.end(); it++)
    {
        auto const counties = it->nodeVector;
        for (auto const it2 = counties.begin(); it2 != counties.end(); it2++)
        {
            it2->relSeverity = (((double) it2->primeVar) / primeTotal) * 100.00; //Unsure if this is how we're doing relative severity. weekly cases in county divided by weekly cases everywhere multiplied by 100. Each severity acts as a percentage.
        }
    }
}

void Node::printSeverity(Node* root, bool country)
{
    if (root->isCountry) //if asking for country statistics, print headers, stats, and run function recursively for each state
    {
        cout << "Total Statistics for " << root->name << endl;
        cout << "Region Name\tPopulation\tDensity\tTotal Cases\tTotal Deaths\tWeekly New Cases\tRelative Severity Score" << endl;
        cout << root->name << "\t" << root->pop << "\t" << root->density << "\t" << root->cases << "\t" << root->deaths << "\t" << root->primeVar << "\t" << root->relSeverity << "\t" << endl;
        for (auto const it = rootNode->nodeVector.begin(); it != rootNode->nodeVector.end(); it++)
        {
            printSeverity(*it, true);
        }
    }
    else if (!country && root->isState) //if not asking for country stats, print headers, stats, and run function recursively for each county
    {
        cout << "Total Statistics for " << root->name << endl;
        cout << "Region Name\tPopulation\tDensity\tTotal Cases\tTotal Deaths\tWeekly New Cases\tRelative Severity Score" << endl;
        cout << root->name << "\t" << root->pop << "\t" << root->density << "\t" << root->cases << "\t" << root->deaths << "\t" << root->primeVar << "\t" << root->relSeverity << "\t" << endl;
        for (auto const it = rootNode->nodeVector.begin(); it != rootNode->nodeVector.end(); it++)
        {
            printSeverity(*it, false);
        }
    }
    else if (country && root->isState)  //if asking for country stats, headers have already been printed, print stats, and run recursively through each county
    {
        cout << "\nState of " << root->name << endl;
        cout << root->name << "\t" << root->pop << "\t" << root->density << "\t" << root->cases << "\t" << root->deaths << "\t" << root->primeVar << "\t" << root->relSeverity << "\t" << endl;
        for (auto const it = rootNode->nodeVector.begin(); it != rootNode->nodeVector.end(); it++)
        {
            printSeverity(*it, false);
        }
    }
    else //if running through counties, simply print stats
    {
        cout << root->name << "\t" << root->pop << "\t" << root->density << "\t" << root->cases << "\t" << root->deaths << "\t" << root->primeVar << "\t" << root->relSeverity << "\t" << endl;
    }
}

void Node::printSeverity(vector<Node*> vec) //NOTE: previous function should print a headline that describes following data:
{
    cout << "Region Name\tPopulation\tDensity\tTotal Cases\tTotal Deaths\tWeekly New Cases\tRelative Severity Score" << endl;

    for (auto const it = vec.begin(); it != vec.end(); it++)
    {
        if (it->isState) //if state, print state header
        {
            cout << "\nState of " << root->name << endl;
        }
        cout << root->name << "\t" << root->pop << "\t" << root->density << "\t" << root->cases << "\t" << root->deaths << "\t" << root->primeVar << "\t" << root->relSeverity << "\t" << endl;
    }
}

vector<Node*> Node::search(string arg, string region, int min, int max)
{
    //This is just one option for taking argument inputs
    vector<string> opt = { "Population", "Density", "Cases", "Deaths", "Weekly Cases", "Severity Score", "population", "density", "cases", "deaths", "weekly cases", "severity score", "weekly Cases", "severity Score", };
    int choice = opt.find(arg);
    //pop = 0, 6    density = 1, 7  cases = 2, 8    deaths = 3, 9   weekly cases = 4, 10, 12    severity = 5, 11, 13
    switch (choice)
    {
    case 0:
    case 6:
        //pop
        return searchPop(region, min, max);
        break;
    case 1:
    case 7:
        //density
        return searchDensity(region, min, max);
        break;
    case 2:
    case 8:
        //cases
        return searchCases(region, min, max);
        break;
    case 3:
    case 9:
        //deaths
        return searchDeaths(region, min, max);
        break;
    case 4:
    case 10:
    case 12:
        //weekly cases
        return searchPrime(region, min, max);
        break;
    case 5:
    case 11:
    case 13:
        //severity score
        return searchSeverity(region, min, max);
        break;
    default:
        break;
    };
    vector<Node*> vec;
    return vec;
}

vector<Node*> Node::searchPop(string region, int min, int max)
{
    vector<Node*> vecList;
    if (stateNames.find(region) != stateNames.end()) //if region is a state
    {
        Node* temp = stateNode(region);
        vector<Node*> vec = temp->nodeVector;
        vecList.push_back(temp);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            if (it->pop >= min && it->pop <= max)
                vecList.push_back(*it);
        }
    }
    else if (region == rootNode->name)  //if region is country
    {
        vector<Node*> vec = rootNode->nodeVector;
        vecList.push_back(rootNode);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            vector<Node*> temp = searchPop(it->name, min, max);
            for (auto const it2 = temp.begin(); it2 != temp.end(); it2++)
            {
                vecList.push_back(*it2);
            }
        }
    }
    else
        return vecList;
    return vecList;
}
vector<Node*> Node::searchDensity(string region, int min, int max)
{
    vector<Node*> vecList;
    if (stateNames.find(region) != stateNames.end()) //if region is a state
    {
        Node* temp = stateNode(region);
        vector<Node*> vec = temp->nodeVector;
        vecList.push_back(temp);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            if (it->density >= min && it->density <= max)
                vecList.push_back(*it);
        }
    }
    else if (region == rootNode->name)  //if region is country
    {
        vector<Node*> vec = rootNode->nodeVector;
        vecList.push_back(rootNode);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            vector<Node*> temp = searchDensity(it->name, min, max);
            for (auto const it2 = temp.begin(); it2 != temp.end(); it2++)
            {
                vecList.push_back(*it2);
            }
        }
    }
    else
        return vecList;
    return vecList;
}
vector<Node*> Node::searchCases(string region, int min, int max)
{
    vector<Node*> vecList;
    if (stateNames.find(region) != stateNames.end()) //if region is a state
    {
        Node* temp = stateNode(region);
        vector<Node*> vec = temp->nodeVector;
        vecList.push_back(temp);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            if (it->cases >= min && it->cases <= max)
                vecList.push_back(*it);
        }
    }
    else if (region == rootNode->name)  //if region is country
    {
        vector<Node*> vec = rootNode->nodeVector;
        vecList.push_back(rootNode);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            vector<Node*> temp = searchCases(it->name, min, max);
            for (auto const it2 = temp.begin(); it2 != temp.end(); it2++)
            {
                vecList.push_back(*it2);
            }
        }
    }
    else
        return vecList;
    return vecList;
}
vector<Node*> Node::searchDeaths(string region, int min, int max)
{
    vector<Node*> vecList;
    if (stateNames.find(region) != stateNames.end()) //if region is a state
    {
        Node* temp = stateNode(region);
        vector<Node*> vec = temp->nodeVector;
        vecList.push_back(temp);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            if (it->deaths >= min && it->deaths <= max)
                vecList.push_back(*it);
        }
    }
    else if (region == rootNode->name)  //if region is country
    {
        vector<Node*> vec = rootNode->nodeVector;
        vecList.push_back(rootNode);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            vector<Node*> temp = searchDeaths(it->name, min, max);
            for (auto const it2 = temp.begin(); it2 != temp.end(); it2++)
            {
                vecList.push_back(*it2);
            }
        }
    }
    else
        return vecList;
    return vecList;
}
vector<Node*> Node::searchPrime(string region, int min, int max)
{
    vector<Node*> vecList;
    if (stateNames.find(region) != stateNames.end()) //if region is a state
    {
        Node* temp = stateNode(region);
        vector<Node*> vec = temp->nodeVector;
        vecList.push_back(temp);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            if (it->primeVar >= min && it->primeVar <= max)
                vecList.push_back(*it);
        }
    }
    else if (region == rootNode->name)  //if region is country
    {
        vector<Node*> vec = rootNode->nodeVector;
        vecList.push_back(rootNode);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            vector<Node*> temp = searchPrime(it->name, min, max);
            for (auto const it2 = temp.begin(); it2 != temp.end(); it2++)
            {
                vecList.push_back(*it2);
            }
        }
    }
    else
        return vecList;
    return vecList;
}
vector<Node*> Node::searchSeverity(string region, int min, int max)
{
    vector<Node*> vecList;
    if (stateNames.find(region) != stateNames.end()) //if region is a state
    {
        Node* temp = stateNode(region);
        vector<Node*> vec = temp->nodeVector;
        vecList.push_back(temp);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            if (it->relSeverity >= min && it->relSeverity <= max)
                vecList.push_back(*it);
        }
    }
    else if (region == rootNode->name)  //if region is country
    {
        vector<Node*> vec = rootNode->nodeVector;
        vecList.push_back(rootNode);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            vector<Node*> temp = searchSeverity(it->name, min, max);
            for (auto const it2 = temp.begin(); it2 != temp.end(); it2++)
            {
                vecList.push_back(*it2);
            }
        }
    }
    else
        return vecList;
    return vecList;
}