#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include <list>
#include <iterator>
#include <vector>
#include <map>
#include <math.h>
#include <algorithm>
using namespace std;

template <class T>
class CSVReader
{
private:

	string censusFile;
	string deathFile;
	string activityFile;
	string delimiter;

public:

	vector<vector<T>> dataList;

	CSVReader(string filename1, string filename2, string filename3 string delm = ",") :
		censusFile(filename1), deathFile(filename2), activityFile(filename3), delimiter(delm)
	{}

	void getCensusData();
	void getDeathData();
	void getActivityData();
};

//Call the functions in order, adding each file's data to the template vectors within the vector for each county
//When data is imported, convert integers back to integers
//May need to reorder data and put it into another vector for assigning to nodes

template <class T>
void CSVReader<T>::getCensusData()
{
	ifstream file(censusFile);
	string line = "";
	int i = 0;
	int j = 1;
	while (getline(file, line, delimiter))
	{
		// sumLev, region, division, state, county, stateName, cityName, census2010Pop, popEstimate2019
		dataList[i].push_back(line);
		if (j % 9 == 0)	//Switch to next vector every 9 iterations
			i++;
		j++;
	}
}

template <class T>
void CSVReader<T>::getDeathData()
{

}

template <class T>
void CSVReader<T>::getActivityData()
{

}
