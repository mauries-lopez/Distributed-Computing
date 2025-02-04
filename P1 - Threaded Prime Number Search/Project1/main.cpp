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
#include <ctype.h> 

std::vector<int> Config::startRange;
std::vector<int> Config::endRange;
std::vector<int> Config::lookUpNumbers;

int Config::variant = 0;

SearchPrime primeSearcher;
std::vector<std::thread> childThreads;

// Helper function for validation test
void helperValidationTest(std::string type, std::string value) {

	// 'General' validation list:
	// 1. Check if one is missing. If atleast one missing, give error. Else, continue
	// 2. Check if config file is empty
	// 3. If all values are in maximum amount of int
	// 4. Duplicated entries (e.g. x 100 x 200)

	// 'x|y' validation list:
	// 1. Number only [/]
	// 2. No texts (e.g. no special characters) [/]
	// 3. Empty value [/]
	// 4. Negative values [/]
	// 5. Value range [?]
	// 6. Decimal [/]
	// 7. Multiple values (e.g. 'x 1 100') [?]

	if (type == "number") {
		std::string tempX = value;
		bool isPassedStringCheck = false;
		bool isPassedNumberCheck = false;
		while (isPassedStringCheck == false && isPassedNumberCheck == false) {
			// [First Check: String-related validation && Decimal (due to '.' character is checked) && Negative (due to '-' character is checked)]
			for (char c : tempX) {
				if (!std::isdigit(c)) {
					isPassedStringCheck = false;
					break;
				}
				else {
					isPassedStringCheck = true;
				}
			}
			if (isPassedStringCheck == false) {
				std::cout << "[ERROR: Characters are not allowed. Enter numerical values only.]" << std::endl;
				std::cout << "Enter 'x' value: ";
				std::cin >> tempX;
			}
			// [Second Check: Number-related validation]
			if (isPassedStringCheck == true && isPassedNumberCheck == false) {
				//Convert to integer value
				std::string tempInt = std::string(tempX);
				int tempXInt = std::stoi(tempInt);
				bool isPassedRangeCheck = false;
				// From 1 to y											// CONTINUE HERE!!!
				if (tempXInt >= 1 && tempXInt <= Config::y){
					isPassedRangeCheck = true;
					//Temporary
					isPassedNumberCheck = true;
					Config::x = tempXInt;
				}
				else {
					std::cout << "[ERROR: Invalid number. Enter number greater or equal to 1.]" << std::endl;
					std::cout << "Enter 'x' value: ";
					std::cin >> tempXInt;
				}
			}
		}
	}
}

void helperPrintThreadInfo() {
	// Set smallest start time and largest end time for each thread
	for (int i = 0; i < Config::x; i++) {
		// For each thread, get the proper timestamps
		if (!Config::threadStorage.at(i).startTime.empty() && !Config::threadStorage.at(i).endTime.empty()) {
			// https://www.geeksforgeeks.org/how-to-find-minimum-element-in-vector-in-cpp/
			// Find the smallest start time and assign it to .at(0)
			Config::threadStorage.at(i).startTime.at(0) = *std::min_element(Config::threadStorage.at(i).startTime.begin(), Config::threadStorage.at(i).startTime.end());
			// Find the largest end time and assign it to .at(0)
			Config::threadStorage.at(i).endTime.at(0) = *std::max_element(Config::threadStorage.at(i).endTime.begin(), Config::threadStorage.at(i).endTime.end());
		}
	}

	for (int i = 0; i < Config::x; i++) {
		if (!Config::threadStorage.at(i).primeNumbers.empty()) {
			std::cout
				<< "[Thread #" << Config::threadStorage.at(i).threadID << "] (From "
				<< Config::threadStorage.at(i).startTime.at(0)
				<< "ms"
				<< " to "
				<< Config::threadStorage.at(i).endTime.at(0)
				<< "ms)"
				<< std::endl;
			for (int b = 0; b < Config::threadStorage.at(i).primeNumbers.size(); b++) {
				std::cout << Config::threadStorage.at(i).primeNumbers.at(b) << " ";
			}

			//Next Thread
			std::cout << "\n\n";
			//std::cout << "\n" << Config::threadStorage.at(i).endTime.at(0) << "ms" << "\n" << std::endl;
		}
		else {
			std::cout
				<< "[Thread #" << Config::threadStorage.at(i).threadID << "] (From "
				<< Config::threadStorage.at(i).startTime.at(0)
				<< "ms"
				<< " to "
				<< Config::threadStorage.at(i).endTime.at(0)
				<< "ms)"
				<< std::endl;
			std::cout << "No stored prime numbers";
			//Next Thread
			std::cout << "\n\n";
		}
	}
}

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
			helperValidationTest("number", value);
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
		Config::startRange.push_back(1);
		for (int i = 0; i < Config::endRange.size(); i++) {
			Config::startRange.push_back(Config::endRange.at(i) + 1);
			//std::cout << Config::startRange.at(i) << std::endl;
		}
	}
	else {
		// End Ranges
		double firstEndRange = floor((Config::y / Config::x)); // Round down
		for (int i = 1; i <= Config::x; i++) {
			if (i != Config::x) {
				Config::endRange.push_back(firstEndRange * i);
			}
			else {
				Config::endRange.push_back((Config::y - (firstEndRange * Config::x)) + (firstEndRange * Config::x)); //Example: [246,492,738,984, (1234-1230) + 1230)]
			}
		}

		//Start Ranges
		int firstStartRange = 0;
		Config::startRange.push_back(1); // Start always on 1
		for (int i = 0; i < Config::endRange.size(); i++) {
			Config::startRange.push_back(Config::endRange.at(i) + 1);
		}
	}

}

void firstCombinationVariant() {

	std::cout << "[Current Setting: Variant #1]" << std::endl;
	std::cout << "[Print Variant: Print-Immediately]" << std::endl;
	std::cout << "[Task Division Scheme: Straight division of search range]" << std::endl;
	std::cout << "\nThe code pauses for 5 seconds." << std::endl;
	Sleep(5000);
	std::cout << "The code awakens!\n" << std::endl;

	// This waits Thread 0 to complete for Thread 1 to run and so on.
	// Also, all threads are created at the same time, it is just that it is not executed concurrently. This is becase of .join().
	for (int i = 0; i < Config::x; i++ ) {
		//Timestamp
		int timeCreated = primeSearcher.helperGetTime();
		std::thread thread_obj(&SearchPrime::splitsFindPrimeNumbers, &primeSearcher, i, timeCreated, Config::startRange.at(i), Config::endRange.at(i), "immediate");
		thread_obj.join();
	} 
}

void secondCombinationVariant() {

	std::cout << "[Current Setting: Variant #2]" << std::endl;
	std::cout << "[Print Variant: Wait-All-Threads]" << std::endl;
	std::cout << "[Task Division Scheme: Straight division of search range]" << std::endl;
	std::cout << "\nThe code pauses for 5 seconds." << std::endl;
	Sleep(5000);
	std::cout << "The code awakens!" << std::endl;
	std::cout << "Processing...\n" << std::endl;

	for (int i = 0; i < Config::x; i++) {
		//Timestamp
		int timeCreated = primeSearcher.helperGetTime();
		childThreads.emplace_back(std::thread(&SearchPrime::splitsFindPrimeNumbers, &primeSearcher, i, timeCreated, Config::startRange.at(i), Config::endRange.at(i), "wait"));
	}

	// Store Thread Information
	for (int i = 0; i < Config::x; i++) {
		// Create Object
		ThreadInfo threadInfo;

		// Thread ID
		threadInfo.threadID = i;

		// Create array of array of strings
		Config::threadStorage.push_back(threadInfo);
	}

	for (auto& th : childThreads) {
		th.join();
	}
	
	// Print Thread Information
	helperPrintThreadInfo();
}

void thirdCombinationVariant() {

	std::cout << "[Current Setting: Variant #3]" << std::endl;
	std::cout << "[Print Variant: Print-Immediately]" << std::endl;
	std::cout << "[Task Division Scheme: The search is linear but the threads are for divisibility testing of individual numbers.]" << std::endl;
	std::cout << "\nThe code pauses for 5 seconds." << std::endl;
	Sleep(5000);
	std::cout << "The code awakens!" << std::endl;

	// Create a look-up table
	for (int i = 1; i <= Config::y; i++) {
		Config::lookUpNumbers.push_back(i);
	}
	std::cout << "Look-up Tables: Initialized!\n" << std::endl;

	// Start Prime Test
	primeSearcher.divisibleTester("immediate");
}

void fourthCombinationVariant() {

	std::cout << "[Current Setting: Variant #4]" << std::endl;
	std::cout << "[Print Variant: Wait-All-Threads]" << std::endl;
	std::cout << "[Task Division Scheme: The search is linear but the threads are for divisibility testing of individual numbers.]" << std::endl;
	std::cout << "\nThe code pauses for 5 seconds." << std::endl;
	Sleep(5000);
	std::cout << "The code awakens!" << std::endl;

	// Create a look-up table
	for (int i = 1; i <= Config::y; i++) {
		Config::lookUpNumbers.push_back(i);
	}
	std::cout << "Look-up Tables: Initialized!\n" << std::endl;

	// Store Thread Information
	for (int i = 0; i < Config::x; i++) {
		// Create Object
		ThreadInfo threadInfo;

		// Thread ID
		threadInfo.threadID = i;

		// Create array of array of strings
		Config::threadStorage.push_back(threadInfo);
	}

	std::cout << "Processing...\n" << std::endl;

	// Start Prime Test
	primeSearcher.divisibleTester("wait");

	// Print Thread Information
	helperPrintThreadInfo();
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
		thirdCombinationVariant();
	}
	else if (Config::variant == 4) {
		fourthCombinationVariant();
	}
	else {
		std::cout << "Choose between [1 to 4] only." << std::endl;
	}

	return 0;
}