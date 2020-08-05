
#include "Main.h"

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

bool testValidInput(string input)
{
	vector<string> stateNames = { "Alabama", "Alaska", "Arizona", "Arkansas", "California", "Colorado", "Connecticut", "Delaware", "District of Columbia", "Florida", "Georgia", "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas", "Kentucky", "Louisiana", "Maine", "Maryland", "Massachusetts", "Michigan", "Minnesota", "Mississippi", "Missouri", "Montana", "Nebraska", "Nevada", "New Hampshire", "New Jersey", "New Mexico", "New York", "North Carolina", "North Dakota", "Ohio", "Oklahoma", "Oregon", "Pennsylvania", "Rhode Island", "South Carolina", "South Dakota", "Tennessee", "Texas", "Utah", "Vermont", "Virginia", "Washington", "West Virginia", "Wisconsin", "Wyoming" };
	if (find(stateNames.begin(), stateNames.end(), input) == stateNames.end())
	{
		cout << "\nRegion not found, please enter a valid region name (Make sure only the first letter is capitalized)\n\n" << endl;
		return false;
	}
	else
		return true;
}

// sets first character of each word to upper case and rest of characters to lower case, such that the proper hash value can be retrieved for the State or County this string represents. Returns adjusted string
string FixStringCases(string s)
{
	std::transform(s.begin(), s.begin() + 1, s.begin(), std::toupper);
	std::transform(s.begin() + 1, s.end(), s.begin() + 1, std::tolower);
	unsigned int temp = s.find_first_of(' ');
	unsigned int temp2 = s.find_first_of('-');
	while ((temp != string::npos && temp + 1 < s.size()) || (temp2 != string::npos && temp2 + 1 < s.size()))
	{
		//do not capitalize "and" or "of"
		if ((temp + 1 == s.find("and", temp+1)) || (temp + 1 == s.find("of", temp + 1)))
		{
			temp = s.find_first_of(' ', temp + 1);
			continue;
		}
		//first letter of new word (recognized as a character that comes after a space) is capitalized
		if (temp != string::npos && temp + 1 < s.size())
		{
			std::transform(s.begin() + temp + 1, s.begin() + 2 + temp, s.begin() + temp + 1, std::toupper);
			temp = s.find_first_of(' ', temp+1);
		}
		//first letter of new word (recognized as a character that comes after a hyphen) is capitalized. This applies to a select few counties, such as some in Alaska.
		if(temp2 != string::npos && temp + 1 < s.size())
		{
			std::transform(s.begin() + temp2 + 1, s.begin() + 2 + temp2, s.begin() + temp2 + 1, std::toupper);
			temp2 = s.find_first_of('-', temp2+1);
		}
	}
	return s;
}

// Prompts User until they give valid input state name.
double CheckState(Nation& nation, State*& state)
{
	double reportTime = 0;		
	timer t;
	string input = "";
	string yn = "No";

	while (yn != "yes" && yn != "y" && yn != "Yes")
	{
		getline(cin, input);
		input = FixStringCases(input);
		t.Start();
		state = nation.GetStateMap()[nation.HashState(input, input.size())];
		reportTime = t.reportTime();
		if (state == nullptr)
			std::cout << "Invalid state name.\n";
		else
		{
			std::cout << "Did you mean " << state->GetName() << "? Yes, or no?\n";
			getline(cin, yn);
		}
		if (yn == "no" || yn == "No") {
			std::cout << "Please type state name again.\n";
		}
		else if (yn == "yes" || yn == "y" || yn == "Yes")
		{
			t.Start();
			nation.GetStateMap()[nation.HashState(input, input.size())];
			cout << "State found in: " << t.reportTime() << " milliseconds" << endl;
		}
		else
		{
			std::cout << "Could not understand answer, please type state name again.\n";
		}
	}
	return reportTime;
}

// Prompts User until they give valid input county name.
double CheckCounty(Nation& nation, State*& state, County*& county)
{
	double reportTime = 0;
	timer t;
	string input = "";
	string yn = "No";

	while (yn != "yes" && yn != "y" && yn != "Yes")
	{
		getline(cin, input);
		t.Start();
		input = FixStringCases(input);
		county = state->GetCountyMap()[state->HashCounty(input,input.size())];
		reportTime = t.reportTime();
		if (county == nullptr)
			std::cout << "Invalid county name.\n";
		else
		{
			std::cout << "Did you mean " << county->GetName() << "? Yes, or no?\n";
			getline(cin, yn);
		}
		if (yn == "no" || yn == "No") 
		{
			std::cout << "Please type state name again.\n";
		}
		else if (yn == "yes" || yn == "y" || yn == "Yes")
		{
			t.Start();
			state->GetCountyMap()[state->HashCounty(input, input.size())];
			cout << "County found in: " << t.reportTime() << " milliseconds" << endl;
		}
		else
		{
			std::cout << "Could not understand answer, please type state name again.\n";
		}
	}
	return reportTime;
}

int CheckMonth(int month)
{
	while (!(month >= 3 && month <= 6))
	{
		while (!(cin >> month))
		{

			if (cin.eof()) {
				cout << "user terminated input." << endl;
				return 0;
			}
			cout << "invalid input (not a number); try again." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		if (!(month >= 3 && month <= 6))
			cout << "invalid input; choose within data time range (3-6)." << endl;
	}
	return month;
}

int CheckDay(int month, int day)
{
	while (!(day >= 1 && day <= 31) || (month == 6 && day > 23) || ((month == 6 || month == 4) && day == 31))
	{
		while (!(cin >> day))
		{

			if (cin.eof()) {
				cout << "user terminated input." << endl;
				return 0;
			}
			cout << "invalid input (not a number); try again." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		if (!(day >= 1 && day <= 31))
			cout << "invalid input; choose a valid day for the month (1-31)" << endl;
		if ((month == 6 && day > 23))
			cout << "invalid input; data range ends on 6/30/2020" << endl;
		if ((month == 6 || month == 4) && day == 31)
			cout << "invalid input; specified month does not have 31 days" << endl;
	}
	return day;
}

int HashWeekBased(Nation& USA, int option)
{
	int month = 0;
	int day = 0;
	timer t;

	string input = "";
	string yn = "No";
	double reportTime = 0;

	if (option == 1)
	{
		std::cout << "What state would you like COVID stats on?\n";
		std::cin.ignore();
		State* state = nullptr;
		reportTime = CheckState(USA, state);


		std::cout << "For the week (7 consecutive days) of interest, what month does it start in? Enter number that corresponds to the month.\n";
		month = CheckMonth(month);
		std::cout << "What day of the month does this week start? \n";
		day = CheckDay(month, day);
		if (month == 0 || day == 0)
			return 0;

		t.Start();
		state->PrintWeek(month, day);
		reportTime += t.reportTime();
		cout << "Data compiled in: " << reportTime << " milliseconds" << endl;
		std::cout << "\n\n";

	}
	else if (option == 2)
	{
		State* state = nullptr;
		County* county = nullptr;

		std::cout << "What state is the county of interest in?\n";
		std::cin.ignore();
		reportTime = CheckState(USA, state);

		std::cout << "These are all of " << state->GetName() << "'s counties: \n";
		state->PrintAllCounties();
		std::cout << "\n\nWhich county are you interested in?\n";
		reportTime += CheckCounty(USA, state, county);

		std::cout << "For the week (7 consecutive days) of interest, what month does it start in? Enter number that corresponds to the month, i.e. 3 for March, 10 for October.\n";
		month = CheckMonth(month);
		std::cout << "What day of the month does this week start? \n\n";
		day = CheckDay(month, day);
		if (month == 0 || day == 0)
			return 0;

		t.Start();
		county->PrintWeek(month, day);
		cout << "Data compiled in: " << t.reportTime() << " milliseconds" << endl;
		std::cout << "\n\n";
	}
	else if (option == 3)
	{
		State* state1 = nullptr;
		State* state2 = nullptr;

		std::cout << "What is the first state you would like to compare?\n";
		std::cin.ignore();
		reportTime = CheckState(USA, state1);

		std::cout << "What is the second state you would like to compare?\n";
		reportTime += CheckState(USA, state2);

		std::cout << "For the week (7 consecutive days) of interest, what month does it start in? Enter number that corresponds to the month.\n";
		month = CheckMonth(month);
		std::cout << "What day of the month does this week start? \n\n";
		day = CheckDay(month, day);
		if (month == 0 || day == 0)
			return 0;

		t.Start();
		USA.CompareStates(*state1, *state2, month, day);
		reportTime += t.reportTime();
		cout << "Data compiled in: " << reportTime << " milliseconds" << endl;
		std::cout << "\n\n";
	}
	else if (option == 4)
	{
		State* state1 = nullptr;
		County* county1 = nullptr;
		State* state2 = nullptr;
		County* county2 = nullptr;

		std::cout << "Which state is the first county you would like to compare in?\n";
		std::cin.ignore();
		reportTime += CheckState(USA, state1);
		std::cout << "These are all of " << state1->GetName() << "'s counties: \n";
		state1->PrintAllCounties();
		std::cout << "\n\nWhich of these counties are you interested in? \n";
		reportTime += CheckCounty(USA, state1, county1);

		std::cout << "What state is the second county you would like to compare in?\n";
		reportTime += CheckState(USA, state2);
		std::cout << "These are all of " << state2->GetName() << "'s counties: \n";
		state2->PrintAllCounties();
		std::cout << "\n\nWhich of these counties are you interested in? \n";
		reportTime += CheckCounty(USA, state2, county2);

		std::cout << "\nFor the week (7 consecutive days) of interest, what month does it start in? Enter number that corresponds to the month.\n";
		month = CheckMonth(month);
		std::cout << "What day of the month does this week start? \n\n";
		day = CheckDay(month, day);
		if (month == 0 || day == 0)
			return 0;

		t.Start();
		USA.CompareCounties(*county1, *county2, month, day);
		reportTime += t.reportTime();

		cout << "Data compiled in: " << reportTime << " milliseconds" << endl;
	}
	return option;
}

int HashTotalBased(Nation& USA, int option)
{
	int month = 0;
	int day = 0;
	timer t;

	string input = "";
	string yn = "No";
	double reportTime = 0;

	if (option == 5)
	{
		std::cout << "What state would you like COVID stats on?\n";
		std::cin.ignore();
		State* state = nullptr;
		reportTime = CheckState(USA, state);

		t.Start();
		state->PrintWeekAvg(3,6,30);
		reportTime += t.reportTime();
		cout << "Data compiled in: " << reportTime << " milliseconds" << endl;
		std::cout << "\n\n";

	}
	else if (option == 6)
	{
		State* state = nullptr;
		County* county = nullptr;

		std::cout << "What state is the county of interest in?\n";
		std::cin.ignore();
		reportTime = CheckState(USA, state);

		std::cout << "These are all of " << state->GetName() << "'s counties: \n";
		state->PrintAllCounties();
		std::cout << "\n\nWhich county are you interested in?\n";
		reportTime += CheckCounty(USA, state, county);


		t.Start();
		county->PrintAvg(3, 6, 30);
		cout << "Data compiled in: " << t.reportTime() << " milliseconds" << endl;
		std::cout << "\n\n";
	}
	else if (option == 7)
	{
		State* state1 = nullptr;
		State* state2 = nullptr;

		std::cout << "What is the first state you would like to compare?\n";
		std::cin.ignore();
		reportTime = CheckState(USA, state1);

		std::cout << "What is the second state you would like to compare?\n";
		reportTime += CheckState(USA, state2);

		t.Start();
		USA.CompareStatesAvg(*state1, *state2, 3,6,30);
		reportTime += t.reportTime();
		cout << "Data compiled in: " << reportTime << " milliseconds" << endl;
		std::cout << "\n\n";
	}
	else if (option == 8)
	{
		State* state1 = nullptr;
		County* county1 = nullptr;
		State* state2 = nullptr;
		County* county2 = nullptr;

		std::cout << "Which state is the first county you would like to compare in?\n";
		std::cin.ignore();
		reportTime += CheckState(USA, state1);
		std::cout << "These are all of " << state1->GetName() << "'s counties: \n";
		state1->PrintAllCounties();
		std::cout << "\n\nWhich of these counties are you interested in? \n";
		reportTime += CheckCounty(USA, state1, county1);

		std::cout << "What state is the second county you would like to compare in?\n";
		reportTime += CheckState(USA, state2);
		std::cout << "These are all of " << state2->GetName() << "'s counties: \n";
		state2->PrintAllCounties();
		std::cout << "\n\nWhich of these counties are you interested in? \n";
		reportTime += CheckCounty(USA, state2, county2);

		t.Start();
		USA.CompareCountiesAvg(*county1, *county2, 3,6,30);
		reportTime += t.reportTime();

		cout << "Data compiled in: " << reportTime << " milliseconds" << endl;
	}
	return option;
}

int main() {
	std::cout << "Welcome to the COVID-19 Activity Tracker! \n";
	std::cout << "Here you can get newly reported COVID-19 statistics for any particular US county or state. \n\n";
	std::cout << "You can also compare two states or counties side by side!" << std::endl;

	int option = 0;
	bool hash = false;
	bool mary = false;
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

	while (option != 0)
	{
		if (option == 1)
		{
			//Code to execute M-ary Search Tree
			timer t;
			mary = true;
			t.Start();
			Node* root = new Node();
			root->name = "USA";

			CSVReader1* data = new CSVReader1("mCensus Data and Pop. Estimates.csv", "COVID19 Activity Cleaned.csv");
			root->nodeVector.clear();
			root->nodeMap.clear();
			// TODO Function to print out time taken to load files
			data->getData(0);
			root->createNodes(data, root);

			root->calcTotals(root, "country");
			root->calcTotalSeverity(root);
			cout << "\n\n\nSuccessfully loaded all data in: " << t.reportTime() << " milliseconds\n" << endl;
			while (option != 0 && option != 9)
			{
				option = 5;
				cout << "Choose from the following options, or enter 0 to stop the program: \n";
				cout << right << setw(55) << "(1) Get Total Report for Single State \n";
				cout << right << setw(56) << "(2) Get Total Report for Single County \n";
				cout << right << setw(57) << "(3) Compare Total Report for Two States \n";
				cout << right << setw(59) << "(4) Compare Total Report for Two Counties \n";
				cout << right << setw(26) << "(0) Exit \n";
				cout << right << setw(38) << "(9) Switch Structure \n";

				string input = "";
				bool acceptable = false;
				vector<int> menuItems = { 0,1,2,3,4,9 };
				while (find(menuItems.begin(), menuItems.end(), option) == menuItems.end())
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
					if (find(menuItems.begin(), menuItems.end(), option) == menuItems.end())
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

					// Print out time taken to locate State and County
					t.Start();
					root->stateNode(input);
					cout << "State found in: " << t.reportTime() << " milliseconds" << endl;

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

					//Print out time taken to locate State and County
					t.Start();
					root->stateNode(input);
					cout << "State found in: " << t.reportTime() << " milliseconds" << endl;
					t.Start();
					state->countyNode(input);
					cout << "County found in: " << t.reportTime() << " milliseconds" << endl;

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
				else if (option == 9)
				{
					cout << "Switching data structure...\n" << endl;
					delete root;
					delete data;
				}
				else
				{
					cout << "Invalid Input, try again.";
				}
			}
		}
		else 
		{
			timer t;
			hash = true;
			t.Start();
			std::ifstream numCountyCSV;
			Nation USA = Nation(numCountyCSV, 51);
			std::cout << "Now loading population data from Census file...\n\n";
			CSVReader::ReadCensusCSV(USA);
			std::cout << "Now loading COVID-19 data from COVID Activity file...\n(This may take up to 30 seconds)\n\n";
			CSVReader::ReadActivityCSV(USA);

			cout << "Successfully loaded all data in: " << t.reportTime() << " milliseconds\n" << endl;

			while (option != 0 && option != 9)
			{
				option = -1;
				std::cout << "Choose from the following options, or enter 0 to stop the program: \n";
				std::cout << std::right << std::setw(56) << "(1) Get Week-based Report for Single State \n";
				std::cout << std::right << std::setw(57) << "(2) Get Week-based Report for Single County \n";
				std::cout << std::right << std::setw(58) << "(3) Compare Week-based Report for Two States \n";
				std::cout << std::right << std::setw(60) << "(4) Compare Week-based Report for Two Counties \n";
				std::cout << std::right << std::setw(56) << "(5) Get Total-based Report for Single State \n";
				std::cout << std::right << std::setw(57) << "(6) Get Total-based Report for Single County \n";
				std::cout << std::right << std::setw(58) << "(7) Compare Total-based Report for Two States \n";
				std::cout << std::right << std::setw(60) << "(8) Compare Total-based Report for Two Counties \n";
				cout << right << setw(22) << "(0) Exit \n";
				cout << right << setw(34) << "(9) Switch Structure \n";
				
				int month = 0;
				int day = 0;

				string input = "";
				string yn = "No";
				double reportTime = 0;
				bool acceptable = false;
				vector<int> menuItems = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

				while (find(menuItems.begin(), menuItems.end(), option) == menuItems.end())
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
					if (find(menuItems.begin(), menuItems.end(), option) == menuItems.end())
						cout << "invalid input; choose from menu." << endl;
				}
				if (option == 0)
				{
					std::cout << "COVID-19 Tracker closing...";
					return 0;
				}
				else if (option >= 1 && option <= 4)
					option = HashWeekBased(USA, option);
				else if (option >= 5 && option <= 8)
					option = HashTotalBased(USA, option);
				else if (option == 9)
				{
					cout << "Switching data structure...\n" << endl;
				}
				else
				{
					std::cout << "Invalid Input, try again.";
				}

			}
		}
		if (option == 9 && mary) { option = 2; mary = false; }
		if (option == 9 && hash) { option = 1; hash = false; }
	}
};