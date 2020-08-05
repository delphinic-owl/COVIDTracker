#include "County.h"
#include "timer.h"

County::County()
{
    name = "";
    population = 0;
    division = 0;

    for (unsigned int i = 0; i < 12; i++)
        calendarList.push_back(nullptr);
}

County::County(string name, int pop, int division)
{
    this->name = name;
    this->population = pop;
    this->division = division;
    for (unsigned int i = 0; i < 12; i++)
        calendarList.push_back(nullptr);
}

County::~County()
{
    Day* curr;
    Day* next;
    for (unsigned int i = 0; i < calendarList.size(); i++)
    {   
        if (calendarList.at(i) == nullptr)
            continue;
        curr = calendarList.at(i);
        next = curr->GetNextDay();
        //std::cout << " C31: " << this->name << "\n";
        while (next != nullptr)
        {
            delete curr; 
            curr = next;
            next = next->GetNextDay();
        }
        delete curr;
    }
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


Day* County::GetDay(int month, int dayOfMonth)
{
    Day* curr = calendarList.at(month-1);
    while (curr!= nullptr && curr->GetNextDay() != nullptr)
    {
        if (curr->GetDayOfMonth() == dayOfMonth)
            return curr;
        else
            curr = curr->GetNextDay();
    }
    return curr;
}

vector<int> County::GetWeekData(vector<Day*>& week)
{
    int totalNewCases = 0;
    int totalCases = 0;
    int totalNewDeaths = 0;
    int totalDeaths = 0;
    for (unsigned int i = 0; i < week.size(); i++)
    {
        totalNewCases += week.at(i)->GetNewPosCases();
        totalNewDeaths += week.at(i)->GetNewDeaths();
        if (i == week.size() - 1)
        {
            totalCases = week.at(i)->GetPosCasesToDate();
            totalDeaths = week.at(i)->GetDeathsToDate();
        }
    }
    
    vector<int> weekData;
    weekData.push_back(totalNewCases);
    weekData.push_back(totalCases);
    weekData.push_back(totalNewDeaths);
    weekData.push_back(totalDeaths);
    return weekData;
}

bool County::GetWeek(int month, int startDay, vector<Day*>& week)
{
    int daysAdded = 0;
    int nextDay = startDay;

    Day* curr = GetDay(month,startDay);
    if (curr == nullptr) {
        std::cout << "No data for date in records for: " << this->name << "\n";
        return false;
    }

    while (daysAdded < 7)
    {
        if (curr == nullptr) return true;
        week.push_back(curr);
        daysAdded++;
        //std::cout << "C112: " << daysAdded << "\n";

        /* if curr->GetNextDay() returns nullptr, this means curr is the last day of this month. Must set curr to first day of next month.*/
        if (curr->GetNextDay() == nullptr) {
            /*check if current month is December, if it is, the next month is at first position in vector*/
            if (month == 12)
                curr = GetDay(1, 1);
            else
               curr = GetDay(month+1,1);
        }
        else
        {
        curr = curr->GetNextDay();
        }
    }
    return true;
}

void County::AddDay(int month, Day* toAdd)
{
    Day* prev = calendarList.at(month - 1);
    //Check if no days are registered for this month yet such as to avoid acting on nullptr
    if (prev == nullptr)
    {
        calendarList.at(month - 1) = toAdd;
        return;
    }
   // std::cout << "C132: toAdd: " << toAdd->GetDayOfMonth() <<" |prev: " << prev->GetDayOfMonth() << "\n";

    // loop exits once the end of the month as currently recorded is reached, or curr's date is found to be larger 
    int i = 0;
    while ((toAdd->GetDayOfMonth() > prev->GetDayOfMonth()))
    {
        i++;
        if (prev->GetNextDay() == nullptr)
            break; 
        else
        {
            prev = prev->GetNextDay();
        }
    }
    if (i == 0)
    { //toAdd is first in month (so far at least)
        toAdd->SetNextDay(prev);
        calendarList.at(month - 1) = toAdd;
       // std::cout << "C148: toAdd: " << toAdd->GetDayOfMonth() << " |prev: " << prev->GetDayOfMonth() << "\n\n";
    }
    else
    { // this is if toAdd comes after prev, whatever prev is
        toAdd->SetNextDay(prev->GetNextDay());
        prev->SetNextDay(toAdd);
       // std::cout << "C153: toAdd: " << toAdd->GetDayOfMonth() << " |prev: " << prev->GetDayOfMonth() << "\n\n";
    }
}

void County::PrintWeek(int month, int startDay)
{
    vector<Day*> week;
    GetWeek(month, startDay, week);
    timer t;
    t.Start();
    vector<int> weekData = GetWeekData(week);
    cout << "Week Data Found in: " << t.reportTime() << " milliseconds" << endl;

    std::cout << "\nFor the 7 days starting on " << month << "/" << startDay << ":\n\n";
    std::cout << std::setw(35) << " " << this->name  << std::setw(10) << " " << "Population: " << this->population << " \n";
    std::cout << std::setfill('_') << std::setw(80) << " " << " \n";

    std::cout << std::setfill(' ') << std::right << std::setw(15) << " " << "[Raw Totals]\n";
    std::cout << "New Cases: " << std::setw(23) << " " <<  weekData.at(0) << " \n";
    std::cout << "Positive Cases to Date " << std::setw(11) << " " << weekData.at(1) << " \n";
    std::cout << "New Deaths: " << std::setw(22) << " " << weekData.at(2) << " \n";
    std::cout << "Deaths to Date " << std::setw(19) << " " << weekData.at(3) << "\n\n";

    double ncNorm = (double)(weekData.at(0)) / this->population;
    double pscdNorm = (double)(weekData.at(1)) / this->population;
    double ndNorm = (double)(weekData.at(2)) / this->population;
    double dtdNorm = (double)(weekData.at(3)) / this->population;

    std::cout << std::setfill(' ') << std::right << std::setw(5) << " " << "[Normalized Totals: Per 10,000 People]\n";
    std::cout << "New Cases: " << std::setw(23) << " " << ncNorm* 10000 << " \n";
    std::cout << "Positive Cases to Date " << std::setw(11) << " " << pscdNorm * 10000 << " \n";
    std::cout << "New Deaths: " << std::setw(22) << " " << ndNorm * 10000 << " \n";
    std::cout << "Deaths to Date " << std::setw(19) << " " << dtdNorm * 10000 << " \n\n";


}

void County::PrintAvg(int startMonth, int endMonth, int endDay)
{
    int numMonths = endMonth - startMonth + 1;
       // Number of months in range multiplied by average number of days in a month, divided by days in a week
    double numWeeks = numMonths * (29.5) / (7);
    
    int end = endDay;

    while (GetDay(endMonth, end) == nullptr && endDay > 0) 
    {
        end--;
        if (end == 0) {
            endMonth = endMonth - 1;
            end = endDay - 1 ;
        }
    }
    int posCases = GetDay(endMonth, end)->GetPosCasesToDate();
    int numDeaths = GetDay(endMonth, end)->GetDeathsToDate();

    std::cout << std::setw(35) << " " << this->name << std::setw(10) << " " << "Population: " << this->population << " \n";
    std::cout << std::setfill('_') << std::setw(80) << " " << " \n";

    std::cout << std::setfill(' ') << std::right << std::setw(15) << " " << "[Raw Totals]\n";
    std::cout << "Total Cases: " << std::setw(21) << " " << posCases << " \n";
    std::cout << "Average Weekly Cases: " << std::setw(12) << " " << posCases /numWeeks << " \n";
    std::cout << "Average Monthly Cases: " << std::setw(11) << " " << posCases /numMonths << " \n";

    std::cout << "Total Deaths: " << std::setw(20) << " " << numDeaths << " \n";
    std::cout << "Average Weekly Deaths: " << std::setw(11) << " " << numDeaths /numWeeks << " \n";
    std::cout << "Average Monthly Deaths: " << std::setw(10) << " " << numDeaths /numMonths << " \n\n";

    double casesNorm = (double)posCases / this->population;
    double deathsNorm = (double)numDeaths / this->population;


    std::cout << std::setfill(' ') << std::right << std::setw(5) << " " << "[Normalized Totals: Per 10,000 People] \n";
    std::cout << std::fixed << std::setprecision(3) << "Total Cases: " << std::setw(21) << " " << casesNorm * 10000 << " \n";
    std::cout << std::fixed << std::setprecision(3) << "Average Weekly Cases " << std::setw(13) << " " << casesNorm/numWeeks * 10000 << " \n";
    std::cout << std::fixed << std::setprecision(3) << "Average Monthly Cases " << std::setw(12) << " " << casesNorm/numMonths * 10000 << " \n";

    std::cout << std::fixed << std::setprecision(3) << "Total Deaths: " << std::setw(20) << " " << deathsNorm * 10000 << " \n";
    std::cout << std::fixed << std::setprecision(3) << "Average Weekly Deaths " << std::setw(12) << " " << deathsNorm/numWeeks * 10000 << " \n";
    std::cout << std::fixed << std::setprecision(3) << "Average Monthly Deaths " << std::setw(11) << " " << deathsNorm/numMonths * 10000 << " \n\n";

}




