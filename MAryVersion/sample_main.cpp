#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <string>
#include <map>
#include <list>
#include <iterator>
#include <vector>
#include <map>
#include <math.h>
#include <algorithm>
#include <cctype>
#include "Node.h"
#include "Node.cpp"
#include "data_importation.h"
#include "timer.h"
using namespace std;

//Test if region name exists in node maps.
bool testValidInput(string input, Node* root)
{
	if (root->nodeMap.find(input) == root->nodeMap.end())
	{
		cout << "\nRegion not found, please enter a valid region name (Make sure only the first letter is capitalized)\n\n" << endl;
		return false;
	}
	else
		return true;
}

// sets first character to upper case and rest of characters to lower case, such that the proper hash value can be retrieved for the State or County this string represents.
string FixStringCases(string s)
{
	std::transform(s.begin(), s.begin() + 1, s.begin(), std::toupper);
	std::transform(s.begin() + 1, s.end(), s.begin() + 1, std::tolower);
	unsigned int temp = s.find_first_of(' ');
	while (temp != string::npos && temp + 1 < s.size())
	{
		//first letter of new word (comes after a space) is capitalized
		std::transform(s.begin() + temp + 1, s.begin() + 2 + temp, s.begin() + temp + 1, std::toupper);
		temp = s.find_first_of(' ', temp + 1);
	}

	return s;
}

int main() {
	std::cout << "Welcome to the COVID-19 Activity Tracker! \n";
	std::cout << "Here you can get newly reported COVID-19 statistics for any particular US county or state. \n\n";
	std::cout << "You can also compare two states or counties side by side!" << std::endl;

	int option = 0;
	std::cout << "Data in this program can be stored and retrieved in one of two ways: \n\t(1) M-ary Search Tree or (2) Hash Tables\n Would you like to use 1, or 2?\n\n";
	while (option != 1 && option != 2)
	{
		while (!(cin >> option)) 
		{

			if (cin.eof()) {
				cout << "user terminated input." << endl;
				return 0;
			}
			cout << "invalid input (not a number); try again." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		if (option != 1 && option != 2)
			cout << "invalid input; choose 1 or 2." << endl;
	}

	if (option == 1) 
	{
		//Code to execute M-ary Search Tree
		timer t;
		t.Start();
		Node *root = new Node();
		root->name = "USA";

		CSVReader* data = new CSVReader("Census Data and Pop. Estimates.csv", "COVID19 Activity Cleaned.csv");
		root->nodeVector.clear();
		root->nodeMap.clear();
		// TODO Function to print out time taken to load files
		data->getData(0);
		root->createNodes(data, root);

		root->calcTotals(root, "country");
		root->calcTotalSeverity(root);
		cout << "Successfully loaded all data in: " << t.reportTime() << " milliseconds\n" << endl;
		while (option != 0)
		{
			option = 5;
			cout << "Choose from the following options, or enter 0 to stop the program: \n";
			cout << right << setw(55) << "(1) Get Total Report for Single State \n";
			cout << right << setw(56) << "(2) Get Total Report for Single County \n";
			cout << right << setw(57) << "(3) Compare Total Report for Two States \n";
			cout << right << setw(59) << "(4) Compare Total Report for Two Counties \n";
			cout << right << setw(26) << "(0) Exit \n";


			string input = "";
			bool acceptable = false;
			while (option != 0 && option != 1 && option != 2 && option != 3 && option != 4)
			{
				while (!(cin >> option))
				{

					if (cin.eof()) {
						cout << "user terminated input." << endl;
						return 0;
					}
					cout << "invalid input (not a number); try again." << endl;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				if (option != 0 && option != 1 && option != 2 && option != 3 && option != 4)
					cout << "invalid input; choose from menu." << endl;
			}

			if (option == 0)
			{
				cout << "COVID-19 Tracker closing...";
				return 0;
			}
			if (option == 1)
			{
				cout << "What state would you like COVID stats on?\n";
				cin.ignore();
				while (!acceptable) { getline(cin, input); acceptable = testValidInput(input, root); }

				t.Start();
				root->stateNode(input)->PrintTotals();
				cout << "Data compiled in: " << t.reportTime() << " milliseconds" << endl;
				cout << "\n\n";
			}
			else if (option == 2)
			{
				cout << "What state is the county of interest in?\n";
				cin.ignore();
				while (!acceptable) { getline(cin, input); acceptable = testValidInput(input, root); }
				Node* state = root->stateNode(input);
				cout << "\nThese are all of " << input << "'s counties: \n";
				state->PrintAllCounties();

				cout << "\n\nWhich county are you interested in?\n";
				while (acceptable) { getline(cin, input); acceptable = !(testValidInput(input, state)); }
				cout << "You chose " << input << "\n";
				Node* county = state->countyNode(input);

				t.Start();
				county->PrintTotals();
				cout << "Data compiled in: " << t.reportTime() << " milliseconds" << endl;
				cout << "\n\n";
			}
			else if (option == 3)
			{
				cout << "What is the first state you would like to compare?\n";
				cin.ignore();
				while (!acceptable) { getline(cin, input); acceptable = testValidInput(input, root); }
				Node* state1 = root->stateNode(input);
				cout << "You chose " << input << "\n\n";

				cout << "What is the second state you would like to compare?\n";
				while (acceptable) { getline(cin, input); acceptable = !(testValidInput(input, root)); }
				Node* state2 = root->stateNode(input);
				cout << "You chose " << input << "\n\n";

				t.Start();
				root->CompareRegions(*state1, *state2);
				cout << "Data compiled in: " << t.reportTime() << " milliseconds" << endl;
				cout << "\n\n";
			}
			else if (option == 4)
			{
				cout << "Which state is the first county you would like to compare in?\n";
				cin.ignore();
				while (!acceptable) { getline(cin, input); acceptable = testValidInput(input, root); }
				cout << "\nThese are all of " << input << "'s counties: \n";
				Node* state1 = root->stateNode(input);
				state1->PrintAllCounties();

				cout << "\n\nWhich of these counties are you interested in? \n";
				while (acceptable) { getline(cin, input); acceptable = !(testValidInput(input, state1)); }
				cout << "You chose " << input << "\n\n";
				Node* county1 = state1->countyNode(input);

				cout << "What state is the second county you would like to compare in?\n";
				while (!acceptable) { getline(cin, input); acceptable = testValidInput(input, root); }
				Node* state2 = root->stateNode(input);
				cout << "\nThese are all of " << input << "'s counties: \n";
				state2->PrintAllCounties();

				cout << "\n\nWhich of these counties are you interested in? \n";
				while (acceptable) { getline(cin, input); acceptable = !(testValidInput(input, state2)); }
				cout << "You chose " << input << "\n\n";
				Node* county2 = state2->countyNode(input);
				
				t.Start();
				root->CompareRegions(*county1, *county2);
				cout << "Data compiled in: " << t.reportTime() << " milliseconds" << endl;
				cout << "\n\n";
			}
			else
			{
				cout << "Invalid Input, try again.";
			}

		}
	}
	else {
		//Code to execute Hash Table
		/*
		std::ifstream numCountyCSV;
		Nation USA = Nation(numCountyCSV, 51);
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

				USA.GetStateMap()[USA.HashState(stateIn1, stateIn1.size())]->PrintWeek(month, day);
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

				std::cout << "These are all of " << stateIn1 << "'s counties: \n";
				state->PrintAllCounties();
				std::cout << "\n\nWhich county are you interested in?\n";
				std::getline(std::cin, countyIn1);
				countyIn1 = FixStringCases(countyIn1);
				std::cout << "You chose " << countyIn1 << "\n";
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
				State* state1 = USA.GetStateMap()[USA.HashState(stateIn1, stateIn1.size())];
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

		}*/
	}

};