#include "Day.h"
#include <iostream>

Day::Day()
{
	nextDay = nullptr;
	dayOfMonth = 0;
	newPosCases = 0;
	newDeaths = 0;
	deathsToDate = 0;
}

Day::Day(int dayOfMonth, int newPosCases, int newDeaths, int deathsToDate, int posCasesToDate)
{
	this->dayOfMonth = dayOfMonth;
	this->newPosCases = newPosCases;
	this->newDeaths = newDeaths;
	this->deathsToDate = deathsToDate;
	this->posCasesToDate = posCasesToDate;
	nextDay = nullptr;
}

Day::~Day()
{
	//std::cout << 1;
}

void Day::SetNextDay(Day* next) 
{
	nextDay = next;
}

Day* Day::GetNextDay()
{
	return nextDay;
}

int Day::GetNewPosCases()
{
	return newPosCases;
}

int Day::GetNewDeaths()
{
	return newDeaths;
}

int Day::GetDeathsToDate()
{
	return deathsToDate;
}

int Day::GetPosCasesToDate()
{
	return posCasesToDate;
}

int Day::GetDayOfMonth()
{
	return dayOfMonth;
}
