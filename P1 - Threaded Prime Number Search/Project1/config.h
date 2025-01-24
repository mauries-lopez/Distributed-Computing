#pragma once
#include <vector>
#include <string>
#include <mutex>

class Config {
public:
	static int x;
	static int y;
	static int range;

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