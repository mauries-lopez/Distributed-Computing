#include "searchPrime.h"
#include "config.h"
#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>
#include <sstream>

int Config::x = 0;
int Config::y = 0;

// Problem [1]: Search for prime numbers to a given y number
// Search Prime Number Algorithm
void SearchPrime::findPrimeNumbers(int primeID, std::string timeStamp, int startRange, int endRange, std::string printVariant) {

	time_t currTime;
	char timeCreation[50];
	struct tm datetime;
	time(&currTime);
	localtime_s(&datetime, &currTime);
	strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);
	std::string timeEnded = (std::string)timeCreation;

	std::string primeNumbers;
	std::string threadStartInfo("\n[Thread #" + std::to_string(primeID) + "]" + "[STARTED: " + timeStamp + "]");
	// std::string threadEndInfo("[Thread #" + std::to_string(primeID) + "]" + "[ENDED: " + timeEnded + "]");
	std::string threadEndInfo("[ENDED: " + timeEnded + "]");

	if (printVariant == "immediate") {
		for (int a = startRange; a < endRange; a++) {

			if (a > 1) {
				int divisorCntr = 0;

				//Prime Conditions:
				//It can only have 2 divisors: '1' and by itself.
				for (int b = 1; b <= a; b++) {
					if (a % b == 0) {
						divisorCntr++;
					}
				}

				if (divisorCntr <= 2) {
					primeNumbers.append(std::to_string(a) + " ");
					std::cout << threadStartInfo << threadEndInfo << " -> (" << a << ") ";
				}
			}

		}

		//std::cout << threadStartInfo << std::endl;
		//std::cout << primeNumbers << std::endl;
		//std::cout << threadEndInfo << std::endl;
	}
	else if ( printVariant == "wait" ){

		for (int a = startRange; a < endRange; a++) {

			if (a > 1) {
				int divisorCntr = 0;

				//Prime Conditions:
				//It can only have 2 divisors: '1' and by itself.
				for (int b = 1; b <= a; b++) {
					if (a % b == 0) {
						divisorCntr++;
					}
				}

				if (divisorCntr <= 2) {
					primeNumbers.append(std::to_string(a) + " ");
				}
			}
		} 

		Config& config = Config::getInstance();

		std::string threadAllInfo;

		threadAllInfo.append(threadStartInfo + "\n");
		threadAllInfo.append(primeNumbers + "\n");
		threadAllInfo.append(threadEndInfo);

		config.printResult.push_back(threadAllInfo);
	}
	
}
