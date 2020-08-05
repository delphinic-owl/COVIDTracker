#pragma once
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
#include "County.h"
#include "CSVReader.h"
#include "Day.h"
#include "Nation.h"
#include "State.h"
#include "Node.h"
#include "data_importation.h"
#include "timer.h"
using namespace std;


//Test if region name exists in node maps.
bool testValidInput(string input, Node* root);

bool testValidInput(string input);

// sets first character of each word to upper case and rest of characters to lower case, such that the proper hash value can be retrieved for the State or County this string represents. Returns adjusted string
string FixStringCases(string s);

// Prompts User until they give valid input state name.
double CheckState(Nation& nation, State*& state);

// Prompts User until they give valid input county name.
double CheckCounty(Nation& nation, State*& state, County*& county);

int CheckMonth(int month);

int CheckDay(int month, int day);

int HashWeekBased(Nation& USA, int option);

int HashTotalBased(Nation& USA, int option);