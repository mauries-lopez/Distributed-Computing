#pragma once

#include <vector>
#include <string>

class Config {
public:

	// User Input
	static unsigned int numDungeons; // n
	static unsigned int numTanks; // t
	static unsigned int numHealers; // h
	static unsigned int numDps; // d
	static unsigned int dungeonMinTime; // t1
	static unsigned int dungeonMaxTime; // t2

	// Functions
	static void getUserInput();
	static void initializeConfig();

	// Virtual Time
	static unsigned int virtualTime;

	// Progress Bar
	static std::vector<std::string> progressBarParties;
	static std::vector<std::string> progressBarDungeons;

private:
};