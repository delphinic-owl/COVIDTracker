#include "State.h"

State::State()
{
	countyMap = nullptr;
	totalDeaths = 0;
	totalPosCases = 0;
	numCounties = 0;
}

State::State(int numCounties)
{
	this->numCounties = numCounties;
	countyMap = new County*[numCounties];
	totalDeaths = 0;
	totalPosCases = 0;
}

State::~State()
{
	for (int i = 0; i < numCounties; i++)
		delete countyMap[i];
}

County** State::GetCountyMap()
{
	return nullptr;
}

//ToDo
int State::HashCounty(string& toHash)
{
	int asciiSum = 0;
	int hashVal = 0;
	string name = toHash;

	for (int i = 0; i < toHash.size(); i++)
		asciiSum += toHash.at(i);
	hashVal = asciiSum;
	//check if there is an element in position 'hashVal', and if so, if it is the county we are looking for. If not, recalculate hashVal. 
	while ((countyMap[hashVal] != nullptr) && (countyMap[hashVal]->GetName().compare(toHash) != 0)) 
	{
		name = Rehash(name);
		hashVal = HashCounty(name);
	}
	return hashVal;
}

string State::Rehash(const string& toRehash)
{
	return toRehash + "!";
}

string State::GetName()
{
	return name;
}

int State::GetNumCounties()
{
	return numCounties;
}

int State::GetTotalDeaths()
{
	return totalDeaths;
}

int State::GetTotalPosCases()
{
	return totalPosCases;
}
