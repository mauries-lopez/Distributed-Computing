#include "config.h"
#include "partySystem.h"
#include "dungeonConfig.h"

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

void startGame() {
	Party::buildParty();
}

void viewStatus() {

	int nFinished = 0;

	while (true) {

		// Clear the console to "refresh" the view
		system("clear"); // For Linux/Unix, or use "cls" for Windows

		// Player size for each class
		std::cout << "------------- Player Count without Party -------------" << std::endl;
		std::cout << "Tank Player List Size: " << Player::tankPlayerList.size() << std::endl;
		std::cout << "Healer Player List Size: " << Player::healerPlayerList.size() << std::endl;
		std::cout << "DPS Player List Size: " << Player::dpsPlayerList.size() << std::endl;
		std::cout << "------------------------------------------------------" << std::endl;


		// Print dungeon status
		std::cout << "------------------- Dungeon Status -------------------" << std::endl;
		for (int i = 0; i < Dungeon::dungeonList.size(); i++) {
			std::cout << "[DungeonID: " << Dungeon::dungeonList.at(i).dungeonId << "] - [" << Dungeon::dungeonList.at(i).status << "] " << std::endl;
			std::cout << "[Party Served: " << Dungeon::dungeonList.at(i).numServed << "]" << std::endl;
			std::cout << "[Total Time Served: " << Dungeon::dungeonList.at(i).totalTimeServed << "s] " << std::endl;
			for (int x = 0; x < Party::partyList.size(); x++) {
				if (Party::partyList.at(x).dungeonID == Dungeon::dungeonList.at(i).dungeonId) {
					if (Party::partyList.at(x).status == "active") {
						std::cout << "[PartyID: " << Party::partyList.at(x).partyID << "] ";
						std::cout << "[Time: " << Dungeon::dungeonList.at(i).curDungeonTime << "/" << Party::partyList.at(x).timeInDungeon << "s]" << std::endl;
						break;
					}
				}
			}
			std::cout << "------------------------------------------------------" << std::endl;
		}

		std::cout << "\n";

		// Print Party in Queue
		std::cout << "-------------------- Party Status --------------------" << std::endl;
		for (int i = 0; i < Party::partyList.size(); i++) {
			if (Party::partyList.at(i).status == "active" || Party::partyList.at(i).status == "queued")  {
				std::cout << "[PartyID: " << Party::partyList.at(i).partyID << "] - Finished Dungeon?: " << Party::partyList.at(i).status << std::endl;
			}
		}
		std::cout << "------------------------------------------------------" << std::endl;

		std::cout << "\n";

		// Print Party Finished Dungeon
		std::cout << "--------------- Party Finished Dungeon ---------------" << std::endl;
		for (int i = 0; i < Party::partyList.size(); i++) {
			if (Party::partyList.at(i).status == "finished") {
				std::cout << "[PartyID: " << Party::partyList.at(i).partyID << "] - Finished Dungeon?: " << Party::partyList.at(i).status << std::endl;
				// Count Finished
			}
		}
		std::cout << "------------------------------------------------------" << std::endl;

		// Sleep for a set duration before refreshing
		std::this_thread::sleep_for(std::chrono::milliseconds(800)); 
	}

	system("exit");
}

int main() {

	Config::getUserInput();
	Config::initializeConfig();

	std::thread startGameThread(&startGame);
	startGameThread.detach();

	viewStatus();

	return 0;
}