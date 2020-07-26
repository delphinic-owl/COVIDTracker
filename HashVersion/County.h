#pragma once
#include <iostream>
#include <vector>
#include "Day.h"
using std::vector;
using std::string;

class County
{
private:
	string name;
	int population, division, stateFIPS;
	int totalDeaths;
	int totalPosCases;
	vector<Day*> calendarList; // Positions of vector correspond to (month - 1); i.e. January, the first month, is in position 0. Each position holds pointer to the data for first day of the month, which points to the rest of the days of that month in a singly-linked list format.

public:
	County();
	County(string name, int pop, int division, int state);
	~County();
	
	/*Accessors*/

	string GetName();
	int GetPopulation();
	int GetDivision();
	int GetState();
	int GetTotalDeaths();
	int GetPosCases();

	//month is EXACT month; i.e. input 3 for March
	Day* GetDay(int month, int dayOfMonth);

	//adds seven Day* to input vector which correspond to a particular stretch of seven days. Could further modularize this function by adding an input integer to designate the number of days to return.
	void GetWeek(vector<Day*>& week, int month, int startDay);

	/* Behavior/Mutators */

	// adds day in correct place in calendarList vector 
	void AddDay(int month, Day* toAdd);



};

