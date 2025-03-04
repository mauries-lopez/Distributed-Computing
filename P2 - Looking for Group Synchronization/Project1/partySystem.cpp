#include "partySystem.h"
#include "threadPool.h"
#include "config.h"
#include "playerConfig.h"

#include <string>
#include <sstream>
#include <iostream>
#include <thread>

std::vector<PartyObject> Party::partyList;
std::mutex Party::partyListMutex;

void Party::deployParties() {
	ThreadPool threadPool;
	threadPool.start(); // Create the number of dungeons
    int i = 0;

    std::cout << "Party Created: " << Party::partyList.size() << std::endl;

    // CONTINUE HERE!!!!
    // PROBLEM: Dungeon is still broken. 2 dungeons and 3 parties, ung isang extrang party hindi na makapasok sa dungeon.
    // FIX?: Dungeon status is not yet implemented. If a dungeon is found available, give task there. Else, wait.

    while (!Party::partyList.empty()) {
        // Capture the first party in the list by value to avoid race conditions
        PartyObject currentParty = Party::partyList.front();

        // Assign the task to the thread pool, capture currentParty by value
        threadPool.giveNewTask([currentParty](int threadID, int dungeonTime) {
            int originalTime = dungeonTime;
            do {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "[Dungeon #" << threadID << "] Duration: " << dungeonTime << "/" << originalTime << " Taken by Party#:" << currentParty.partyID << std::endl;
                dungeonTime--;
            } while (dungeonTime != 0);
            });

        // Safely erase the first element of the list
        partyListMutex.lock();
        Party::partyList.erase(Party::partyList.begin());
        partyListMutex.unlock();
    }

    threadPool.busy();
    threadPool.stop();

	for (int i = 0; i < Party::partyList.size(); i++) {
		for (int x = 0; x < 5; x++) {
			std::cout << "Party#" << Party::partyList.at(i).partyID << " PlayerID:" << Party::partyList.at(i).members.at(x).playerId << " Class:" << Party::partyList.at(i).members.at(x).classType << std::endl;
		}
	}

    std::cout << "Tank Player List Size: " << Player::tankPlayerList.size() << std::endl;
    std::cout << "Healer Player List Size: " << Player::healerPlayerList.size() << std::endl;
    std::cout << "DPS Player List Size: " << Player::dpsPlayerList.size() << std::endl;
    
}

void Party::buildParty() {
    bool canBuildFullParty = true;
    int partyID = 0;

    while (canBuildFullParty) {

        PartyObject partyObject;
        partyObject.partyID = partyID;

        // Check if we have enough players to form a full party (1 tank, 1 healer, 3 DPS)
        if (Player::tankPlayerList.size() < 1 || Player::healerPlayerList.size() < 1 || Player::dpsPlayerList.size() < 3) {
            canBuildFullParty = false;  // If we don't have enough players, stop building parties
            //std::cout << "Not enough players to form a full party." << std::endl;
        }
        else {
            // Get 1 Tank from tankPlayerList
            if (!Player::tankPlayerList.empty()) {
                PlayerObject player;

                // Get the first player in the list of tanks
                player = *(Player::tankPlayerList.begin());

                // Remove the player from the list
                Player::tankPlayerList.erase(Player::tankPlayerList.begin());

                // Put player in the party
                partyObject.members.push_back(player);
            }

            // Get 1 Healer from healerPlayerList
            if (!Player::healerPlayerList.empty()) {
                PlayerObject player;

                // Get the first player in the list of healers
                player = *(Player::healerPlayerList.begin());

                // Remove the player from the list
                Player::healerPlayerList.erase(Player::healerPlayerList.begin());

                // Put player in the party
                partyObject.members.push_back(player);
            }

            // Get 3 DPS from dpsPlayerList
            for (int i = 0; i < 3 && !Player::dpsPlayerList.empty(); i++) {
                PlayerObject player;

                // Get the first player in the list of DPS
                player = *(Player::dpsPlayerList.begin());

                // Remove the player from the list
                Player::dpsPlayerList.erase(Player::dpsPlayerList.begin());

                // Put player in the party
                partyObject.members.push_back(player);
            }

            //std::cout << "Party built successfully!" << std::endl;

            // Add the party to the party list
            Party::partyList.push_back(partyObject);
            partyID++;
        }
    }
}


