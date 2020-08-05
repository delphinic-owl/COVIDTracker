#pragma once
#include <iostream>
#include <vector>
#include "County.h"
using std::vector;


class State
{
public:
	State();
	State(string name, int numCounties);
	~State();

	/*
	Mutators
	*/
	void SetPopulation(int num);
	/*
		Accessors
	*/

	string GetName();
	int GetNumCounties();
	int GetTotalDeaths();
	int GetTotalPosCases();
	int GetPopulation();
	// returns vector containing 4 integers representing the following stats for a week, IN THIS ORDER: total new cases, total cases to date, total new deaths, total deaths to date. Input vector "week" must already have Days of interest stored.
	vector<int> GetWeekData(int month, int startDay);
	County** GetCountyMap();
	// returns vector containing all keys needed for direct access to every County within this state's hashmap.
	vector<int>& GetCountyHashKeys();

	/*=================Behaviors================*/
	//Gets rid of county hash keys for which there was no corresponding data loaded from the Covid-Activity CSV
	void RemoveEmptyCounties();
	void PrintAllCounties();
	void PrintWeek(int month, int startDay);
	void PrintWeekAvg(int startMonth, int endMonth, int endDay);
	//returns hash value for key which is determined by ASCII sum of county name characters
	int HashCounty(string& toHash, int sizeOGstr);

	//appends "%" to end of string (which should be the name of a State), such that a new hashvalue can be calculated. ASCII code for % is 37, a prime number.
	string Rehash(const string& toRehash);

private:
	County** countyMap;
	vector<int> countyHashKeys;
	string name;
	int numCounties;
	int totalDeaths;
	int totalPosCases;
	int population;
};

