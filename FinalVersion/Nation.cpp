#include "Nation.h"
const int per10k = 10000;

Nation::Nation(std::ifstream& numCountyCSV, int numStates)
{
	this->numStates = numStates;
	
	// creation of hashtable for States
	int sizeStateTable = (int)(numStates / 0.7);
	this->stateMap = new State*[sizeStateTable]();

	numCountyCSV.open("Number of Counties Per State.csv");
	if (!numCountyCSV.is_open())
	{
		std::cout << "Counties Per State file not opened.";
		return;
	}
	string countyNumString = "";
	string stateName = "";
	// create State object- constructor effectively determines size of countyMap hashtable.
	while (!numCountyCSV.eof()) {
		std::getline(numCountyCSV, countyNumString, ',');
		std::getline(numCountyCSV, stateName, '\n');
		int numCounties = std::stoi(countyNumString);
		State* newState = new State(stateName, numCounties);
		int stateHashKey = HashState(stateName,stateName.size());
		stateMap[stateHashKey] = newState;
	}
	numCountyCSV.close();
	
}

Nation::Nation()
{
	stateMap = nullptr;
	numStates = 0;
}

Nation::~Nation()
{
	int sizeHashTable = (int)numStates / 0.7;
	for (int i = 0; i < sizeHashTable; i++) 
	{
		if (stateMap[i] != nullptr)
		{
			//std::cout << " N46: " << stateMap[i]->GetName();
			stateMap[i]->~State();
			//std::cout << "\n\n" ;
		}
	}
}

State** Nation::GetStateMap()
{
	return stateMap;
}

int Nation::HashState(string& toHash, int sizeOGstr)
{
	int asciiSum = 0;
	int hashVal = 0;
	string name = toHash;

	for (int i = 0; i < toHash.size(); i++)
		asciiSum += toHash.at(i);
	hashVal = asciiSum % (int)(numStates/0.7);
	//check if there is an element in position 'hashVal', and if so, if it is the state we are looking for. If not, recalculate hashVal. 
	if(stateMap[hashVal] == nullptr)
		return hashVal;
	if(stateMap[hashVal]->GetName().compare(name.substr(0,sizeOGstr))!=0)
	{
		name = Rehash(name);
		hashVal = HashState(name,sizeOGstr);
	}
	return hashVal;
}



string Nation::Rehash(string toRehash) 
{
	return toRehash + "%";
}

// CONSIDER: Could hypothetically reduce the need for two functions doing basically the same thing by making the two input objects Templates - would mean converting State and County into template objects though. I believe this would be doable since the State and County classes are fairly similar and have the same function names for doing tasks on their particular data. For instance, both have GetWeekData() functions
void Nation::CompareCounties(County& county1, County& county2, int month, int startDay)
{
	vector<Day*> week1;
	vector<Day*> week2;
	county1.GetWeek(month, startDay, week1);
	county2.GetWeek(month, startDay, week2);
	vector<int> week1Data = county1.GetWeekData(week1);
	vector<int> week2Data = county2.GetWeekData(week2);

	std::cout << "\nFor the 7 days starting on " << month << "/" << startDay << ":\n";
	std::cout << "Population: " << county1.GetPopulation() << std::setw(20) << county1.GetName() << std::setw(5) << "|" << std::setw(6) << " " << county2.GetName() << "\t" << "Population: " << county2.GetPopulation() << "\n";
	std::cout << std::setfill('_') << std::setw(80) << ""  << " \n";

	std::cout << std::setfill(' ') << std::right << std::setw(33) << " " << "[Raw Totals]\n\n";
	std::cout << std::left << std::setw(30) << "New Cases: " << std::setw(10) << week1Data.at(0) << "|" << std::setw(10) << " " << week2Data.at(0) << " \n";
	std::cout << std::setw(30) << "Positive Cases to Date "   << std::setw(10) << week1Data.at(1) << "|" << std::setw(10) << " " << week2Data.at(1) << " \n";
	std::cout << std::setw(30) << "New Deaths: "  << std::setw(10) << week1Data.at(2) <<"|" << std::setw(10) << " " << week2Data.at(2) << " \n";
	std::cout << std::setw(30) << "Deaths to Date "  << std::setw(10) << week1Data.at(3) << "|" << std::setw(10) << " " << week2Data.at(3) << " \n\n";

	double ncNorm1 = (double)(week1Data.at(0)) / county1.GetPopulation();
	double pscdNorm1 = (double)(week1Data.at(1)) / county1.GetPopulation();
	double ndNorm1 = (double)(week1Data.at(2)) / county1.GetPopulation();
	double dtdNorm1 = (double)(week1Data.at(3)) / county1.GetPopulation();

	double ncNorm2 = (double)(week2Data.at(0)) / county2.GetPopulation();
	double pscdNorm2 = (double)(week2Data.at(1)) / county2.GetPopulation();
	double ndNorm2 = (double)(week2Data.at(2)) / county2.GetPopulation();
	double dtdNorm2 = (double)(week2Data.at(3)) / county2.GetPopulation();

	std::cout << std::right << std::setw(20) << " " << "[Normalized Totals: Per 10,000 People]\n\n";
	std::cout << std::fixed << std::setprecision(3) << std::left << std::setw(30) << "New Cases: " << std::setw(10) << ncNorm1 * per10k  << "|" << std::setw(10) << " " << ncNorm2 * per10k << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Positive Cases to Date " << std::setw(10) << pscdNorm1 * per10k << "|" << std::setw(10) << " " << pscdNorm2* per10k << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "New Deaths: " << std::setw(10) << ndNorm1 * per10k << "|" << std::setw(10) << " " << ndNorm2 * per10k << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Deaths to Date " << std::setw(10) << dtdNorm1 * per10k << "|" << std::setw(10) << " " << dtdNorm2 * per10k << "\n\n";
}

// CONSIDER: Could hypothetically reduce the need for two functions doing basically the same thing by making the two input objects Templates - would mean converting State and County into template objects though.
void Nation::CompareStates(State& state1, State& state2, int month, int startDay)
{

	vector<int> week1 = state1.GetWeekData(month, startDay);
	vector<int> week2 = state2.GetWeekData(month, startDay);

	std::cout << "\nFor the 7 days starting on " << month << "/" << startDay << ":\n";
	std::cout << std::setw(15) << "Population: " <<state1.GetPopulation() << std::setw(5) << " " << state1.GetName() << std::right << std::setw(6) << "|" << std::setw(10) << " " << state2.GetName() << "\t Population: " << state1.GetPopulation() << "\n";
	std::cout << std::setfill('_') << std::setw(80) << "" << "\n";

	std::cout << std::setfill(' ') << std::right << std::setw(33) << " " << "[Raw Totals]\n\n";
	std::cout << std::left << std::setw(30) << "New Cases: "<< std::setw(10) << week1.at(0) <<  "|" << std::setw(10) << " " << week2.at(0) << " \n";
	std::cout << std::setw(30)<< "Positive Cases to Date " << std::setw(10) << week1.at(1) <<  "|" << std::setw(10) << " " << week2.at(1) << " \n";
	std::cout << std::setw(30) << "New Deaths: " << std::setw(10) << week1.at(2) <<  "|" << std::setw(10) << " " << week2.at(2) << " \n";
	std::cout << std::setw(30) << "Deaths to Date " << std::setw(10) << week1.at(3) <<  "|" << std::setw(10) << " " << week2.at(3) << " \n\n";

	double ncNorm1 = (double)(week1.at(0)) / state1.GetPopulation();
	double pscdNorm1 = (double)(week1.at(1)) / state1.GetPopulation();
	double ndNorm1 = (double)(week1.at(2)) / state1.GetPopulation();
	double dtdNorm1 = (double)(week1.at(3)) / state1.GetPopulation();

	double ncNorm2 = (double)(week2.at(0)) / state2.GetPopulation();
	double pscdNorm2 = (double)(week2.at(1)) / state2.GetPopulation();
	double ndNorm2 = (double)(week2.at(2)) / state2.GetPopulation();
	double dtdNorm2 = (double)(week2.at(3)) / state2.GetPopulation();

	std::cout << std::setfill(' ') << std::right << std::setw(20) << " " << "[Normalized Totals: Per 10,000 People]\n\n";
	std::cout << std::fixed << std::setprecision(3) << std::left << std::setw(30) << "New Cases: " << std::setw(10) << ncNorm1 * per10k << "|" << std::setw(10) << " " << ncNorm2 * per10k << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Positive Cases to Date " << std::setw(10) << pscdNorm1 * per10k <<  "|" << std::setw(10) << " " << pscdNorm2 * per10k << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "New Deaths: " << std::setw(10) << ndNorm1 * per10k << "|" << std::setw(10) << " " <<ndNorm2 * per10k << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Deaths to Date " << std::setw(10) << dtdNorm1 * per10k << "|" << std::setw(10) << " " << dtdNorm2 * per10k << " \n";

}

void Nation::CompareCountiesAvg(County& county1, County& county2, int startMonth, int endMonth, int endDay)
{
	int totalCases1 = county1.GetDay(endMonth, endDay)->GetPosCasesToDate();
	int totalDeaths1 = county1.GetDay(endMonth, endDay)->GetDeathsToDate();
	int totalCases2 = county1.GetDay(endMonth, endDay)->GetPosCasesToDate();
	int totalDeaths2 = county2.GetDay(endMonth, endDay)->GetDeathsToDate();
	int county1pop = county1.GetPopulation();
	int county2pop = county2.GetPopulation();

	int numMonths = endMonth - startMonth + 1;
	// Number of months in range multiplied by average number of days in a month, divided by days in a week
	double numWeeks = numMonths * (29.5) / (7);

	float posCasesNorm1 = (float)totalCases1 / county1pop;
	float deathsNorm1 = (float)totalDeaths1 / county1pop;
	float posCasesNorm2 = (float)totalCases2 / county2pop;
	float deathsNorm2 = (float)totalDeaths2 / county2pop;


	std::cout << std::setw(15) << "Population: " << county1pop << std::setw(5) << " " << county1.GetName() << std::right << std::setw(6) << "|" << std::setw(10) << " " << county2.GetName() << "\t Population: " << county2pop << "\n";
	std::cout << std::setfill('_') << std::setw(80) << "" << "\n";

										/*===== Table for [Raw Totals] ======*/

	std::cout << std::setfill(' ') << std::right << std::setw(33) << " " << "[Raw Totals]\n\n";
	std::cout << std::left << std::setw(30) << "New Cases: " << std::setw(10) << totalCases1 << "|" << std::setw(10) << " " << totalCases2 << " \n";
	std::cout << std::setw(30) << "Average Weekly Cases: " << std::setw(10) << totalCases1 / numWeeks << "|" << std::setw(10) << " " << totalCases2 / numWeeks << " \n";
	std::cout << std::setw(30) << "Average Monthly Cases: " << std::setw(10) << totalCases1 / numMonths << "|" << std::setw(10) << " " << totalCases2 / numMonths << " \n";

	std::cout << std::setw(30) << "Total Deaths: " << std::setw(10) << totalDeaths1 << "|" << std::setw(10) << " " << totalCases2 << " \n";
	std::cout << std::setw(30) << "Average Weekly Deaths: " << std::setw(10) << totalDeaths1 / numWeeks << "|" << std::setw(10) << " " << totalCases2 / numWeeks << " \n";
	std::cout << std::setw(30) << "Average Monthly Deaths: " << std::setw(10) << totalDeaths1 / numMonths << "|" << std::setw(10) << " " << totalCases2 / numMonths << " \n\n";

	/*===== Table for [Normalized Totals: Per 10,000 People] ======*/

	std::cout << std::setfill(' ') << std::right << std::setw(20) << " " << "[Normalized Totals: Per 10,000 People]\n\n";
	std::cout << std::fixed << std::setprecision(3) << std::left << std::setw(30) << "Total Cases: " << std::setw(10) << posCasesNorm1 * 10000 << "|" << std::setw(10) << " " << posCasesNorm2 * 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Average Weekly Cases: " << std::setw(10) << posCasesNorm1 / numWeeks * 10000 << "|" << std::setw(10) << " " << posCasesNorm2 / numWeeks * 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Average Monthly Cases: " << std::setw(10) << posCasesNorm1 / numMonths * 10000 << "|" << std::setw(10) << " " << posCasesNorm2 / numMonths * 10000 << " \n";

	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Total Deaths: " << std::setw(10) << deathsNorm1 * 10000 << "|" << std::setw(10) << " " << deathsNorm2 * 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Average Weekly Deaths " << std::setw(10) << deathsNorm1 / numWeeks * 10000 << "|" << std::setw(10) << " " << deathsNorm2 / numWeeks * 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Average Monthly Deaths " << std::setw(10) << deathsNorm1 / numMonths * 10000 << "|" << std::setw(10) << " " << deathsNorm2 / numMonths * 10000 << " \n\n";
}

void Nation::CompareStatesAvg(State& state1, State& state2, int startMonth, int endMonth, int endDay)
{
	int totalCases1 = 0;
	int totalDeaths1 = 0;
	int totalCases2 = 0;
	int totalDeaths2 = 0;
	int state1pop = state1.GetPopulation();
	int state2pop = state2.GetPopulation();

	// Following two loops get the last day from each county in the state and add them to the running total for their respective state
	vector<int>* hashKeys = &state1.GetCountyHashKeys();
	County** countyTable = state1.GetCountyMap();
	for (unsigned int i = 0; i < hashKeys->size(); i++)
	{
		totalCases1 += countyTable[hashKeys->at(i)]->GetDay(endMonth, endDay)->GetPosCasesToDate();
		totalDeaths1 += countyTable[hashKeys->at(i)]->GetDay(endMonth, endDay)->GetDeathsToDate();
	}
	hashKeys = &state2.GetCountyHashKeys();
	countyTable = state2.GetCountyMap();
	for (unsigned int i = 0; i < hashKeys->size(); i++)
	{
		totalCases2 += countyTable[hashKeys->at(i)]->GetDay(endMonth, endDay)->GetPosCasesToDate();
		totalDeaths2 += countyTable[hashKeys->at(i)]->GetDay(endMonth, endDay)->GetDeathsToDate();
	}

	int numMonths = endMonth - startMonth;
	// Number of months in range multiplied by average number of days in a month, divided by days in a week
	double numWeeks = numMonths * (29.5) / (7);

	float posCasesNorm1 = (float)totalCases1 / state1pop;
	float deathsNorm1 = (float)totalDeaths1 / state1pop;
	float posCasesNorm2 = (float)totalCases2 / state2pop;
	float deathsNorm2 = (float)totalDeaths2 / state2pop;


	std::cout << std::setw(15) << "Population: " << state1pop << std::setw(5) << " " << state1.GetName() << std::right << std::setw(6) << "|" << std::setw(10) << " " << state2.GetName() << "\t Population: " << state2pop << "\n";
	std::cout << std::setfill('_') << std::setw(80) << "" << "\n";
	
										/*===== Table for [Raw Totals] ======*/
				
	std::cout << std::setfill(' ') << std::right << std::setw(33) << " " << "[Raw Totals]\n\n";
	std::cout << std::left << std::setw(30) << "New Cases: " << std::setw(10) << totalCases1 << "|" << std::setw(10) << " " << totalCases2 << " \n";
	std::cout << std::setw(30) << "Average Weekly Cases: " << std::setw(10) << totalCases1/numWeeks << "|" << std::setw(10) << " " << totalCases2/numWeeks << " \n";
	std::cout << std::setw(30) << "Average Monthly Cases: " << std::setw(10) << totalCases1/numMonths << "|" << std::setw(10) << " " << totalCases2/numMonths << " \n";

	std::cout << std::setw(30) << "Total Deaths: " << std::setw(10) << totalDeaths1 << "|" << std::setw(10) << " " << totalCases2 << " \n";
	std::cout << std::setw(30) << "Average Weekly Deaths: " << std::setw(10) << totalDeaths1/numWeeks << "|" << std::setw(10) << " " << totalCases2/numWeeks << " \n";
	std::cout << std::setw(30) << "Average Monthly Deaths: " << std::setw(10) << totalDeaths1/numMonths << "|" << std::setw(10) << " " << totalCases2/numMonths << " \n\n";

							/*===== Table for [Normalized Totals: Per 10,000 People] ======*/

	std::cout << std::setfill(' ') << std::right << std::setw(20) << " " << "[Normalized Totals: Per 10,000 People]\n\n";
	std::cout << std::fixed << std::setprecision(3) << std::left << std::setw(30) << "Total Cases: " << std::setw(10) << posCasesNorm1 * 10000 << "|" << std::setw(10) << " " << posCasesNorm2 * 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Average Weekly Cases: " << std::setw(10) << posCasesNorm1/numWeeks * 10000 << "|" << std::setw(10) << " " << posCasesNorm2/numWeeks * 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Average Monthly Cases: " << std::setw(10) << posCasesNorm1/numMonths * 10000 << "|" << std::setw(10) << " " << posCasesNorm2/numMonths * 10000 << " \n";

	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Total Deaths: " << std::setw(10) << deathsNorm1 * 10000 << "|" << std::setw(10) << " " << deathsNorm2 * 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Average Weekly Deaths " << std::setw(10) << deathsNorm1/numWeeks* 10000 << "|" << std::setw(10) << " " << deathsNorm2/numWeeks * 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << std::setw(30) << "Average Monthly Deaths " << std::setw(10) << deathsNorm1/numMonths * 10000 << "|" << std::setw(10) << " " << deathsNorm2/numMonths * 10000 << " \n\n";
}


