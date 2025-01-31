#pragma once

#include <string>

class SearchPrime {

public:
	void splitsFindPrimeNumbers(int threadID, std::string timeStamp, int startRange, int endRange, std::string printVariant);
	void divisibleTester(std::string printVariant);
	void testForDivisible(int threadID, int num);

private:

	//int factorCounter = 0;

};