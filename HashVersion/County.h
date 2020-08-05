#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include "Day.h"
using std::vector;
using std::string;

class County
{
private:
	string name;
	int population, division;
	vector<Day*> calendarList; // Positions of vector correspond to (month - 1); i.e. January, the first month, is in position 0. Each position holds pointer to the data for first day of the month, which points to the rest of the days of that month in a singly-linked list format.

public:
	County();
	County(string name, int pop, int division);
	~County();
	
	/*==============Accessors================*/

	string GetName();
	int GetPopulation();
	int GetDivision();


	//month is EXACT month; i.e. input 3 for March
	Day* GetDay(int month, int dayOfMonth);
	// returns vector containing 4 integers representing the following stats for a week, IN THIS ORDER: total new cases, total cases to date, total new deaths, total deaths to date. Input vector "week" must already have Days of interest stored.
	vector<int> GetWeekData(vector<Day*>& week);

	//returns a vector which containins seven Day*. Said vector represents a week/particular stretch of seven days. Could further modularize this function by adding an input integer to designate the number of days to return.
	bool GetWeek(int month, int startDay, vector<Day*>& week);

	/*====================Behavior/Mutators=================*/

	// adds day in correct place in calendarList vector 
	void AddDay(int month, Day* toAdd);

	// Prints out formatted week data (based off of 7 averaged days)
	void PrintWeek(int month, int startDay);
};

