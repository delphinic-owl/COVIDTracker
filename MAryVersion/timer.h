#include <algorithm>
#include <chrono>
#include <iostream>
#pragma once
using namespace std;
using namespace std::chrono;

class timer
{
private:

	steady_clock::time_point start;
	steady_clock::time_point stop;
	steady_clock::time_point lap;
	microseconds duration = duration_cast<microseconds>(stop - start);

	void Stop() { stop = high_resolution_clock::now(); };		//Stop time
	void Lap() { lap = high_resolution_clock::now(); };		//Lap time (for multiple reports)

public:

	timer() {};

	void Start() { start = high_resolution_clock::now(); };	//Starting time
	void Duration(steady_clock::time_point currTime) { duration = duration_cast<microseconds>(currTime - start); };	//Calculates total duration from start to current time or stop point

	double reportTime()		//Stops timer, reports duration
	{
		Stop();
		Duration(stop);
		return (double) duration.count() / 1000;
	};
	double currentTime()		//Reports duration without stopping the timer
	{
		Lap();
		Duration(lap);
		return (double) duration.count() / 1000;
	};
};