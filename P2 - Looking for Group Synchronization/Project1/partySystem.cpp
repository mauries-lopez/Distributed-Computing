#include "partySystem.h"
#include "threadPool.h"
#include "config.h"
#include "playerConfig.h"
#include "dungeonConfig.h"

#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <random>

std::vector<PartyObject> Party::partyList;
std::mutex Party::partyListMutex;
unsigned int Party::doneCount;
std::mutex Dungeon::dungeonListMutex;
bool Party::isAllPartyCreated;

ThreadPool* threadPool = new ThreadPool();

void Party::deployParties() {

    unsigned int i = 0;
    size_t dungeonListSize = Dungeon::dungeonList.size();
    unsigned int x = 0;
    size_t partyListSize = Party::partyList.size();

    while (true) {

        if (Party::doneCount == partyListSize) {
            break;
        }

        if (i >= dungeonListSize) {
            i = 0;
        }

        if (x >= partyListSize) {
            x = 0;
        }

        if (Party::partyList.at(x).status == "queued") {
            if (Dungeon::dungeonList.at(i).status == "empty") {
                Party::partyList.at(x).dungeonID = i;
                Party::partyList.at(x).status = "active";
                Dungeon::dungeonList.at(i).status = "active";
                // Assign the task to the thread pool, capture currentParty by value
                threadPool->giveNewTask([x, i](unsigned int threadID) {
                    unsigned int originalTime = Party::partyList.at(x).timeInDungeon;
                    unsigned int dungeonTime = 0;
                    //Dungeon Life Time
                    do {
                        Dungeon::dungeonList.at(i).curDungeonTime = dungeonTime;
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        dungeonTime++;
                        Dungeon::dungeonList.at(i).curDungeonTime = dungeonTime;
                        Dungeon::dungeonList.at(i).totalTimeServed++;
                        //std::cout << "[Dungeon #" << threadID << "] Duration: " << dungeonTime << "/" << originalTime << " Taken by Party#: " << Party::partyList.at(x).partyID  << std::endl;
                    } while (dungeonTime < originalTime);
                    Dungeon::dungeonList.at(i).status = "empty";
                    Party::doneCount++;
                    Dungeon::dungeonList.at(i).numServed++;
                    Party::partyList.at(x).status = "finished";
                 });
                x++;
            }
        }
        i++;
    }

    threadPool->busy();
    threadPool->stop();
}

void Party::buildParty() {

    std::thread poolThread(&ThreadPool::start, threadPool);
    poolThread.detach();
    //threadPool.start();

    bool canBuildFullParty = true;
    unsigned int partyID = 0;

    while (canBuildFullParty) {

        system("cls");

        PartyObject partyObject;

        //Generate a random time within the interval
        //https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
        std::random_device randomDevice;
        std::mt19937 gen(randomDevice());
        std::uniform_int_distribution<> distr(Config::dungeonMinTime, Config::dungeonMaxTime);

        partyObject.timeInDungeon = distr(gen);
        partyObject.partyID = partyID;
        partyObject.status = "queued";

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
            for (unsigned int i = 0; i < 3 && !Player::dpsPlayerList.empty(); i++) {
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

        std::string log = "[SYSTEM: Party #" + std::to_string(partyID) + " created.]";
        Config::progressBarParties.insert(Config::progressBarParties.begin(), log);

        std::cout << Config::progressBarParties.at(0) << std::endl;
        std::cout << Config::progressBarDungeons.at(0) << std::endl;
    }

    Party::isAllPartyCreated = true;
    std::cout << "[SYSTEM: Parties created]" << std::endl;
    Party::deployParties();
}