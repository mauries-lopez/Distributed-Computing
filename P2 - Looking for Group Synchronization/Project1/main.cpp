#include "config.h"
#include "partySystem.h"

#include <string>
#include <sstream>
#include <iostream>

/*
	(1) [ALGORITHM for INITIALIZATION]: This can be seen in config.cpp
	1. Get the total number of players
	2. Each player will get a player id and a class
	3. These players are stored in a variable

	(2) [ALGORITHM for DUNGEON SYSTEM]
	1. Create a thread pool
	2. 

	[ALGORITHM for PARTY SYSTEM]: This can be seen in partySystem.cpp
	1. Every after creati
	ng a party consisting of 1 tank, 1 healer, and 3 dps. Directly put them in a dungeon

	[ALGORITHM for DUNGEON SYSTEM]: This can be seen inside a function in partySystem.cpp
	1. After party is created, put them in a dungeon
	2. Dungeon is created with 

*/

void viewDungeonStatus() {

};

int main() {

	Config::getUserInput();
	Config::initializeConfig();
	Party::buildParty();
	Party::deployParties();

	while (true) {
		std::string userInput;
		std::cin >> userInput;
	}

	//threadPool.stop();

	return 0;
}