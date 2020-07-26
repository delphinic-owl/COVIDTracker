#include "Nation.h"

Nation::Nation(vector<std::pair<string, int>>& stateNumCountyPair, int numStates)
{
	this->numStates = numStates;
	
	// creation of hashtable for States
	int sizeStateTable = (int)(numStates / 0.7);
	this->stateMap = new State*[sizeStateTable];

	// tracking number of counties per state allows for size determination of hashtable for that State's counties.
	// size of this hashtable is the same as the size of stateMap
	int sizeCountyTable = (int)(numStates / 0.7);
	this->countiesPerState = new int[sizeCountyTable];

	//fill in hashtable, countiesPerState, in fashion identical to stateMap initialization
	int hashVal = 0;
	for (int i = 0; i < stateNumCountyPair.size(); i++)
	{
		string name = stateNumCountyPair.at(i).first;
		hashVal = HashNumCounties(name);
		//while there is an element at hashVal's position and that element is not the element to be inserted; recalculate hashVal by add an arbitrarily chosen, but constant character to the string.
		while (countiesPerState[hashVal] != 0 && countiesPerState[hashVal] != stateNumCountyPair.at(i).second)
		{
			name = Rehash(name);
			hashVal = HashNumCounties(name);
		}
		countiesPerState[hashVal] = stateNumCountyPair.at(i).second;
	}
}

Nation::Nation()
{
	stateMap = nullptr;
	countiesPerState = nullptr;
	numStates = 0;
}

Nation::~Nation()
{
	for (int i = 0; i < this->numStates; i++) {
		stateMap[i]->~State();
	}
}

State** Nation::GetStateMap()
{
	return stateMap;
}

int Nation::HashState(string& toHash)
{
	int asciiSum = 0;
	int hashVal = 0;
	string name = toHash;

	for (int i = 0; i < toHash.size(); i++)
		asciiSum += toHash.at(i);
	hashVal = asciiSum;
	//check if there is an element in position 'hashVal', and if so, if it is the state we are looking for. If not, recalculate hashVal. 
	if ((stateMap[hashVal] != nullptr) && stateMap[hashVal]->GetName() != toHash)
	{
		name = Rehash(name);
		hashVal = HashState(name);
	}
	return hashVal;
}

int Nation::HashNumCounties(string& toHash)
{
	int asciiSum = 0;
	int hashVal = 0;

	for (int i = 0; i < toHash.size(); i++)
		asciiSum += toHash.at(i);
	hashVal = asciiSum % (int)(numStates / 0.7);
	return hashVal;
}

string Nation::Rehash(string toRehash) 
{
	return toRehash + "!";
}


