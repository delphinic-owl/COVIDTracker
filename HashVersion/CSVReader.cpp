#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Nation.h"
#include "CSVReader.h"

void CSVReader::ReadCensusCSV(Nation& nation) 
{
	std::ifstream census;
	census.open("HashVersion/Census Data and Pop. Estimates.csv");
	if (!census.is_open()) 
	{
		std::cout << "census file not opened.\n";
		return;
	}
	string placeholder = "";
	std::getline(census, placeholder, '\n'); //get rid of column headers

	int region = 0;
	int division = 0;
	int stateFips = 0;
	int countyFips = 0;
	string stateName = "";
	string countyName = "";
	int population = 0;
	int row = 1;

	while (!census.eof())
	{
		std::getline(census, placeholder, ',');
		region = std::stoi(placeholder);

		std::getline(census, placeholder, ',');
		division = std::stoi(placeholder);

		std::getline(census, placeholder, ',');
		stateFips = std::stoi(placeholder);

		std::getline(census, placeholder, ',');
		countyFips = std::stoi(placeholder);

		std::getline(census, stateName, ',');
		std::getline(census, countyName, ',');

		std::getline(census, placeholder, '\n');
		population = std::stoi(placeholder);

		int stateHashVal = nation.HashState(stateName, stateName.size());
		State* stateAtHash = nation.GetStateMap()[stateHashVal];
		// A countyFips of 0 means this line of data represents an entire State population
		if (countyFips == 0) 
		{
			stateAtHash->SetPopulation(population);
			continue;
		}
		int countyHashKey = stateAtHash->HashCounty(countyName, countyName.size());
		County* countyAtHash = stateAtHash->GetCountyMap()[countyHashKey];
		if (countyAtHash == nullptr)	// county doesn't exist- so add it to countyMap which is a hashtable within the State
		{
			countyAtHash = new County(countyName, population, division);
			stateAtHash->GetCountyMap()[countyHashKey] = countyAtHash;
			stateAtHash->GetCountyHashKeys().push_back(countyHashKey);
		}
	}
	census.close();
}

// Appends " County" to end of string for county name, since this document lacks it
void CSVReader::ReadActivityCSV(Nation& nation)
{
	std::ifstream activity;
	activity.open("HashVersion/COVID-19-Activity Cleaned.csv");
	if (!activity.is_open())
	{
		std::cout << "COVID Activity file not opened.";
		return;
	}
	string placeholder = "";
	std::getline(activity, placeholder, '\n'); //get rid of column headers

	//in order of read in 
	int totalCases = 0;
	string countyName = "";			// needs input processing besides stoi()
	string stateName = "";			
	int reportMonth = 0;	
	int reportDay = 0;
	int reportYear = 0;
	int newDeaths = 0;
	//int countyAndStateFIPS = 0;
	int newCases = 0;
	int totalDeaths = 0;
	//std::cout << "AF: 98 \n";
	int row = 1;

	//This whole loop creates and adds new Day (i.e. entire line of COVID Activity file - that represents statistics for a particular county on a particular day) to the corresponding County, which should have been created previously by CSVReader::ReadCensusCSV(Nation& nation).
	while (!activity.eof())
	{
		std::getline(activity, placeholder, ',');
		totalCases = std::stoi(placeholder);

		std::getline(activity, countyName, ',');
		// to get the correct hashkey, 'County' must be appended (as the census.csv is the file which initializes most of the counties, and its county names include 'County')
		if (countyName.compare("Unknown") == 0)
		{
			std::getline(activity, placeholder, '\n');
			row++;
			continue;
		}

		std::getline(activity, stateName, ',');
		/* there are several states with 'county equivalents' rather than traditional counties. This line accounts for that
		 Louisiana has all 'Parishes'; Alaska has all 'Census areas' and 'Boroughs'; several states have cities which are designated as having County properties. These are specified in both the Census and COVID-Activity CSV files, whereas 'County' is a designation only present in the Census CSV; hence the need to append 'County' to data from COVID-Activity CSV which does not fall under the previously described exceptions.
		 */
		if(countyName.find("Anchorage") !=string::npos)
			countyName = countyName+ " Municipality";
		else if (stateName.find("Louisiana") != string::npos)
			countyName = countyName + " Parish";
		else if((stateName.compare("Alaska") !=0) && (countyName.find("City") == string::npos) && (stateName.compare("District of Columbia") !=0))
			countyName = countyName + " County";

		//parse "Report Date" column
		std::getline(activity, placeholder, '/');
		reportMonth	= std::stoi(placeholder);
		std::getline(activity, placeholder, '/');
		reportDay = std::stoi(placeholder);
		std::getline(activity, placeholder, ',');
		reportYear = std::stoi(placeholder);

		std::getline(activity, placeholder, ',');
		newDeaths = std::stoi(placeholder);

		/*std::getline(activity, placeholder, ',');
		countyAndStateFIPS = std::stoi(placeholder);	//currently not doing anything with this; though it could be used as confirmed unique identifiers/keys; would have to parse into State FIPS and County FIPS*/

		std::getline(activity, placeholder, ',');
		newCases = std::stoi(placeholder);

		std::getline(activity, placeholder, '\n');
		totalDeaths = std::stoi(placeholder);

		int stateHashVal = nation.HashState(stateName,stateName.size());
		State* stateAtHash = nation.GetStateMap()[stateHashVal];

		//check if state is already present in State* hashtable; if not create and add at position 'stateHashVal' in stateMap, which is a hashtable within Nation
		/*if (stateAtHash == nullptr)
		{
			State* newState = new State(stateName, nation.HashNumCounties(stateName));
			stateAtHash = newState;
			nation.GetStateMap()[stateHashVal] = newState;
		}*/

		int countyHashKey = stateAtHash->HashCounty(countyName, countyName.size());
		County* countyAtHash = stateAtHash->GetCountyMap()[countyHashKey];
		//std::cout << "AF:147 County: " << countyName << "\t stateHashKey|countyHashKey: " << stateHashVal << "|" << countyHashKey << "\n";
		if (countyAtHash == nullptr)	// county doesn't exist-
		{
			std::cout << "Error: " << countyName << " does not exist. Meaning, it was not loaded from Census CSV.";
		}
		Day* toAdd= new Day(reportDay, newCases, newDeaths, totalDeaths, totalCases);	/// assume there are no duplicate dates
		countyAtHash->AddDay(reportMonth, toAdd);
		row++;
	}
	activity.close();
}

void CSVReader::ReadStateDataCSV()
{
}
