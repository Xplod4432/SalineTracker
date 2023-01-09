#include <chrono>
#include <iostream>
#include <Windows.h>
using namespace std;
using namespace std::chrono;
//trigger the function if sudden increase in weight is seen (for example zero to 500gm)
//Start measuring time when nurse says drip started

//Wait till 1gm change is recorded w.r.t. to initial weight after drip is started

int dripRate(int dripFactor, float timeTo1gmChangeInSeconds, int prescribedIVAmountInmL, int prescribedDurationInMinutes)
{
	float dripRate = (prescribedIVAmountInmL / prescribedDurationInMinutes) * dripFactor;
	float actualRate = (dripFactor * 60) / timeTo1gmChangeInSeconds;
	float errorAllowed = 1.81150;
	if(actualRate < dripRate + errorAllowed && actualRate > dripRate - errorAllowed)
	{
		return 0;
	}
	else if (actualRate > dripRate + errorAllowed)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

double measureDripTime(float prevWeight)
{
	float currWeight = 498;
	auto start = high_resolution_clock::now();
	//function whose time is to be measured
	while(currWeight - prevWeight < 1);
	Sleep(30000);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	return (duration.count() / 1000000);
}

int salineInserted(float prevWeight, float currWeight)
{
	if(currWeight - prevWeight > 300)
	{
		//prompt to start drip
		//after starting call measureDripTime
		double timeTo1gmChangeInSeconds = measureDripTime(prevWeight);
		//Call dripRate
		// Fetch IV Drip parameters
		int correctionFactor = dripRate(20, timeTo1gmChangeInSeconds, 500, 240);
		if (correctionFactor == 0)
		{
			cout << "All Set!" << endl;
		}
		else if (correctionFactor == -1)
		{
			cout << "Decrease dripRate" << endl;
		}
		else {
			cout << "Increase dripRate" << endl;
		}
		return 0;
	}
	return -1;
}

int main()
{
	if (salineInserted(0.0, 501.0) == -1)
	{
		cout << "Error, IV not inserted" << endl;
	}
	return 0;
}