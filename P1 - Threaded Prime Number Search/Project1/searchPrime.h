#pragma once

#include <string>

class SearchPrime {

public:
	void splitsFindPrimeNumbers(int threadID, int timeStamp, int startRange, int endRange, std::string printVariant);
	void divisibleTester(std::string printVariant);
	void testForDivisible(int threadID, int num, std::string printVariant);
	int helperGetTime();
private:

};