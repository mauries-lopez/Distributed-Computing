#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <semaphore> //https://www.geeksforgeeks.org/cpp-20-semaphore-header/

class Config {
public:
	static int x;
	static int y;
	static int variant;

	static std::vector<int> lookUpNumbers;
	static std::counting_semaphore<1> semaphore;  // Semaphore to control access to removing numbers
	static std::mutex lookUpNumbersMutex;
	static std::mutex printMutex;

	static std::vector<int> startRange;
	static std::vector<int> endRange;

	static Config& getInstance() {
		static Config instance;
		return instance;
	}

	std::vector<std::string> printResult;
	std::mutex mtx;

private:
	Config() {}
};