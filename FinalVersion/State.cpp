#include <iostream>
#include <vector>
#include "State.h"
#include "timer.h"
using std::vector;

State::State()
{
	countyMap = nullptr;
	totalDeaths = 0;
	totalPosCases = 0;
	numCounties = 0;
	name = "";
}

State::State(string name, int numCounties)
{
	this->numCounties = numCounties;
	//creation of hashtable for this State's counties
	countyMap = new County*[(int)(numCounties/0.7)]();
	totalDeaths = 0;
	totalPosCases = 0;
	this->name = name;
}

State::~State()
{
	for (int i = 0; i < countyHashKeys.size(); i++)
		{
			//std::cout << "S30 " << this->name  << "Total counties deleted:" << i << "|";
			delete countyMap[countyHashKeys.at(i)];
		}
}

void State::SetPopulation(int pop)
{
	this->population = pop;
}

County** State::GetCountyMap()
{
	return countyMap;
}

vector<int>& State::GetCountyHashKeys()
{
	return countyHashKeys;
}

void State::RemoveEmptyCounties()
{
	for (int i = 0; i < countyHashKeys.size(); i++) 
	{
		//If there is no data for March 2nd, for the county, take that as an alert that the county has no data reported for it in the COVID-Actvity CSV. Remove its hashkey from the vector of hashkeys.
		if (countyMap[countyHashKeys.at(i)]->GetDay(3, 2) == nullptr)
			countyHashKeys.erase(countyHashKeys.begin()+i);
	}

}

void State::PrintAllCounties()
{
	RemoveEmptyCounties();
	int format = 0;
	for (unsigned int i = 0; i < countyHashKeys.size(); i++) 
	{
		if (format < 3)
		{
			std::cout << std::setw(30) << countyMap[countyHashKeys.at(i)]->GetName() << std::setw(10) << "|\t";
			format++;
		}
		else {
			format = 0;
			std::cout << countyMap[countyHashKeys.at(i)]->GetName() << "\n";
		}
	}


}

void State::PrintWeek(int month, int startDay)
{
	timer t;
	t.Start();
	vector<int> stateWeekData = GetWeekData(month, startDay);
	cout << "Week Data Found in: " << t.reportTime() << " milliseconds" << endl;

	std::cout << "For the 7 days starting on " << month << "/" << startDay << ":\n\n";
	std::cout << std::setw(33) << " " << this->name << std::setw(10) << " " << "Population: " << population << " \n";
	std::cout << std::setfill('_') << std::setw(80) << " " << " \n";

	std::cout << std::setfill(' ') << std::right << std::setw(15) << " " << "[Raw Totals]\n";
	std::cout << "New Cases: " << std::setw(23) << " " << stateWeekData.at(0) << " \n";
	std::cout << "Positive Cases to Date " << std::setw(11) << " " << stateWeekData.at(1) << " \n";
	std::cout << "New Deaths: " << std::setw(22) << " " << stateWeekData.at(2) << " \n";
	std::cout << "Deaths to Date " << std::setw(19) << " " << stateWeekData.at(3) << " \n\n";
	
	double ncNorm = (double)(stateWeekData.at(0)) / this->population;
	double pscdNorm = (double)(stateWeekData.at(1)) / this->population;
	double ndNorm = (double)(stateWeekData.at(2)) / this->population;
	double dtdNorm = (double)(stateWeekData.at(3)) / this->population;

	std::cout << std::setfill(' ') << std::right << std::setw(5) << " " << "[Normalized Totals: Per 10,000 People] \n";
	std::cout << std::fixed << std::setprecision(3) << "New Cases: " << std::setw(23) << " " <<  ncNorm* 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << "Positive Cases to Date " << std::setw(11) << " " << pscdNorm* 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << "New Deaths: " << std::setw(22) << " " << ndNorm* 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << "Deaths to Date " << std::setw(19) << " " << dtdNorm * 10000 << " \n\n";

}

void State::PrintWeekAvg(int startMonth, int endMonth, int endDay)
{
	int totalAvgCases = 0;
	int totalAvgDeaths= 0;

	timer t;
	t.Start();

	Day* curr = nullptr;
	for (unsigned int i = 0; i < countyHashKeys.size(); i++)
	{
		curr = countyMap[countyHashKeys.at(i)]->GetDay(endMonth, endDay);
		if (curr == nullptr)
			continue;
		totalAvgCases += curr->GetPosCasesToDate();
		totalAvgDeaths += curr->GetDeathsToDate();
	}
	cout << "Week Data Found in: " << t.reportTime() << " milliseconds" << endl;

	int numMonths = endMonth - startMonth +1;
	// Number of months in range multiplied by average number of days in a month, divided by days in a week
	double numWeeks = numMonths * (29.5) / (7);

	std::cout << std::setw(35) << " " << this->name << std::setw(10) << " " << "Population: " << this->population << " \n";
	std::cout << std::setfill('_') << std::setw(80) << " " << " \n";

	std::cout << std::setfill(' ') << std::right << std::setw(15) << " " << "[Raw Totals]\n";
	std::cout << "Total Cases: " << std::setw(21) << " " << totalAvgCases << " \n";
	std::cout << "Average Weekly Cases: " << std::setw(12) << " " << totalAvgCases / numWeeks << " \n";
	std::cout << "Average Monthly Cases: " << std::setw(11) << " " << totalAvgCases / numMonths << " \n";

	std::cout << "Total Deaths: " << std::setw(20) << " " << totalAvgDeaths << " \n";
	std::cout << "Average Weekly Deaths: " << std::setw(11) << " " << totalAvgDeaths / numWeeks << " \n";
	std::cout << "Average Monthly Deaths: " << std::setw(10) << " " << totalAvgDeaths / numMonths << " \n\n";

	double casesNorm = (double)totalAvgCases / this->population;
	double deathsNorm = (double)totalAvgDeaths / this->population;


	std::cout << std::setfill(' ') << std::right << std::setw(5) << " " << "[Normalized Totals: Per 10,000 People] \n";
	std::cout << std::fixed << std::setprecision(3) << "Total Cases: " << std::setw(21) << " " << casesNorm * 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << "Average Weekly Cases " << std::setw(13) << " " << casesNorm / numWeeks * 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << "Average Monthly Cases " << std::setw(12) << " " << casesNorm / numMonths * 10000 << " \n";

	std::cout << std::fixed << std::setprecision(3) << "Total Deaths: " << std::setw(20) << " " << deathsNorm * 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << "Average Weekly Deaths " << std::setw(12) << " " << deathsNorm / numWeeks * 10000 << " \n";
	std::cout << std::fixed << std::setprecision(3) << "Average Monthly Deaths " << std::setw(11) << " " << deathsNorm / numMonths * 10000 << " \n\n";

}

int State::HashCounty(string& toHash, int sizeOGstr)
{
	int asciiSum = 0;
	int hashVal = 0;
	string name = toHash;

	for (int i = 0; i < toHash.size(); i++)
		asciiSum += toHash.at(i);
	hashVal = asciiSum%(int)(numCounties/0.7);
	if (countyMap[hashVal] == nullptr)
		return hashVal;
	//check if there is an element in position 'hashVal', and if so, if it is the county we are looking for. If not, recalculate hashVal. 
	if ((countyMap[hashVal]->GetName().compare(name.substr(0,sizeOGstr)) != 0)) 
	{
		name = Rehash(name);
		hashVal = HashCounty(name,sizeOGstr);
	}
	return hashVal;
}

string State::Rehash(const string& toRehash)
{
	return toRehash + "%";
}

vector<int> State::GetWeekData(int month, int startDay)
{
	int totalNewCases = 0;
	int totalCases = 0;
	int totalNewDeaths = 0;
	int totalDeaths = 0;

	// Go through every county and sum their respective weekly data; keys for each county are stored in countyHashKeys vector
	for (unsigned int i = 0; i < countyHashKeys.size(); i++)
	{
		int key = countyHashKeys.at(i);
		County* county = (countyMap[key]);
		//std::cout << "S115:" << county->GetName() << "\n";
		vector<Day*> currCountyWeek;
		// check if this county has data for this particular day; if not skip to next county.
		if (county->GetWeek(month, startDay, currCountyWeek) == false)
			continue;
		vector<int> currCountyWeekData = (county->GetWeekData(currCountyWeek));

		totalNewCases += currCountyWeekData.at(0);
		totalCases += currCountyWeekData.at(1);
		totalNewDeaths += currCountyWeekData.at(2);
		totalDeaths += currCountyWeekData.at(3);
	}
	vector<int> stateWeekData;
	stateWeekData.push_back(totalNewCases);
	stateWeekData.push_back(totalCases);
	stateWeekData.push_back(totalNewDeaths);
	stateWeekData.push_back(totalDeaths);

	return stateWeekData;
}

string State::GetName()
{
	return name;
}

int State::GetNumCounties()
{
	return numCounties;
}

int State::GetTotalDeaths()
{
	return totalDeaths;
}

int State::GetTotalPosCases()
{
	return totalPosCases;
}

int State::GetPopulation()
{
	return population;
}
