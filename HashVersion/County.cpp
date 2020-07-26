#include "County.h"

County::County()
{
    name = "";
    population = 0;
    division = 0;
    stateFIPS = 0;
    totalDeaths = 0;
    totalPosCases = 0;
}

County::County(string name, int pop, int division, int state)
{
    this->name = name;
    this->population = pop;
    this->division = division;
    stateFIPS = state;
    totalDeaths = 0;
    totalPosCases = 0;
}

County::~County()
{
}

string County::GetName()
{
    return name;
}

int County::GetPopulation()
{
    return population;
}

int County::GetDivision()
{
    return division;
}

int County::GetState()
{
    return stateFIPS;
}

int County::GetTotalDeaths()
{
    return totalDeaths;
}

int County::GetPosCases()
{
    return totalPosCases;
}

Day* County::GetDay(int month, int dayOfMonth)
{
    Day* curr = calendarList.at(month-1);
    while (curr->GetNextDay() != nullptr)
    {
        if (curr->GetDayOfMonth() == dayOfMonth)
            return curr;
    }
    std::cout << "Day not found\n";
    return nullptr;
}

void County::GetWeek(vector<Day*>& week, int month, int startDay)
{
    int daysAdded = 1;
    int nextDay = startDay;

    Day* curr = GetDay(month,startDay);
    if (curr == nullptr) {
        return;
    }
    while (daysAdded < 7)
    {
        week.push_back(curr);
        daysAdded++;
        /* if curr->GetNextDay() returns nullptr, this means curr is the last day of this month. Must set curr to first day of next month.*/
        if (curr->GetNextDay() == nullptr) {
            /*check if current month is December, if it is, the next month is at first position in vector*/
            if (month == 12)
                curr = calendarList.at(0);
            else
               curr = calendarList.at(month);
        }
        curr = curr->GetNextDay();
    }
}

void County::AddDay(int month, Day* toAdd)
{
    Day* curr = calendarList.at(month - 1);
    //Check if no days are registered for this month yet such as to avoid acting on nullptr
    if (curr == nullptr)
        calendarList.at(month - 1) = toAdd;
    Day* temp = nullptr;

    // loop exits once the end of the month as currently recorded is reached, or curr's date is found to be larger 
    while ((toAdd->GetDayOfMonth() > curr->GetDayOfMonth()) && (curr->GetNextDay() != nullptr))
    {
        temp = curr;
        curr = curr->GetNextDay();
    }
    temp->SetNextDay(toAdd);
    toAdd->SetNextDay(curr);
}


