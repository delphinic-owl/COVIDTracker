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
    //Variables may be moved to private potentially as they are only accessed in Node so far. Structure is set up so that they won't need to be accessed outside class.
    string name;
    string stateName;
    int pop;
    int density;
    int cases;
    int deaths;
    int primeVar;   //Weekly cases
    vector<int> statistics;
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
        int density = node.density;     //What is density? Like population over land area? Population over cases?
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
        //This function uses vectors with 5 INTS ONLY. No less.
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
        root->relSeverity = 0.0;
    };

    //This might need to be void?
    //Stats might need to be calculated when reading from file (i.e. density is a calculation, weekly cases might not be stated explicitly, and many reports are over time.
    //Stats may be changed to a vector of a vector of integers to represent stats over time (i.e. March, April, May, June)
    Node* insert(Node* root, string name, string stateName, bool state, vector<int> stats)
    {
        Node* temp = new Node();
        assignStats(temp, name, stats);
        if (root == nullptr)
        {
            temp->isCountry = true;
            temp->isState = false;
            temp->rootNode = temp;
            return temp;
        }

        temp->isCountry = false;
        temp->rootNode = root;

        if (state)
        {
            temp->isState = true;
            rootNode->nodeVector.push_back(temp);
            rootNode->nodeMap.emplace(name, temp);
            return temp;
        }
        else
        {
            temp->isState = false;
            //In rootNode's vector, find state node pointer and push temp back into its node pointer vector
            stateNode(stateName)->nodeVector.push_back(temp);
            stateNode(stateName)->nodeMap.emplace(name, temp);
            return temp;
        }
        return root;
    };

    void sort(int n, vector<Node*>& vec);                                                       //Sorts all nodes within a vector into descending order by primeVar (weekly cases)
    void calcTotals(string stateName);                                                          //Calculates the totals under the particular region. Accepts "country", country name, or state names.
    void calcTotalSeverity();                                                                   //Calls the country totals function to ensure all state and country totals are found, then runs through each state->county to find severities based on primeVar (weekly cases)
    void printSeverity(Node* root);                                                             //Prints the name of the root node, its stats, and each region below it. If the rootNode is used, will print every state and county.
    void printSeverity(vector<Node*> vec);                                                      //Similar to the above function, will print the stats for each region in a vector, with states above their counties (if included).
    vector<Node*> search(string arg, string region, int min, int max, int numberOfCounties);    //Search for a particular argument in a region. Only reports the max number of counties per state
    vector<Node*> searchStat(int arg, string region, int min, int max, int numCounties);        //Helper function for search
};





//Node functions





//The input n repesents the index of the statistic to sort by in descending order
// 0 = pop; 1 = density; 2 = cases; 3 = deaths; 4 = primeVar (weekly cases)
// Can be changed to sort by severity score (see searchStat for example)
void Node::sort(int n, vector<Node*>& vec)
{
    for (int i = 0; i < vec.size() - 1; i++)
    {
        int swapped = 0;
        for (int j = 0; j < vec.size() - i - 1; j++)
        {
            if (vec[j]->statistics[n] < vec[j + 1]->statistics[n])
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

void Node::calcTotals(string stateName)
{
    if (stateName == rootNode->name || stateName == "country")  //If input is country, first calculate all state totals
    {
        Node* temp = rootNode;
        for (auto const it = stateNames.begin(); it != stateNames.end(); it++)
        {
            calcTotals(*it);
        }
    }
    else
        Node* temp = stateNode(stateName);  //If input is state, calculate its totals

    vector<int> stats = { 0, 0, 0, 0, 0 };
    for (auto const it = temp->nodeVector.begin(); it != temp->nodeVector.end(); it++)
    {
        stats[0] += it->pop;
        stats[1] += it->density;
        stats[2] += it->cases;
        stats[3] += it->deaths;
        stats[4] += it->primeVar;
    }
    assignStats(temp, stateName, stats);
}

//This function should be called *AFTER* calculating totals for the entire country
void Node::calcTotalSeverity()
{
    auto const states = rootNode->nodeVector;
    double primeTotal = rootNode->primeVar; //The country only gets its stats after totalling the rest. This function will return 0 for every county if not totalled first
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

void Node::printSeverity(vector<Node*> vec, string arg)
{
    //Header defines where the list was retrieved from (search argument). This may be adjusted to print the values from the search as well.
    cout << "Displaying all regions within the " << arg << " specifications" << endl;
    cout << "Region Name\tPopulation\tDensity\tTotal Cases\tTotal Deaths\tWeekly New Cases\tRelative Severity Score" << endl;

    for (auto const it = vec.begin(); it != vec.end(); it++)
    {
        if (it->isState) //if state, print state header
        {
            cout << "\nState of " << root->name << endl;
        }
        //Function can be adjusted to only print the relevant statistics, or can continue to display all.
        cout << root->name << "\t" << root->pop << "\t" << root->density << "\t" << root->cases << "\t" << root->deaths << "\t" << root->primeVar << "\t" << root->relSeverity << "\t" << endl;
    }
}

//This function is exclusively used to convert a string into an integer for the searchStat function. If a string is not used, this function can be bypassed
vector<Node*> Node::search(string arg, string region, int min, int max, int numberOfCounties)
{
    vector<Node*> nullVec;
    vector<string> opt = { "Population", "population", "Density", "density", "Cases", "cases", "Deaths", "deaths", "Weekly Cases", "weekly cases", "Severity Score", "severity score" };
    if (opt.find(arg) != opt.end())
        int stat = (opt.find(arg)) / 2;
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
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            vector<Node*> temp = searchStat(arg, it->name, min, max, numCounties);   //Runs through each state, then runs through the returned vector to add each county and state to the total list
            for (auto const it2 = temp.begin(); it2 != temp.end(); it2++)
            {
                vecList.push_back(*it2);
            }
        }
    }
    if (stateNames.find(region) != stateNames.end()) //if region is a state
    {
        Node* temp = stateNode(region);
        vector<Node*> vec = temp->nodeVector;
        vecList.push_back(temp);
        for (auto const it = vec.begin(); it != vec.end(); it++)
        {
            //first argument tests if arg equals 5 to determine whether to check for double severity score or integer stats. Second ensures only the max number of counties are added (not including state).
            if (arg == 5 && vecList.size() < numCounties + 1 && it->relSeverity >= min && it->relSeverity <= max)
                vecList.push_back(*it);
            if (arg != 5 && vecList.size() < numCounties + 1 && it->statistics[arg] >= min && it->statistics[arg] <= max)
                vecList.push_back(*it);
        }
    }
    return vecList;
}