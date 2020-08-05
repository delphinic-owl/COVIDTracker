#include "Node.h"
#include "timer.h"
using namespace std;

//Node functions

/*
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
}*/


void Node::calcTotals(Node* root, string stateName)
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
        //Recalculate weekly and monthly averages based on new totals
        stats[3] = stats[1] / (122.0 / 7.0);
        stats[4] = stats[1] / 4.0;
        stats[5] = stats[2] / (122.0 / 7.0);
        stats[6] = stats[2] / 4.0;

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
        //Recalculate weekly and monthly averages based on new totals
        stats[3] = stats[1] / (122.0 / 7.0);
        stats[4] = stats[1] / 4.0;
        stats[5] = stats[2] / (122.0 / 7.0);
        stats[6] = stats[2] / 4.0;

        temp->assignStats(stats);
    }
}

void Node::calcTotalSeverity(Node* root)
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

void Node::createNodes(CSVReader* data, Node* root)
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
            stats.push_back(stoi(test[6])); //deaths
            stats.push_back(stoi(test[4])); //weeklycases
            stats.push_back(stoi(test[5])); //monthlycases
            stats.push_back(stoi(test[7])); //weekly deaths
            stats.push_back(stoi(test[8])); //monthly deaths
            stats.push_back(((double)stats[3] / (double)stats[2]) * 10000.00);     //Density = total cases / population * 10,000 people  AKA: Cases per 10,000 people
        }

        if (stateBoolean || test.size() >= 4)   //Exclude counties with incomplete data
            root->insertNode(root, countyName, stateName, stats);
        i++;
    }
}

void Node::printSeverity(Node* root, bool country, bool state)
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

void Node::PrintTotals()
{
    cout << setw(33) << " " << name << setw(10) << " " << "Population: " << statistics[0] << " \n";
    cout << setfill('_') << setw(80) << " " << " \n";

    cout << setfill(' ') << right << setw(15) << " " << "[Raw Totals]\n";
    cout << "Total Cases: " << setw(21) << " " << statistics[1] << " \n";
    cout << "Average Weekly Cases: " << setw(12) << " " << statistics[3] << " \n";
    cout << "Average Monthly Cases: " << setw(11) << " " << statistics[4] << " \n";

    cout << "Total Deaths: " << setw(20) << " " << statistics[2] << " \n";
    cout << "Average Weekly Deaths: " << setw(11) << " " << statistics[5] << " \n";
    cout << "Average Monthly Deaths: " << setw(10) << " " << statistics[6] << " \n\n";

    double tc = (double)(statistics[1]) / (double) statistics[0];
    double awc = (double)(statistics[3]) / (double) statistics[0];
    double amc = (double)(statistics[4]) / (double) statistics[0];
    double td = (double)(statistics[2]) / (double) statistics[0];
    double awd = (double)(statistics[5]) / (double) statistics[0];
    double amd = (double)(statistics[6]) / (double) statistics[0];

    cout << setfill(' ') << right << setw(5) << " " << "[Normalized Totals: Per 10,000 People] \n";
    cout << fixed << setprecision(3) << "Total Cases: " << setw(21) << " " << tc * 10000 << " \n";
    cout << fixed << setprecision(3) << "Average Weekly Cases " << setw(13) << " " << awc * 10000 << " \n";
    cout << fixed << setprecision(3) << "Average Monthly Cases " << setw(12) << " " << amc * 10000 << " \n";

    cout << fixed << setprecision(3) << "Total Deaths: " << setw(20) << " " << td * 10000 << " \n";
    cout << fixed << setprecision(3) << "Average Weekly Deaths " << setw(12) << " " << awd * 10000 << " \n";
    cout << fixed << setprecision(3) << "Average Monthly Deaths " << setw(11) << " " << amd * 10000 << " \n\n";
}

void Node::PrintAllCounties()
{
    int format = 0;
    for (auto i = nodeVector.begin(); i != nodeVector.end(); i++)
    {
        if (format < 3)
        {
            std::cout << std::setw(15) << (*i)->name << std::setw(10) << "|\t";
            format++;
        }
        else {
            format = 0;
            std::cout << (*i)->name << "\n";
        }
    }
}

void Node::CompareRegions(Node& region1, Node& region2)
{
    auto stats1 = region1.stats();
    auto stats2 = region2.stats();

    std::cout << std::setw(15) << "Population: " << stats1[0] << std::setw(5) << " " << region1.name << std::right << std::setw(6) << "|" << std::setw(10) << " " << region2.name << "\t Population: " << stats2[0] << "\n";
    std::cout << std::setfill('_') << std::setw(80) << "" << "\n";

    std::cout << std::setfill(' ') << std::right << std::setw(33) << " " << "[Raw Totals]\n\n";
    std::cout << std::left << std::setw(30) << "New Cases: " << std::setw(10) << stats1[1] << "|" << std::setw(10) << " " << stats2[1] << " \n";
    std::cout << std::setw(30) << "Average Weekly Cases: " << std::setw(10) << stats2[3] << "|" << std::setw(10) << " " << stats2[3] << " \n";
    std::cout << std::setw(30) << "Average Monthly Cases: " << std::setw(10) << stats2[4] << "|" << std::setw(10) << " " << stats2[4] << " \n";

    std::cout << std::setw(30) << "Total Deaths: " << std::setw(10) << stats1[2] << "|" << std::setw(10) << " " << stats2[2] << " \n";
    std::cout << std::setw(30) << "Average Weekly Deaths: " << std::setw(10) << stats1[5] << "|" << std::setw(10) << " " << stats1[5] << " \n";
    std::cout << std::setw(30) << "Average Monthly Deaths: " << std::setw(10) << stats1[6] << "|" << std::setw(10) << " " << stats1[6] << " \n\n";


    double tc1 = (double)(stats1[1]) / stats1[0];
    double awc1 = (double)(stats2[3]) / stats1[0];
    double amc1 = (double)(stats2[4]) / stats1[0];
    double td1 = (double)(stats1[2]) / stats1[0];
    double awd1 = (double)(stats1[5]) / stats1[0];
    double amd1 = (double)(stats1[6]) / stats1[0];


    double tc2 = (double)(stats2[1]) / stats2[0];
    double awc2 = (double)(stats2[3]) / stats2[0];
    double amc2 = (double)(stats2[4]) / stats2[0];
    double td2 = (double)(stats2[2]) / stats2[0];
    double awd2 = (double)(stats1[5]) / stats2[0];
    double amd2 = (double)(stats1[6]) / stats2[0];


    std::cout << std::setfill(' ') << std::right << std::setw(20) << " " << "[Normalized Totals: Per 10,000 People]\n\n";
    std::cout << std::fixed << std::setprecision(3) << std::left << std::setw(30) << "Total Cases: " << std::setw(10) << tc1 * 10000 << "|" << std::setw(10) << " " << tc2 * 10000 << " \n";
    std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Average Weekly Cases: " << std::setw(10) << awc1 * 10000 << "|" << std::setw(10) << " " << awc2 * 10000 << " \n";
    std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Average Monthly Cases: " << std::setw(10) << amc1 * 10000 << "|" << std::setw(10) << " " << amc2 * 10000 << " \n";

    std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Total Deaths: " << std::setw(10) << td1 * 10000 << "|" << std::setw(10) << " " << td2 * 10000 << " \n";
    std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Average Weekly Deaths " << std::setw(10) << awd1 * 10000 << "|" << std::setw(10) << " " << awd2 * 10000 << " \n";
    std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Average Monthly Deaths " << std::setw(10) << amd1 * 10000 << "|" << std::setw(10) << " " << amd2 * 10000 << " \n\n";
}