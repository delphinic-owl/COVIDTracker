#pragma once
#include <iostream>
#include "County.h"
#include <vector>
using std::vector;


class State
{
public:
	State();
	State(int numCounties);
	~State();

	/*
	Behaviors/Tools
	*/

	//returns hash value for key which is determined by ASCII sum of county name characters
	int HashCounty(string& toHash);

	//appends "!" to end of string (which should be the name of a State), such that a new hashvalue can be calculated. ASCII code for 5 is 37, a prime number.
	string Rehash(const string& toRehash);

	/*
		Accessors
	*/
	string GetName();
	int GetNumCounties();
	int GetTotalDeaths();
	int GetTotalPosCases();
	County** GetCountyMap();

private:
	County** countyMap;
	string name;
	int numCounties;
	int totalDeaths;
	int totalPosCases;
};

