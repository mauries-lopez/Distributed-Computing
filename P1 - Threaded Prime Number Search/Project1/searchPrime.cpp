#include "searchPrime.h"
#include "config.h"

#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <atomic>
#include <chrono>

int Config::x = 0;
int Config::y = 0;
std::vector<int> Config::lookUpNumbersFactors;
std::vector<ThreadInfo> Config::threadStorage;
std::mutex Config::lookUpNumbersMutex;
std::mutex Config::lookUpNumbersFactorsMutex;
std::mutex Config::factorCounterMutex;
std::mutex Config::printMutex;
std::mutex arrayPrimeNumbersLock;

// Atomic Variable
std::atomic<int> factorCounter(0);
std::atomic<bool> primeFound(false);

int SearchPrime::helperGetTime() {

	// Get the current time in milliseconds
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

	// Last 4 digits
	milliseconds = milliseconds % 1000000; 

	// Convert to string and append milliseconds
	int timeStamp = milliseconds;
	return timeStamp;
}

// Problem [1]: Search for prime numbers to a given y number
// Search Prime Number Algorithm
void SearchPrime::splitsFindPrimeNumbers(int threadID, int threadStartInfo, int startRange, int endRange, std::string printVariant) {

	std::vector<int> primeNumbers;
	int threadEndInfo = 0;

	if (printVariant == "wait") {
		Config::threadStorage.at(threadID).startTime.push_back(threadStartInfo);
	}

	for (int a = startRange; a <= endRange; a++) {
		if (a > 1) {
			int factorCounter = 0;
			//Prime Conditions:
			//It can only have 2 divisors: '1' and by itself.
			for (int b = 1; b <= a; b++) {
				//Using modulo, we can find the FACTORS of a number
				//If the amount of factors a number has is greater than 2. Then, it is not a prime number.
				if (a % b == 0) {
					factorCounter++;
				}
			}
			if (factorCounter <= 2) { 
				primeNumbers.push_back(a);
				//std::string threadEndInfo = helperGetTime("end", threadID);
				if (printVariant == "immediate") {
					threadEndInfo = helperGetTime();
					std::cout
						<< "[Thread #" << threadID << "] (From "
						<< threadStartInfo
						<< "ms"
						<< " to "
						<< threadEndInfo
						<< "ms): ["
						<< a
						<< "]"
						<< std::endl;
				}
			}
		}
		else {
			// If statement is needed because if it does not include this, it will print immediately on wait-for-all threads mode.
			if (printVariant == "immediate" && a < 1) {
				threadEndInfo = helperGetTime();
				std::cout
					<< "[Thread #" << threadID << "] (From "
					<< threadStartInfo
					<< "ms"
					<< " to "
					<< threadEndInfo
					<< "ms): ["
					<< "No prime numbers have been assigned in the thread"
					<< "]"
					<< std::endl;
			}
		}
	}
	//For wait-for-all print, wait for the thread to finish and store all prime numbers it found in its respective thread information.
	if (printVariant == "wait") {
		for (int i = 0; i < primeNumbers.size(); i++) {
			Config::threadStorage.at(threadID).primeNumbers.push_back(primeNumbers.at(i));
		}
		threadEndInfo = helperGetTime();
		Config::threadStorage.at(threadID).endTime.push_back(threadEndInfo);
	}
}

void SearchPrime::divisibleTester(std::string printVariant) {

	// [Algorithm:]
	// 1. Remove the first number in lookUpNumbers
	// 2. Store it in a variable (num)
	// 3. use num in a thread where it will be determined if its prime or not
	// 4. in separate threads, each has its own number to be used to test num if its prime or not. The numbers came from lookUpNumbersFactors (1 to y)
	// 5. If its divisible, it will iterate factorCounter. This way, I can count how many factors does the num has. (Note: the thread is in a while loop so it will use 1 2 3 4 5 6 ... to test on 'num')

	// [ATTENTION!!! This algorithm is VERY SLOW because of few reasons. ATTENTION!!!]
	//	- the lookUpNumbersFactors needs to be filled every each number
	// For example: Test '10'(num) if its prime with 3 threads.
	// [1] lookUpNumbersFactors will be initialized from numbers 1 to (num) FOR EVERY UNIQUE (num).
	// [2] the algorithm will use the look-up table to get one number at a time per thread to be tested on the given number.
	// Thread #0: gets '1' from lookUpNumbersFactors and test it on '10'
	// Thread #1: gets '2' from lookUpNumbersFactors and test it on '10'
	// Thread #2: gets '3' from lookUpNumbersFactors and test it on '10'
	// 
	//	- A lot of thread synchronization (e.g. mutex)

	// [WEAKNESS OF THE ALGORITHM]
	// Since the threads are created "at the same time" or 0.0000001... difference, all of these treads checks their respective number to (num) if its divisible at the "at the same time".
	// In result, even 2 threads have found that (num) is prime, other threads will continue checking/test.
	// To mitigate this, line 188 is implemented as one of the base cases of the while loop.. Assuming it can stop other threads that are yet to be created.

	int num = 0;
	SearchPrime primeSearcher;

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

		for (int i = 1; i <= num; i++) {
			Config::lookUpNumbersFactors.push_back(i);
		}
		
		// Make sure primeFound bool starts false every new number
		primeFound.store(false);

		std::vector<std::thread> childThreads;

		// Determine the number if its a prime or not
		for (int i = 0; i < Config::x; i++) {
			// Each thread will have its own information (e.g. thread ID, start time, end time, prime numbers)
			if (printVariant == "wait") {
				Config::threadStorage.at(i).startTime.push_back(helperGetTime());
			}
			childThreads.emplace_back(std::thread(&SearchPrime::testForDivisible, &primeSearcher, i, num, printVariant));
		}

		for (int i = 0; i < Config::x; i++) { 
			childThreads.at(i).join();
		}

		Config::lookUpNumbersFactors.clear();
	}
}

void SearchPrime::testForDivisible(int threadID, int num, std::string printVariant) {

	int factor = 0;
	std::string primeNumbers;
	//std::string threadStartInfo = helperGetTime("start", threadID);
	int threadStartInfo = helperGetTime();
	std::string threadAllInfo;

	if (num > 1) {
		while (true) {

			if (primeFound.load()) {
				break; // Exit if a prime has been found
			}

			Config::lookUpNumbersFactorsMutex.lock();
			if (Config::lookUpNumbersFactors.empty()) {
				Config::lookUpNumbersFactorsMutex.unlock();
				break; // So only one thread can manipulate lookUpNumbersFactorsMutex at a time until its finish determining the number of factors.
			}
			factor = Config::lookUpNumbersFactors.at(0);
			Config::lookUpNumbersFactors.erase(Config::lookUpNumbersFactors.begin());
			Config::lookUpNumbersFactorsMutex.unlock();

			if (num % factor == 0) {
				factorCounter.fetch_add(1);
			}
		}
	}

	// Check if the number is prime
	Config::printMutex.lock();
	if (factorCounter.load() == 2 && factorCounter.load() != 0 ) {
		primeFound.store(true);
		if (printVariant == "immediate") {
			int threadEndInfo = helperGetTime();
			// Upon observation, usually the first thread who have started the divisiblity testing on the number prints the output.
			// The way how the algorithm works, it might be because the first threads always gets the lowest number to try on 'num'.
			// For example: Test '3' if its prime. 3 threads
			// [Thread #0] will test 1 on 3 <-- Will finish first because it is easier to 3 modulo 1... This would be more obvious in larger numbers...
			// [Thread #1] will test 2 on 3 
			// [Thread #2] will test 3 on 3
			// Note: Though, the algorithm that I created works like that, it still dependent on how fast the CPU cores are being used.
			// I also observed that when running the code for variants 3-4, I have high CPU usage.
			std::cout
				<< "[Thread #" << threadID << "] (From "
				<< threadStartInfo
				<< "ms"
				<< " to "
				<< threadEndInfo
				<< "ms): ["
				<< num 
				<< "]"
				<< std::endl;
		} else if ( printVariant == "wait") {
			arrayPrimeNumbersLock.lock();
			Config::threadStorage.at(threadID).primeNumbers.push_back(num);
			arrayPrimeNumbersLock.unlock();
			int threadEndInfo = helperGetTime();
			Config::threadStorage.at(threadID).endTime.push_back(threadEndInfo);
		}
	}
	else {
		// This is necessary for printing after all threads have finished, as it requires information from each thread to determine when it was last executed.
		if (printVariant == "wait") {
			int threadEndInfo = helperGetTime();
			Config::threadStorage.at(threadID).endTime.push_back(threadEndInfo);
		}
	}
	Config::printMutex.unlock();

	// RESET to get ready for the new number
	factorCounter.store(0);
}