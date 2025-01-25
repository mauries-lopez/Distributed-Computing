#include "searchPrime.h"
#include "config.h"

#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>
#include <sstream>

int Config::x = 0;
int Config::y = 0;
std::counting_semaphore<1> Config::semaphore(1);

std::mutex Config::lookUpNumbersMutex;
std::mutex Config::printMutex;

// Problem [1]: Search for prime numbers to a given y number
// Search Prime Number Algorithm
void SearchPrime::splitsFindPrimeNumbers(int threadID, std::string timeStamp, int startRange, int endRange, std::string printVariant) {

	std::string primeNumbers;
	std::string threadStartInfo("\n[Thread #" + std::to_string(threadID) + "]" + "[STARTED: " + timeStamp + "]");
	// std::string threadEndInfo("[Thread #" + std::to_string(primeID) + "]" + "[ENDED: " + timeEnded + "]");

	if (printVariant == "immediate") {
		for (int a = startRange; a < endRange; a++) {

			if (a > 1) {
				int divisorCntr = 0;

				//Prime Conditions:
				//It can only have 2 divisors: '1' and by itself.
				for (int b = 1; b <= a; b++) {

					//Using modulo, we can find the FACTORS of a number
					//If the amount of factors a number has is greater than 2. Then, it is not a prime number.
					if (a % b == 0) {
						divisorCntr++;
					}
				}

				if (divisorCntr <= 2) {
					primeNumbers.append(std::to_string(a) + " ");

					time_t currTime;
					char timeCreation[50];
					struct tm datetime;
					time(&currTime);
					localtime_s(&datetime, &currTime);
					strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);
					std::string timeEnded = (std::string)timeCreation;
					std::string threadEndInfo("[ENDED: " + timeEnded + "]");

					std::cout << threadStartInfo << threadEndInfo << " -> (" << a << ") ";
				}
			}

		}

		//std::cout << threadStartInfo << std::endl;
		//std::cout << primeNumbers << std::endl;
		//std::cout << threadEndInfo << std::endl;
	}
	else if ( printVariant == "wait" ){

		std::string threadEndInfo;

		for (int a = startRange; a < endRange; a++) {

			if (a > 1) {
				int divisorCntr = 0;

				//Prime Conditions:
				//It can only have 2 divisors: '1' and by itself.
				for (int b = 1; b <= a; b++) {

					//Using modulo, we can find the FACTORS of a number
					//If the amount of factors a number has is greater than 2. Then, it is not a prime number.
					if (a % b == 0) {
						divisorCntr++;
					}
				}

				if (divisorCntr <= 2) {
					primeNumbers.append(std::to_string(a) + " ");

					time_t currTime;
					char timeCreation[50];
					struct tm datetime;
					time(&currTime);
					localtime_s(&datetime, &currTime);
					strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);
					std::string timeEnded = (std::string)timeCreation;
					threadEndInfo = "[ENDED: " + timeEnded + "]";
				}
			}
		} 

		Config& config = Config::getInstance();

		std::string threadAllInfo;

		threadAllInfo.append(threadStartInfo + "\n");
		threadAllInfo.append(primeNumbers + "\n");
		threadAllInfo.append(threadEndInfo);

		Config::printMutex.lock();
		config.printResult.push_back(threadAllInfo);
		Config::printMutex.unlock();
	}
	
}

void SearchPrime::divisibleTester(int threadID, std::string timeStamp, std::string printVariant) {


	Config& config = Config::getInstance();

	std::string threadStartInfo("\n[Thread #" + std::to_string(threadID) + "]" + "[STARTED: " + timeStamp + "]");
	std::string threadAllInfo;
	std::string threadEndInfo;
	std::string primeNumbers;

	// Algorithm:
	// 1. Remove the first number in lookUpNumbers
	// 2. Store it in a variable, determine if its prime or not
	// 3. If prime, store it to primeNumbers. Else, stay removed.
	// 4. If lookUpNumbers is empty, sort primeNumbers. (For wait-all-thread mode)

	threadAllInfo.append(threadStartInfo + "\n");

	int num = 0;

	while (!Config::lookUpNumbers.empty()) {

		// Mutex is needed here since it is possible that 2 or more threads can remove the same number in lookUpNumbers. 
		// We want mutex to restrict only 1 thread to modify it at a time.
		Config::lookUpNumbersMutex.lock();

		// Get the first number
		if (!Config::lookUpNumbers.empty()) { //For double check, these are threads. We don't know if all are still in-synch (it should be but... just making sure)
			num = Config::lookUpNumbers.at(0);
		}

		// Remove the first number
		if (!Config::lookUpNumbers.empty()) {
			Config::lookUpNumbers.erase(Config::lookUpNumbers.begin());
		}

		Config::lookUpNumbersMutex.unlock();

		// Determine the number if its a prime or not
		if (num > 1) {
			int divisorCntr = 0;
			for (int b = 1; b <= num; b++) {

				//Using modulo, we can find the FACTORS of a number
				//If the amount of factors a number has is greater than 2. Then, it is not a prime number.
				if (num % b == 0) {
					divisorCntr++;
				}
			}

			if (divisorCntr <= 2) {

				if (printVariant == "immediate") {
					time_t currTime;
					char timeCreation[50];
					struct tm datetime;
					time(&currTime);
					localtime_s(&datetime, &currTime);
					strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);
					std::string timeEnded = (std::string)timeCreation;
					std::string threadEndInfo("[ENDED: " + timeEnded + "]");

					Config::printMutex.lock();
					std::cout << threadStartInfo << threadEndInfo << " -> (" << num << ")";
					Config::printMutex.unlock();

					primeNumbers.append(std::to_string(num) + " ");
				}
				else {
					time_t currTime;
					char timeCreation[50];
					struct tm datetime;
					time(&currTime);
					localtime_s(&datetime, &currTime);
					strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);
					std::string timeEnded = (std::string)timeCreation;
					threadEndInfo = "[ENDED: " + timeEnded + "]";

					primeNumbers.append(std::to_string(num) + " ");
				}

			}
		}
	}

	threadAllInfo.append(primeNumbers + "\n");
	threadAllInfo.append(threadEndInfo);

	Config::printMutex.lock();
	config.printResult.push_back(threadAllInfo);
	Config::printMutex.unlock();


}
