#include "config.h"
#include "partySystem.h"
#include "dungeonConfig.h"
#include "threadPool.h"

#include <string>
#include <sstream>
#include <iostream>

static void startGame() {
	Party::buildParty();
}

static void viewStatus() {

	unsigned int nFinished = 0;
	bool isLoopAllowedToRun = false;

	//Consistently check for signal
	while (true) {
		//[1] All party is created && [2] All dungeons(threads) are created
		if (Party::isAllPartyCreated == true && ThreadPool::isAllDungeonCreated == true) { 
			isLoopAllowedToRun = true;
			break;
		}
	}

	if (isLoopAllowedToRun == true) {
		while (true) {

			// Clear the console to "refresh" the view
			system("clear"); // For Linux/Unix, or use "cls" for Windows

			// Print dungeon status
			std::cout << "------------------- Dungeon Status -------------------" << std::endl;
			for (unsigned int i = 0; i < Dungeon::dungeonList.size(); i++) {
				std::cout << "[DungeonID: " << Dungeon::dungeonList.at(i).dungeonId << "] - [" << Dungeon::dungeonList.at(i).status << "] " << std::endl;
				std::cout << "[Party Served: " << Dungeon::dungeonList.at(i).numServed << "]" << std::endl;
				std::cout << "[Total Time Served: " << Dungeon::dungeonList.at(i).totalTimeServed << "s] " << std::endl;
				for (unsigned int x = 0; x < Party::partyList.size(); x++) {
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

			// Print Party in Queue
			unsigned int activePartyCount = 0;
			unsigned int queuedPartyCount = 0;
			std::cout << "-------------------- Party Status --------------------" << std::endl;
			for (unsigned int i = 0; i < Party::partyList.size(); i++) {
				if (Party::partyList.at(i).status == "active") {
					activePartyCount++;
					//std::cout << "[PartyID: " << Party::partyList.at(i).partyID << "] - Finished Dungeon?: " << Party::partyList.at(i).status << std::endl;
				}
				else if (Party::partyList.at(i).status == "queued") {
					queuedPartyCount++;
				}
			}
			std::cout << "[Active Parties: " << activePartyCount << "]" << std::endl;
			std::cout << "[Queued Parties: " << queuedPartyCount << "]" << std::endl;

			// Print Party Finished Dungeon
			unsigned int finishedPartyCount = 0;
			for (unsigned int i = 0; i < Party::partyList.size(); i++) {
				if (Party::partyList.at(i).status == "finished") {
					finishedPartyCount++;
					//std::cout << "[PartyID: " << Party::partyList.at(i).partyID << "] - Finished Dungeon?: " << Party::partyList.at(i).status << std::endl;
				}
			}
			std::cout << "[Finished Parties: " << finishedPartyCount << "/" << Party::partyList.size() << "]" << std::endl;
			std::cout << "------------------------------------------------------" << std::endl;

			// Stop Refreshing
			// Since the party mechanics is in a separate thread, it is crucial to check if players have created parties already before viewing the status
			if (finishedPartyCount == Party::partyList.size()) {
				break;
			}

			// Sleep for a set duration before refreshing
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
		}
	}

	// Sleep again to make sure everything is prepared
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	// Clear the console to "refresh" the view
	system("clear"); // For Linux/Unix, or use "cls" for Windows

	// Print dungeon status
	std::cout << "------------------- Dungeon Status -------------------" << std::endl;
	for (unsigned int i = 0; i < Dungeon::dungeonList.size(); i++) {
		std::cout << "[DungeonID: " << Dungeon::dungeonList.at(i).dungeonId << "] - [" << Dungeon::dungeonList.at(i).status << "] " << std::endl;
		std::cout << "[Party Served: " << Dungeon::dungeonList.at(i).numServed << "]" << std::endl;
		std::cout << "[Total Time Served: " << Dungeon::dungeonList.at(i).totalTimeServed << "s] " << std::endl;
		for (unsigned int x = 0; x < Party::partyList.size(); x++) {
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

	// Player size for each class
	std::cout << "------------- Player Count without Party -------------" << std::endl;
	std::cout << "Tank Player List Size: " << Player::tankPlayerList.size() << std::endl;
	std::cout << "Healer Player List Size: " << Player::healerPlayerList.size() << std::endl;
	std::cout << "DPS Player List Size: " << Player::dpsPlayerList.size() << std::endl;
	std::cout << "------------------------------------------------------" << std::endl;

	std::cout << "------------- Party Summary Information --------------" << std::endl;
	for (unsigned int i = 0; i < Party::partyList.size(); i++) {
		std::cout << "[PartyID: " << Party::partyList.at(i).partyID << "] - "
				  << "DungeonID: " << Party::partyList.at(i).dungeonID
				  << ", Time In Dungeon: " << Party::partyList.at(i).timeInDungeon
				  << std::endl;
	}
	std::cout << "------------------------------------------------------" << std::endl;
}

int main() {

	Config::getUserInput();
	Config::initializeConfig();

	std::thread startGameThread(&startGame);
	startGameThread.detach();

	viewStatus();

	return 0;
}