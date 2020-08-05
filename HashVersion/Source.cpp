#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iomanip>
#include "CSVReader.h"
using std::string;
using std::vector;

/*void LoadCountyNums(vector<std::pair<string, int>> countyCountPairs)
{
	std::ifstream nation;
	nation.open("Number of Counties Per State.csv");
	if (!nation.is_open())
	{
		std::cout << "Counties Per State file not opened.";
		return;
	}
	string countyNumString = "";
	string stateName = "";
	int rowNum = 1;
	while (!nation.eof()) {
		std::getline(nation, countyNumString, ',');
		std::getline(nation, stateName, '\n');
		//std::cout << "27: " << rowNum << ": " << countyNumString << "\t" << stateName << "\n";
		int numCounties = std::stoi(countyNumString);
		countyCountPairs.push_back(std::make_pair(stateName, numCounties));
		rowNum++;
	}
	nation.close();
}*/

// sets first character to upper case and rest of characters to lower case, such that the proper hash value can be retrieved for the State or County this string represents.
string FixStringCases(string s) 
{
	std::transform(s.begin(), s.begin()+1, s.begin(), std::toupper);
	std::transform(s.begin() + 1, s.end(), s.begin() + 1, std::tolower);
	unsigned int temp = s.find_first_of(' ');
	while (temp != string::npos && temp+1 <s.size())
	{
		//do not capitalize "and"
		if ((temp + 1 == s.find_first_of("and", temp)) || (temp + 1 == s.find_first_of("of",temp)))
		{
			temp = s.find_first_of(' ', temp + 1);
			continue;
		}
		//first letter of new word (recognized as a character that comes after a space) is capitalized
		std::transform(s.begin()+temp+1, s.begin() + 2 + temp, s.begin() + temp+1, std::toupper);
		temp = s.find_first_of(' ', temp+1);
	}

	return s;
}

int main(){
	std::cout << "Welcome to the COVID-19 Activity Tracker! \n";
	std::cout << "Here you can get newly reported weekly and monthly stats such as new cases and deaths for any particular US county or state. \n\n";
	std::cout << "You can also compare two states or counties side by side!" << std::endl;

	int option = 0;
	std::cout << "Data in this program can be stored and retrieved in one of two ways: \n(1) M-ary Search Tree or (2) Hash Tables\n Would you like to use 1, or 2?\n\n";
	std::cin >> option;
	while (option != 1 && option != 2) {
		std::cout << "Invalid input." << std::endl;
	}
	if (option == 1) {
		//TO:DO Input code to execute M-ary Search Tree
	}
	else {
		std::ifstream numCountyCSV;
		Nation USA = Nation(numCountyCSV,51);
		std::cout << "Now loading population data from Census file...\n\n";
		CSVReader::ReadCensusCSV(USA);
		// TODO Function to print out time taken to load file
		std::cout << "Now loading COVID-19 data from COVID Activity file...\n\n";
		CSVReader::ReadActivityCSV(USA);
		// TODO Function to print out time taken to load file


		string stateIn1 = "";
		string stateIn2 = "";
		string countyIn1 = "";
		string countyIn2 = "";
		int month = 0;
		int day = 0;

		while (option != 0) 
		{
			std::cout << "Choose from the following options, or enter 0 to stop the program: \n";
			std::cout << std::right << std::setw(56) << "(1) Get Week-based Report for Single State \n";
			std::cout << std::right << std::setw(57) << "(2) Get Week-based Report for Single County \n";
			std::cout << std::right << std::setw(57) << "(3) Compare Week-based Report for Two States \n";
			std::cout << std::right << std::setw(59) << "(4) Compare Week-based Report for Two Counties \n"; 

			std::cin >> option;
			if (option == 0)
			{
				std::cout << "COVID-19 Tracker closing...";
				return 0;
			}
			//TODO: implement timer
			if (option == 1) 
			{
				std::cout << "What state would you like COVID stats on?\n";
				std::cin.ignore();
				std::getline(std::cin, stateIn1);
				stateIn1 = FixStringCases(stateIn1);

				std::cout << "For the week (7 consecutive days) of interest, what month does it start in? Enter number that corresponds to the month.\n";
				std::cin >> month;
				std::cout << "What day of the month does this week start? \n\n";
				std::cin >> day;

				USA.GetStateMap()[USA.HashState(stateIn1, stateIn1.size())]->PrintWeek(month,day);
				std::cout << "\n\n\n";

			}
			//TODO: implement timer
			else if (option == 2)
			{
				std::cout << "What state is the county of interest in?\n";
				std::cin.ignore();
				std::getline(std::cin, stateIn1);
				stateIn1 = FixStringCases(stateIn1);
				State* state = USA.GetStateMap()[USA.HashState(stateIn1, stateIn1.size())];
				
				std::cout << "These are all of " << stateIn1 << "'s counties: \n" ;
				state->PrintAllCounties();
				std::cout << "\n\nWhich county are you interested in?\n";
				std::getline(std::cin,countyIn1);
				countyIn1 = FixStringCases(countyIn1);
				std::cout << "You chose " << countyIn1 << "\n" ;
				County* county = state->GetCountyMap()[state->HashCounty(countyIn1, countyIn1.size())];

				std::cout << "For the week (7 consecutive days) of interest, what month does it start in? Enter number that corresponds to the month, i.e. 3 for March, 10 for October.\n";
				std::cin >> month;
				std::cout << "What day of the month does this week start? \n\n";
				std::cin >> day;

				county->PrintWeek(month, day);
				std::cout << "\n\n\n";

			}
			else if (option == 3)
			{
				std::cout << "What is the first state you would like to compare?\n";
				std::cin.ignore();
				std::getline(std::cin, stateIn1);
				stateIn1 = FixStringCases(stateIn1);
				State* state1 = USA.GetStateMap()[USA.HashState(stateIn1, stateIn1.size())];
				std::cout << "You chose " << stateIn1 << "\n";

				std::cout << "What is the second state you would like to compare?\n";
				std::getline(std::cin, stateIn2);
				stateIn2 = FixStringCases(stateIn2);
				State* state2 = USA.GetStateMap()[USA.HashState(stateIn2, stateIn2.size())];
				std::cout << "You chose " << stateIn2 << "\n";


				std::cout << "For the week (7 consecutive days) of interest, what month does it start in? Enter number that corresponds to the month.\n";
				std::cin >> month;
				std::cout << "What day of the month does this week start? \n\n";
				std::cin >> day;

				USA.CompareStates(*state1, *state2, month, day);
				std::cout << "\n\n\n";
			}
			else if (option == 4)
			{
				std::cout << "Which state is the first county you would like to compare in?\n";
				std::cin.ignore();
				std::getline(std::cin, stateIn1);
				stateIn1 = FixStringCases(stateIn1);
				std::cout << "These are all of " << stateIn1 << "'s counties: \n";
				State* state1 = USA.GetStateMap()[USA.HashState(stateIn1,stateIn1.size())];
				state1->PrintAllCounties();
				std::cout << "\n\nWhich of these counties are you interested in? \n";
				std::getline(std::cin, countyIn1);
				countyIn1 = FixStringCases(countyIn1);
				std::cout << "You chose " << countyIn1 << "\n";
				County* county1 = state1->GetCountyMap()[state1->HashCounty(countyIn1, countyIn1.size())];
				
				std::cout << "What state is the second county you would like to compare in?\n";
				std::getline(std::cin, stateIn2);
				stateIn2 = FixStringCases(stateIn2);
				State* state2 = USA.GetStateMap()[USA.HashState(stateIn2, stateIn2.size())];
				std::cout << "These are all of " << stateIn2 << "'s counties: \n";
				state2->PrintAllCounties();
				std::cout << "\n\nWhich of these counties are you interested in? \n";
				std::getline(std::cin, countyIn2);	
				countyIn2 = FixStringCases(countyIn2);
				std::cout << "You chose " << countyIn2 << "\n";
				County* county2 = state2->GetCountyMap()[state2->HashCounty(countyIn2, countyIn2.size())];
				
				std::cout << "\nFor the week (7 consecutive days) of interest, what month does it start in? Enter number that corresponds to the month.\n";
				std::cin >> month;
				std::cout << "What day of the month does this week start? \n\n";
				std::cin >> day;

				USA.CompareCounties(*county1, *county2, month, day);
			}
			else 
			{
				std::cout << "Invalid Input, try again.";
			}
				
		}
	}

};