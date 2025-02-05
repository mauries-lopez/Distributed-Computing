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
void helperValidationTest(std::string type, std::string parameter, std::string value) {

	// 'x|y|variant' validation list:
	// - Number only [/]
	// - No texts (e.g. no special characters) [/]
	// - Empty value [/]
	// - Negative values [/]
	// - Value range [/]
	// - Decimal [/]

	if (type == "number") {
		std::string tempVal = value;
		bool isPassedStringCheck = false;
		bool isPassedNumberCheck = false;
		while (isPassedStringCheck == false && isPassedNumberCheck == false) {
			// [First Check: String-related validation && Decimal (due to '.' character is checked) && Negative (due to '-' character is checked)]
			for (char c : tempVal) {
				if (!std::isdigit(c)) {
					isPassedStringCheck = false;
					break;
				}
				else {
					isPassedStringCheck = true;
				}
			}
			if (isPassedStringCheck == false) {
				if (parameter == "x") {
					std::cout << "[ERROR: Characters (e.g. '-', '.' are included) are not allowed in 'x'. Enter numerical values only.]" << std::endl;
					std::cout << "Enter 'x' value: ";
				}
				else if (parameter == "y"){
					std::cout << "[ERROR: Characters (e.g. '-', '.' are included) are not allowed in 'y'. Enter numerical values only.]" << std::endl;
					std::cout << "Enter 'y' value: ";
				}
				else if (parameter == "variant") {
					std::cout << "[ERROR: Characters (e.g. '-', '.' are included) are not allowed in 'variant'. Enter numerical values only.]" << std::endl;
					std::cout << "Enter 'variant' value: ";
				}
				std::cin >> tempVal;
			}
			// [Second Check: Number-related validation]
			if (isPassedStringCheck == true && isPassedNumberCheck == false) {
				//Convert to integer value
				std::string tempInt = std::string(tempVal);
				int tempValInt = std::stoi(tempInt);
				bool isPassedRangeCheck = false;
				if (tempValInt >= 1 && tempValInt <= INT_MAX){
					if (parameter == "x") {
						isPassedRangeCheck = true;
						isPassedNumberCheck = true;
						Config::x = tempValInt;
					}
					else if (parameter == "y" && tempValInt > 1) {
						isPassedRangeCheck = true;
						isPassedNumberCheck = true;
						//isPassedAllChecksForY = true;
						Config::y = tempValInt;
					}
					else if (parameter == "variant") {
						if (tempValInt >= 1 && tempValInt <= 4) {
							isPassedRangeCheck = true;
							isPassedNumberCheck = true;
							Config::variant = tempValInt;
						}
						else {
							//Temporary tempValString
							std::string tempValString;
							std::cout << "[ERROR: Invalid 'variant' value. Enter a number between 1 and 4 only.]" << std::endl;
							std::cout << "Enter 'variant' value: ";
							std::cin >> tempValString;
							
							//Reset all booleans to check all validations again
							isPassedStringCheck = false;
							tempVal = tempValString; //Return to string validation
						}
					}
					else if (parameter == "y" && tempValInt <= 1) {
						//Temporary tempValString
						std::string tempValString;
						//Reset all booleans to check all validations again
						std::cout << "[ERROR: Invalid 'y' value. Enter number greater than 1.]" << std::endl;
						std::cout << "Enter 'y' value: ";
						std::cin >> tempValString;
						isPassedStringCheck = false;
						tempVal = tempValString; //Return to string validation
					}
				}
				else {
					//Temporary tempValString
					std::string tempValString;
					if (parameter == "x") {
						std::cout << "[ERROR: Invalid 'x' value. Enter number greater or equal to 1.]" << std::endl;
						std::cout << "Enter 'x' value: ";
					}
					else if (parameter == "y"){
						std::cout << "[ERROR: Invalid 'y' value. Enter number greater than 1.]" << std::endl;
						std::cout << "Enter 'y' value: ";
					}
					else if (parameter == "variant") {
						std::cout << "[ERROR: Invalid 'variant' value. Enter a number between 1 and 4 only.]" << std::endl;
						std::cout << "Enter 'variant' value: ";
					}
					std::cin >> tempValString;
					//Reset all booleans to check all validations again
					isPassedStringCheck = false;
					tempVal = tempValString; //Return to string validation
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
			std::cout << "\n\n";		}
		else {
			std::cout
				<< "[Thread #" << Config::threadStorage.at(i).threadID << "] (From "
				<< Config::threadStorage.at(i).startTime.at(0)
				<< "ms"
				<< " to "
				<< Config::threadStorage.at(i).endTime.at(0)
				<< "ms)"
				<< std::endl;
			std::cout << "No prime numbers have been assigned in the thread";
			//Next Thread
			std::cout << "\n\n";
		}
	}
}

// Problem [2]: The values x and y should be configurable in a separate config file. x: number of threads, y: number to use
// I/O File
bool getConfigValues() {

	// 'General' validation list:
	// - Check if one is missing. If atleast one missing, give error. Else, continue [/]
	// - Check if config file is empty [/]
	// - Duplicated entries (e.g. x 100 x 200) [/]
	// - 'x 1' 'y 1' [/]
	// - config.txt does not exists [/]

	//Bool for each parameter
	bool paramX = false;
	bool paramY = false;
	bool paramVar = false;
	bool isAllFilled = false;

	std::string parameter, value;

	//Read Config.txt
	std::ifstream input("config.txt");
	if (input.is_open()) {
		while (std::getline(input, parameter)) {
			std::istringstream stream(parameter);
			stream >> parameter >> value;
			if (parameter == "x" && paramX == false) {
				helperValidationTest("number", parameter, value);
				paramX = true;
			}
			else if (parameter == "y" && paramY == false) {
				helperValidationTest("number", parameter, value);
				paramY = true;
			}
			else if (parameter == "variant" && paramVar == false) {
				helperValidationTest("number", parameter, value);
				paramVar = true;
			}
		}

		// Check if all parameters are filled
		if (paramX == true && paramY == true && paramVar == true) {
			isAllFilled = true;
		}

		// If one of the parameters is missing.
		while (isAllFilled == false) {
			std::string tempValString;
			if (paramX == false) {
				std::cout << "[ERROR: 'x' parameter does not exists.]" << std::endl;
				std::cout << "Enter 'x' value: ";
				std::cin >> tempValString;
				helperValidationTest("number", "x", tempValString);
				paramX = true;
			}
			else if (paramY == false) {
				std::cout << "[ERROR: 'y' parameter does not exists.]" << std::endl;
				std::cout << "Enter 'y' value: ";
				std::cin >> tempValString;
				helperValidationTest("number", "y", tempValString);
				paramY = true;
			}
			else if (paramVar == false) {
				std::cout << "[ERROR: 'variant' parameter does not exists.]" << std::endl;
				std::cout << "Enter 'variant' value: ";
				std::cin >> tempValString;
				helperValidationTest("number", "variant", tempValString);
				paramVar = true;
			}
			else {
				isAllFilled = true;
			}
		}
		input.close();
		return true;
	}
	else {
		std::cout << "[ERROR: config.txt does not exists. Create a config.txt file.]" << std::endl;
		return false;
	}
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
	std::cout << "[x: " << Config::x << "][y: " << Config::y << "][variant: " << Config::variant << "]\n" << std::endl;

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
	std::cout << "[x: " << Config::x << "][y: " << Config::y << "][variant: " << Config::variant << "]\n" << std::endl;

	// Store Thread Information
	for (int i = 0; i < Config::x; i++) {
		// Create Object
		ThreadInfo threadInfo;

		// Thread ID
		threadInfo.threadID = i;

		// Create array of array of strings
		Config::threadStorage.push_back(threadInfo);
	}

	for (int i = 0; i < Config::x; i++) {
		//Timestamp
		int timeCreated = primeSearcher.helperGetTime();
		childThreads.emplace_back(std::thread(&SearchPrime::splitsFindPrimeNumbers, &primeSearcher, i, timeCreated, Config::startRange.at(i), Config::endRange.at(i), "wait"));
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
	std::cout << "[x: " << Config::x << "][y: " << Config::y << "][variant: " << Config::variant << "]\n" << std::endl;

	// Create a look-up table
	for (int i = 1; i <= Config::y; i++) {
		Config::lookUpNumbers.push_back(i);
	}

	// Start Prime Test
	primeSearcher.divisibleTester("immediate");
}

void fourthCombinationVariant() {

	std::cout << "[Current Setting: Variant #4]" << std::endl;
	std::cout << "[Print Variant: Wait-All-Threads]" << std::endl;
	std::cout << "[Task Division Scheme: The search is linear but the threads are for divisibility testing of individual numbers.]" << std::endl;
	std::cout << "[x: " << Config::x << "][y: " << Config::y << "][variant: " << Config::variant << "]\n" << std::endl;

	// Create a look-up table
	for (int i = 1; i <= Config::y; i++) {
		Config::lookUpNumbers.push_back(i);
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

	// Start Prime Test
	primeSearcher.divisibleTester("wait");

	// Print Thread Information
	helperPrintThreadInfo();
}

int main() {

	int timeStart = primeSearcher.helperGetTime();
	std::cout << "[System] Time Started (" << timeStart << "ms)" << std::endl;

	bool isConfigPrepared = getConfigValues();
	if (isConfigPrepared == true) {
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
			std::cout << "Choose between variants 1 to 4 only." << std::endl;
		}
	}

	int timeEnded = primeSearcher.helperGetTime();
	std::cout << "\n[System] Time Ended (" << timeEnded << "ms)" << std::endl;

	int overallMs = timeEnded - timeStart;
	std::cout << "[System] Time Diff. (" << overallMs << "ms)" << std::endl;

	return 0;
}