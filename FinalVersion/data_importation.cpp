#include "data_importation.h"
using namespace std;

/*Total entries and indexes for:
			(3) census data:	0 stateName,	1 countyName,	2 popEstimate2019, (3 stateBoolean if true)
			(5) activity data:	0 totalCases,	1 countyName,	2 stateName,	3 date,		4 deathCount
*/

void CSVReader1::getData(int fileNum)
{
    timer t;
    t.Start();
    ifstream file(fileNames[fileNum]);

    //census data:		0 stateName, 1 countyName, 2 2019 pop. estimate, (3 stateBoolean if true)
    //activity data:	3 totalCases, 4 weeklyCases, 5 monthlyCases, 6 totalDeaths, 7 weeklyDeaths, 8 monthlyDeaths

    string line = "";
    string entry = "";
    int i = 0;
    size_t pos = 0;
    getline(file, line);			//Removes first row containing headers
    std::cout << "Loading File Number " << fileNum + 1 << endl;
    while (getline(file, line))		//Read first line of data
    {
        if (i == 800 || i == 1600 || i == 2400)
            std::cout << "(" << (((double)i / 3200) * 100) << "%) " << endl;     //Report file percentage and time

        vector<string> dataVec;
        bool isState = true;

        if (fileNum == 1) { dataVec = getActiveData(file); }
        else
        {
            string stateName, countyName;
            int entryNum = 0;

            while ((pos = line.find(delimiter)) != string::npos)
            {
                entry = line.substr(0, pos);
                if (entry.find(" County") != string::npos) //If county is found in name, remove it
                {
                    isState = false;
                    size_t index = entry.find(" County");
                    entry.erase(index, string::npos);
                }
                dataVec.push_back(entry);
                entryNum++;
                line.erase(0, pos + delimiter.length());
            }
            dataVec.push_back(line);
            if (isState) { dataVec.push_back("true"); }
        }

        if (fileNum == 0)
        {
            string stateCounty = dataVec[0] + dataVec[1];
            dataList.push_back(dataVec);
            if (!isState) { countyVecMap[stateCounty] = i; }	//emplace "StatenameCountyname" key to vector pointer value. Does not contain states.
        }
        else
        {
            int size = dataVec.size();
            string stateCounty = dataVec[size - 2] + dataVec[size - 1];
            if (countyVecMap.find(stateCounty) != countyVecMap.end()) //If state & county match a node from previous file, add data to it (minus region names)
            {
                for (int i = 0; i < size - 2; i++)  //Loop through all data and exclude name and county
                {
                    dataList[countyVecMap[stateCounty]].push_back(dataVec[i]);
                }
            }
            else	//If region did not exist in previous file, add to incomplete list
                incompleteList.push_back(dataVec);
        }
        i++;
    }
    cout << "(100%) " << t.reportTime() << " milliseconds\n" << endl;
    if (fileNum < 1)
        getData(++fileNum);	//Repeat for next file
}

vector<string> CSVReader1::getActiveData(ifstream &file)
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
    dataVec.push_back(entry[i]);										                    //totalCases		0
    dataVec.push_back(to_string((double)(stod(entry[i])) / (double) (122.0 / 7.0)));		//weekly cases		1
    dataVec.push_back(to_string((double)(stod(entry[i++])) / (double) 4.0));				//monthly cases		2

    countyName = entry[i++];	//countyName
    stateName = entry[i++];		//stateName
    i++;	                    //date is unused currently

    dataVec.push_back(entry[i]);										                    //totalDeaths		3
    dataVec.push_back(to_string((double)(stod(entry[i])) / (double) (122.0 / 7.0)));		//weekly deaths		4
    dataVec.push_back(to_string((double)(stod(entry[i++])) / (double) 4.0));				//monthly deaths	5
    dataVec.push_back(stateName);										                    //stateName			6
    dataVec.push_back(countyName);										                    //countyName		7

    return dataVec;
}