#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "County.h"
#include "State.h"
using std::vector;

class Nation
{
private:
	State** stateMap;	//hashtable of States
	int numStates;
	int* countiesPerState;	//hashtable with number of counties per state;

public:

		/*	Constructors and Destructor	*/

	Nation(vector<std::pair<string, int>>& stateNumCountyPair, int numStates); //these ints are used for calculating the size of hashtables generated for countyMap and stateMap
	Nation();
	~Nation();

		/*	Accessor Functions */

	State** GetStateMap();

		/*	Behavior and Tool Functions	*/		

	//returns hash value for key which is determined by ASCII sum of state name characters. Only used for stateMap
	int HashState(string& toHash);

	//returns hash value for key which is determined by ASCII sum of state name characters. Only used for countiesPerState
	int HashNumCounties(string& toHash);

	//appends "!" to end of string (which should be the name of a State), such that a new hashvalue can be calculated. ASCII code for 5 is 37, a prime number.
	string Rehash(string toRehash) ;

	

};

