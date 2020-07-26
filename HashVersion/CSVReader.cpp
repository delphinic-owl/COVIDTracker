#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Nation.h"

class CSVReader
{
	void ReadCensusCSV(Nation& nation);
	void ReadActivityCSV();
	void ReadStateDataCSV();

};

void CSVReader::ReadCensusCSV(Nation& nation) 
{
	std::ifstream census;
	census.open("Census Data and Pop. Estimates.csv");
	if (!census.is_open()) 
	{
		std::cout << "census file not opened.";
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

		std::getline(census, placeholder, ',');
		population = std::stoi(placeholder);

		int stateHashVal = nation.HashState(stateName);
		State* stateAtHash = nation.GetStateMap()[stateHashVal];

		//check if state is already present in State* hashtable; if not create and add at position 'stateHashVal' in stateMap, which is as hashtable within Nation
		if (countyFips != 0 || stateAtHash == nullptr) 
		{
			State* newState = new State(nation.HashNumCounties(stateName));
			stateAtHash = newState;
			nation.GetStateMap()[stateHashVal] = newState;
		}

		int countyHashVal = stateAtHash->HashCounty(countyName);
		County* countyAtHash = stateAtHash->GetCountyMap()[countyHashVal];
		if (countyAtHash == nullptr)	// county doesn't exist- so add it to countyMap which is a hashtable within the State
		{
			County* newCounty = new County(countyName, population, division, stateFips);
			countyAtHash = newCounty;
			stateAtHash->GetCountyMap()[countyHashVal] = newCounty;
		}
	}
}

void CSVReader::ReadActivityCSV()
{
}

void CSVReader::ReadStateDataCSV()
{
}
