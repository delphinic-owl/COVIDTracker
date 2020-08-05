#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "County.h"
#include "State.h"
using std::vector;

class Nation
{
private:
	State** stateMap;	//hashtable of States
	int numStates;

public:

		/*===========Constructors and Destructor============*/

	Nation(std::ifstream& numCountyCSV, int numStates); //these ints are used for calculating the size of hashtables generated for countyMap and stateMap
	Nation();
	~Nation();

		/*===========Accessor Functions================*/

	State** GetStateMap();

		/*==================Behavior Functions===================*/		

	//returns hash value for key which is determined by ASCII sum of state name characters. Only used for stateMap
	int HashState(std::string& toHash, int sizeOGstr);

	//appends "%" to end of string (which should be the name of a State), such that a new hashvalue can be calculated. ASCII code for % is 37, a prime number.
	std::string Rehash(std::string toRehash) ;

	void CompareCounties(County& county1, County& county2, int month, int startDay);
	void CompareStates(State& state1 , State& state2, int month, int startDay);

};

