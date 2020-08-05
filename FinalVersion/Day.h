#pragma once


class Day
{
private:
	int dayOfMonth;
	int newPosCases;
	int newDeaths;
	int deathsToDate;
	int posCasesToDate;
	Day* nextDay;

public:
	Day();
	Day(int dayOfMonth, int newPosCases, int newDeaths, int deathsToDate, int posCasesToDate);
	~Day();

	void SetNextDay(Day* next);
	Day* GetNextDay();
	int GetNewPosCases();
	int GetNewDeaths();
	int GetDeathsToDate();
	int GetPosCasesToDate();
	int GetDayOfMonth();

};

