#pragma once

#include <vector>
#include <string>

class Config {
public:

	// User Input
	static int numDungeons; // n
	static int numTanks; // t
	static int numHealers; // h
	static int numDps; // d
	static int dungeonMinTime; // t1
	static int dungeonMaxTime; // t2

	// Functions
	static void getUserInput();
	static void initializeConfig();

	// Virtual Time
	static int virtualTime;

private:
};