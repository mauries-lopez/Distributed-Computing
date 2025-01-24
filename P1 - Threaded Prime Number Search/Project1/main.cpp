#include "config.h"
#include "searchPrime.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono> 
#include <ctime> 
#include <windows.h>

std::vector<int> Config::startRange;
std::vector<int> Config::endRange;

int Config::variant = 0;

SearchPrime primeSearcher;
std::vector<std::thread> childThreads;

// Problem [2]: The values x and y should be configurable in a separate config file. x: number of threads, y: number to use
// I/O File
void getConfigValues() {

	//Read Config.txt
	std::ifstream input("config.txt");
	std::string parameter, value;

	while (std::getline(input, parameter)) {
		std::istringstream stream(parameter);
		stream >> parameter >> value;

		if (parameter == "x") {
			Config::x = std::stoi(value);
			//std::cout << config.x << std::endl;
		}
		else if (parameter == "y") {
			Config::y = std::stoi(value);
			//std::cout << config.y << std::endl;
		}
		else if (parameter == "variant") {
			Config::variant = std::stoi(value);;
		}
	}

	input.close();
}

// Problem[3]: Determine the range of each thread to search
void determineRange() {
	
	//Assume: Y can be divided fairly using X (e.g. 1000 / 4 is 250 each thread). Division of ranges did not result to decimals.
	if (Config::y % Config::x == 0) { //No Decimal (if answer is 0, no decimal. else, with decimal)
		// End Ranges
		int firstEndRange = (Config::y / Config::x);
		for (int i = 1; i <= Config::x; i++) {
			Config::endRange.push_back(firstEndRange * i);
			//std::cout << Config::endRange.at(i-1) << std::endl;
		}

		//Start Ranges
		int firstStartRange = 0;
		Config::startRange.push_back(0);
		for (int i = 0; i < Config::endRange.size(); i++) {
			Config::startRange.push_back(Config::endRange.at(i) + 1);
			//std::cout << Config::startRange.at(i) << std::endl;
		}
	}
	else {
		// End Ranges
		int firstEndRange = floor((Config::y / Config::x)); // Round down
		for (int i = 1; i <= Config::x; i++) {
			if (i != Config::x) {
				Config::endRange.push_back(firstEndRange * i);
			}
			else {
				Config::endRange.push_back((Config::y - (firstEndRange * Config::x)) + (firstEndRange * Config::x)); //Example: [246,492,738,984, (1234-1230) + 1230)]
			}
			//std::cout << Config::endRange.at(i-1) << std::endl;
		}

		//Start Ranges
		int firstStartRange = 0;
		Config::startRange.push_back(0);
		for (int i = 0; i < Config::endRange.size(); i++) {
			Config::startRange.push_back(Config::endRange.at(i) + 1);
			//std::cout << Config::startRange.at(i) << std::endl;
		}
	}

}

void firstCombinationVariant() {

	// Current Implementation:
	// Printing Variant: Print Immediately
	// Task Division Scheme: Straight division of search range.

	std::cout << "[Current Setting: Variant #1]" << std::endl;
	std::cout << "[Print Variant: Print-Immediately]" << std::endl;
	std::cout << "[Task Division Scheme: Straight division of search range]" << std::endl;
	std::cout << "\nThe code pauses for 5 seconds." << std::endl;
	Sleep(5000);
	std::cout << "The code awakens!" << std::endl;

	//Timestamp
	time_t currTime;
	char timeCreation[50];
	struct tm datetime;
	time(&currTime);
	localtime_s(&datetime, &currTime);
	strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);
	std::string timeCreated = (std::string)timeCreation;

	// This waits Thread 0 to complete for Thread 1 to run and so on.
	// Also, all threads are created at the same time, it is just that it is not executed concurrently. This is becase of .join().
	for (int i = 0; i < Config::x; i++ ) {
		std::thread thread_obj(&SearchPrime::findPrimeNumbers, &primeSearcher, i, timeCreated, Config::startRange.at(i), Config::endRange.at(i), "immediate");
		thread_obj.join();
		//childThreads.push_back(std::move(thread_obj));
	}

	// Problem encountered on why .detach() doesnt work: main thread is exiting before the child threads finishes.
	// Solution: Uncomment line 106, 111, and 112
	// Problem encountered: Hindi mabasa ung started at ended.
	// Solution: Store the strings into one variable then print after all threads finishes. <--- Do this.
	//for (int i = 0; i < Config::x; i++) { 
	//	childThreads.at(i).join();
	//}
}

void secondCombinationVariant() {

	// Current Implementation:
	// Printing Variant: Wait-All-Threads
	// Task Division Scheme: Straight division of search range.
	
	std::cout << "[Current Setting: Variant #2]" << std::endl;
	std::cout << "[Print Variant: Wait-All-Threads]" << std::endl;
	std::cout << "[Task Division Scheme: Straight division of search range]" << std::endl;
	std::cout << "\nThe code pauses for 5 seconds." << std::endl;
	Sleep(5000);
	std::cout << "The code awakens!" << std::endl;

	//Timestamp
	time_t currTime;
	char timeCreation[50];
	struct tm datetime;
	time(&currTime);
	localtime_s(&datetime, &currTime);
	strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);
	std::string timeCreated = (std::string)timeCreation;

	for (int i = 0; i < Config::x; i++) {
		childThreads.emplace_back(std::thread(&SearchPrime::findPrimeNumbers, &primeSearcher, i, timeCreated, Config::startRange.at(i), Config::endRange.at(i), "wait"));
	}

	for (auto& th : childThreads) {
		th.join();
	}

	Config& config = Config::getInstance();

	std::cout << "\nAll threads finished!!" << std::endl;

	for (int i = 0; i < config.printResult.size(); i++) {
		std::cout << config.printResult.at(i) << std::endl;
	}
	
}

int main() {

	//Initialize
	getConfigValues();
	determineRange();

	if (Config::variant == 1) {
		firstCombinationVariant();
	}
	else if (Config::variant == 2) {
		secondCombinationVariant();
	}
	else if (Config::variant == 3) {
		std::cout << "Not Implemented Yet!" << std::endl;
	}
	else if (Config::variant == 4) {
		std::cout << "Not Implemented Yet!" << std::endl;
	}
	else {
		std::cout << "Choose between [1 to 4] only." << std::endl;
	}

	return 0;
}