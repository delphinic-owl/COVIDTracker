#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
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
#include "data_importation.h"
using namespace std;

/*Total entries and indexes for:
			(9) census data:	0 sumLev,		1 region,		2 division,		3 state,	4 county,		5 stateName,	6 countyName,	7 census2010Pop,	8 popEstimate2019
			(8) activity data:	0 totalCases,	1 countyName,	2 stateName,	3 date,		4 newDeaths,	5 countyNum,	6 newCases,		7 deathCount
			(?) death data:		*unknown*

			Update if statement in getData when death data indices are found
*/

//When data is imported, convert from string to appropriate data type
//May need to reorder, sort through data and/or put it into another vector for assigning to nodes

void CSVReader::getData(int fileNum)
{
    //Statements to determine data input based on file

    ifstream file(fileNames[fileNum]);

    bool firstRead = false;
    bool stateFirst;
    vector<int> stateCountyIndexes;
    vector<int> uselessDataIndexes;	//This is used to exclude certain data that isn't currently helpful

    if (fileNum == 0) { stateCountyIndexes = { 5, 6 }; uselessDataIndexes = { 0, 1, 2, 3, 4, 8 };  stateFirst = true; firstRead = true; }	//census data:		0 census2010Pop, 1 stateName, 2 countyName
    if (fileNum == 1) { stateCountyIndexes = { 1, 2 }; stateFirst = false; }																//activity data:	3 totalCases, 4 weeklyCases, 5 monthlyCases, 6 totalDeaths, 7 weeklyDeaths, 8 monthlyDeaths
    if (fileNum == 2) { stateCountyIndexes = { 0, 0 }; stateFirst = false; }																//death data:		*UPDATE ZEROS*

    string line = "";
    string entry = "";
    int i = 0;
    size_t pos = 0;
    getline(file, line);			//Removes first row containing headers
    cout << "Loading File Number " << fileNum + 1 << endl;
    while (getline(file, line))		//Read first line of data
    {
        if (i == 800 || i == 1600 || i == 2400)
            cout << "(" << (((double)i / 3200) * 100) << "%)" << endl;
        vector<string> dataVec;
        bool isState = true;

        if (fileNum == 1)
            dataVec = getActiveData(file);
        else
        {
            string stateName, countyName;
            int entryNum = 0;

            while ((pos = line.find(delimiter)) != string::npos)
            {
                entry = line.substr(0, pos);
                //bool name = (indexIt != stateCountyIndexes.end());		// identify region name input
                //bool useless = (indexIt2 != uselessDataIndexes.end());	// identify useless data input
                if (entryNum == 5 || entryNum == 6)
                {
                    if (entry.find(" County") != string::npos) { isState = false; size_t index = entry.find(" County"); entry.erase(index, string::npos); }	//Remove " County" from county names if there
                    if (stateFirst) { stateName = entry; stateFirst = false; }	//If state input comes first, assign and switch to county
                    else { countyName = entry; stateFirst = true; }			//Else, assign county name and switch to state
                }
                if (entryNum != 8 && !(entryNum >= 0 && entryNum <= 6))	//Add names last and skip useless data
                    dataVec.push_back(entry);
                entryNum++;
                line.erase(0, pos + delimiter.length());
            }
            dataVec.push_back(line);

            //Before region names, define if state or not (some counties have the same name as their state
            if (isState)
                dataVec.push_back("true");
            else
                dataVec.push_back("false");

            dataVec.push_back(stateName);
            dataVec.push_back(countyName);
        }

        int size = dataVec.size();
        auto namesIndex = dataVec.end() - 2;
        string stateCounty = *namesIndex + *(namesIndex + 1);

        if (firstRead)
        {
            dataList.push_back(dataVec);
            if (!isState) { countyVecMap[stateCounty] = i; }	//emplace "StatenameCountyname" key to vector pointer value. Does not contain states.
        }
        else if (countyVecMap.find(stateCounty) != countyVecMap.end()) //If state & county match a node from previous file, add data to it (minus region names)
        {
            for (int i = 0; i < size - 2; i++)
            {
                dataList[countyVecMap[stateCounty]].push_back(dataVec[i]);
            }
        }
        else	//If region did not exist in previous file, add to incomplete list
            incompleteList.push_back(dataVec);
        i++;
    }
    cout << "(100%)" << endl;
    if (fileNum < 1)	//Change this to 2 to read the third file
        getData(++fileNum);	//Repeat for next file
}

vector<string> CSVReader::getActiveData(ifstream &file)
{
    //Each county has 122 dates with 8 inputs each
    //activity data : 0 totalCases, 1 countyName, 2 stateName, 3 date, 4 newDeaths, 5 countyNum, 6 newCases, 7 deathCount

    /* Calculations:
        average weekly cases/deaths: total / (122 days / 7 days per week)
        average monthly cases/deaths: total / 4 months

        dates could be summarized into 4 months, with relevant data being stored in a vector of vectors (i.e. { {marchCases, marchDeaths}, {aprilCases, aprilDeaths}, etc. }
            implementing this into the m-ary structure would be difficult with what I have currently, but this was only mentioned as an additional feature
            newDeaths and newCases are unhelpful as calculations can be done off of totals at the end. Can be included in monthly stats if desired
    */

    //Already read line 1 of 122

    string stateName = "";
    string countyName = "";
    size_t pos = 0;
    vector<string> dataVec;

    string line = "";
    vector<string> entry;
    while (getline(file, line) && line.find("6/30/2020") == string::npos);	//Cycle through each line until last date is found

    //Retrieve necessary values, calculate, and add them to vector:		Variable			Index
    while (pos = line.find(delimiter) != string::npos)
    {
        pos = line.find(delimiter);
        entry.push_back(line.substr(0, pos));
        line.erase(0, pos + delimiter.size());
    }
    entry.push_back(line);
    int i = 0;
    dataVec.push_back(entry[i]);										//totalCases		0
    dataVec.push_back(to_string(stod(entry[i]) / (122.0 / 7.0)));		//weekly cases		1
    dataVec.push_back(to_string(stod(entry[i++]) / 4.0));				//monthly cases		2

    countyName = entry[i++];					//countyName
    stateName = entry[i++];					//stateName

    //Unused data
    i++;	//date
    i++;	//newDeaths
    i++;	//countyNum
    i++;	//newCases

    entry[i].pop_back(); // Remove /r from end of totalDeaths
    dataVec.push_back(entry[i]);										//totalDeaths		3
    dataVec.push_back(to_string(stod(entry[i]) / (122.0 / 7.0)));		//weekly deaths		4
    dataVec.push_back(to_string(stod(entry[i++]) / 4.0));				//monthly deaths	5
    dataVec.push_back("false");											//State boolean		6
    dataVec.push_back(stateName);										//stateName			7
    dataVec.push_back(countyName);										//countyName		8

    return dataVec;
}