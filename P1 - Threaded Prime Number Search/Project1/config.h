#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <semaphore> //https://www.geeksforgeeks.org/cpp-20-semaphore-header/

struct ThreadInfo {
	int threadID;
	std::string startTime;
	std::vector<int> primeNumbers;  // Vector to store prime numbers
	std::string endTime;
};

class Config {
public:

	// I/O Configuration
	static int x;
	static int y;
	static int variant;

	// Variable Configuration
	static std::vector<int> lookUpNumbers;
	static std::vector<int> lookUpNumbersFactors;
	static std::vector<int> startRange;
	static std::vector<int> endRange;
	static std::vector<ThreadInfo> threadStorage;
	std::vector<std::string> printResult;

	static std::mutex lookUpNumbersMutex;
	static std::mutex lookUpNumbersFactorsMutex;
	static std::mutex factorCounterMutex;
	static std::mutex printMutex;

	static Config& getInstance() {
		static Config instance;
		return instance;
	}

private:
	Config() {}
};