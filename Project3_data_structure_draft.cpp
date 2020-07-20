#include <iostream>
#include <iomanip>
#include <cstring>
#include <list>
#include <iterator>
#include <vector>
#include <map>
#include <math.h>
using namespace std;

//I've commented out potential code structures/functions that might be useful.

//If making each region its own class, change this to Region instead of Regions.
class Regions
{
private:
    /*Either we can organize all regions into one class using struct nodes or have each region be its own class. The former would probably give us more options. Thinking about the adjacency list project.
    //non-struct structure
    string name;
    int pop;
    int density;
    int cases;
    int deaths;
    double relSeverity;
    */

    struct Region
    {
        string name = "";
        int pop = 0;
        int density = 0;
        int cases = 0;
        int deaths = 0;
        double relSeverity = 0.0;
    };

    map<string, int> regMap; //contains the index of each region accessed by their name. *MAKE SURE REGIONS HAVE UNIQUE NAMES*

    vector<region> regVector; //contains all region structures. Index for each region can be retrieved from the regMap

    double currentID; //current ID. ID is always equal to total number of regions + 1.

public:

    /* Constructors for each Region class if we decide to do each one as a separate class.
    Region()
    {
        this.name = "";
        this.pop = 0;
        this.density = 0;
        this.cases = 0;
        this.deaths = 0;
        this.relSeverity = 0.0;
    }

    Region (string name, int pop, int density, int cases, int deaths, double relSeverity)
    {
        this.name = name;
        this.pop = pop;
        this.density = density;
        this.cases = cases;
        this.deaths = deaths;
        this.relSeverity = relSeverity;
    }
    */

    Regions()
    {
        currentID = 1;
    };

    
    void scoreReport(int min, int max); //prints an ordered list of regions by severity scores within the range.
    vector<region> scoreReport(int min, int max); //returns an ordered vector similar to above.
    
    vector<int> alphaOrder(); //sorts region names alphabetically, returning a vector of the indexes that pertain to each region in the correct order.
    
    bool isRegion(string regName); //test to identify if a region exists in the data.
    
    void insertRegion(string regName); //adds a new region.
    
    void insertEdge(string from, string to); //copied from project2 code, likely to be unused.
};

//I've copied the following functions from Project 2. These all worked, despite having an odd-structure. We might be able to salvage these. At least, it helps me to have reference material.

// (COPIED) prints the PageRank of all pages after powerIterations in ascending alphabetical order of webpages and rounding rank to two decimal places
// We can use this same structure to calculate severity scores and print/return regions. Will need editing.
void Regions::scoreReport(int min, int max)
{
    for (int j = 0; j < currentID - 1; j++)
    {
        regVector[j].rank = 1.00 / (currentID - 1.00);
    }
    for (int i = 1; i < p; i++) // repeats based on power iterations from last project.
    {
        map<int, double> storedRanks;
        for (int j = 0; j < currentID - 1; j++)
        {
            int prevNode = 0;
            double prevRank = 0;
            double prevDegree = 0;
            double rankSum = 0;
            for (int k = 0; k < regVector[j].prevID.size(); k++)  // This for loop looks through a series of pointers to determine the pageRank from the previous project. Will remove later.
            {
                prevNode = regVector[j].prevID[k];
                prevRank = regVector[prevNode - 1].rank;
                prevDegree = regVector[prevNode - 1].out_degree;
                rankSum += (prevRank / prevDegree);
            }
            storedRanks[j] = floor(rankSum * 100.00) / 100.00;
        }
        for (int n = 0; n < currentID - 1; n++)
        {
            regVector[n].rank = storedRanks[n];
        }
    }

    vector<int> order = alphaOrder();

    cout << fixed << setprecision(2);
    for (int i = 0; i < currentID - 1; i++)
    {
        cout << regVector[order[i]].name << " " << regVector[order[i]].rank << endl;    //This is how I printed the nodes in Project 2 in alphabetical order.
    }
}

// (COPIED) Method to generate an int vector corresponding to node index in alphabetical order.
// If we put things in alpha order, this might help.
vector<int> Regions::alphaOrder()
{
    vector<string> array;
    vector<int> order;
    //inserts all names from each vector and their indexes into two arrays.
    for (int i = 0; i < currentID - 1; i++)
    {
        array.push_back(regVector[i].name);
        order.push_back(i);
    }
    int i, j, k;
    string key;
    //this loop sorts through them, putting array into alpha order and moving the indexes in the order vector the same way. 
    for (i = 1; i < currentID - 1; i++)
    {
        key = array[i];
        k = order[i];
        j = i - 1;
        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            order[j + 1] = order[j];
            j = j - 1;
        }
        array[j + 1] = key;
        order[j + 1] = k;
    }
    //returns the ordered vector of indexes.
    return order;
}

bool Regions::isRegion(string regName)
{
    // (COPIED) Look for vertex name in map.
    if (regMap.find(regName) != regMap.end())
        return true;
    else
        return false;
}

void AdjacencyList::insertRegion(string regName)
{
    // (COPIED) Insert new vertex node into vector, and assign string-int pair to map. All IDs are equal to index+1.
    Region* newReg = new Region();
    newReg->name = regName;
    /* allows editing each struct element. If each region becomes its own class, this can be done in a constructor.
    newReg->pop = pop;
    newReg->density = density;
    newReg->cases = cases;
    newReg->deaths = deaths;
    newReg->relSeverity = relSeverity;  //This can be altered to make a calulcation, or it can be done at the end like pageRank from Project2.
    */
    regVector.push_back(*newReg);
    regMap.emplace(regName, currentID++);
    return;
}

/* from last project, likely gonna remove.

void Regions::insertEdge(string from, string to)
{
    //Test to see if vertex exists for both to and from, if not, create them, implement them, and assign ID.
    if (!isVertex(from))
        insertVertex(from);
    if (!isVertex(to))
        insertVertex(to);
    //Retrieve IDs equal to vector index+1.
    int findex = nodeMap[from] - 1;
    int tindex = nodeMap[to] - 1;
    //Assign pointers.
    Node* fNode = &adj[findex];
    Node* tNode = &adj[tindex];
    //If from node has pointers to existing vertices, assign to last nullptr. Maximum of 3 pages each?
    if (fNode->next1 == nullptr)
        fNode->next1 = tNode;
    else if (fNode->next2 == nullptr)
        fNode->next2 = tNode;
    else
        fNode->next3 = tNode;
    fNode->out_degree += 1.00;
    tNode->in_degree += 1.00;
    tNode->prevID.push_back(findex + 1);
    return;
}
*/

//A main program for testing through console. Receives input just as in Project2. This can be made into a GUI later.
int main()
{
    //Create list, retrieve number of lines and power iterations.
    Regions regList;
    int no_of_lines, min, max;
    string from, to;
    //Retrieve number of lines for commands, min score, max score
    cin >> no_of_lines;
    cin >> min;
    cin >> max;
    //Loop to retrieve from and to entries then create edges,
    for (int i = 0; i < no_of_lines; i++)
    {
        cin >> from;
        cin >> to;
        regList.insertEdge(from, to); //used insertEdge for adjacency list structure. Will change.
    }
    //Output page ranks after p iterations
    regList.scoreReport(power_iterations);
}