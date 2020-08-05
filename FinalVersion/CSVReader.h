#pragma once
#include "Nation.h"
class CSVReader
{
public:
	static void ReadCensusCSV(Nation& nation);
	static void ReadActivityCSV(Nation& nation);
	static void ReadStateDataCSV();
	
};