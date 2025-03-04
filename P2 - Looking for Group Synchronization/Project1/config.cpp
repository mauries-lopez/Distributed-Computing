#include "config.h"
#include "playerConfig.h"

#include <string>
#include <sstream>
#include <iostream>
#include <thread>

// Config Header
int Config::numDungeons;
int Config::numTanks;
int Config::numHealers;
int Config::numDps;
int Config::dungeonMinTime;
int Config::dungeonMaxTime;
int Config::virtualTime;

// Player Header
std::vector<PlayerObject> Player::tankPlayerList;
std::vector<PlayerObject> Player::healerPlayerList;
std::vector<PlayerObject> Player::dpsPlayerList;

void Config::getUserInput() {

	std::cout << "Number of Dungeons: ";
	std::cin >> Config::numDungeons;

	std::cout << "Number of Tank Players: ";
	std::cin >> Config::numTanks;

	std::cout << "Number of Healer Players: ";
	std::cin >> Config::numHealers;

	std::cout << "Number of DPS Players: ";
	std::cin >> Config::numDps;

	std::cout << "Minimum time before a dungeon is finished: ";
	std::cin >> Config::dungeonMinTime;

	std::cout << "Maximum time before a dungeon is finished: ";
	std::cin >> Config::dungeonMaxTime;

	system("cls");
}

void Config::initializeConfig() {

	//Get Total Amount of Players
	int numAllPlayers = Config::numTanks + Config::numHealers + Config::numDps;

	//Get Number of each class
	int nTanks = Config::numTanks;
	int nHealers = Config::numHealers;
	int nDps = Config::numDps;

	//Initialize Player Object
	for (int i = 0; i < numAllPlayers; i++) {

		//Create Object
		PlayerObject player;

		//Assign Player ID
		player.playerId = i;

		if (nTanks != 0) {
			//Assign Class
			player.classType = "Tank";
			//Put to Array of Players
			Player::tankPlayerList.push_back(player);
			nTanks--;
		}
		else if (nHealers != 0) {
			//Assign Class
			player.classType = "Healer";
			//Put to Array of Players
			Player::healerPlayerList.push_back(player);
			nHealers--;
		}
		else if (nDps != 0) {
			player.classType = "DPS";
			//Put to Array of Players
			Player::dpsPlayerList.push_back(player);
			nDps--;
		}
	}
	
	//Initialize Dungeon Object
	for (int i = 0; i < Config::numDungeons; i++) {

	}

	// Check Player Information
	/*for (int i = 0; i < Config::numTanks; i++) {
		std::cout << Player::tankPlayerList.at(i).playerId << "[" << Player::tankPlayerList.at(i).classType << "]" << std::endl;
	}

	for (int i = 0; i < Config::numHealers; i++) {
		std::cout << Player::healerPlayerList.at(i).playerId << "[" << Player::healerPlayerList.at(i).classType << "]" << std::endl;
	}

	for (int i = 0; i < Config::numDps; i++) {
		std::cout << Player::dpsPlayerList.at(i).playerId << "[" << Player::dpsPlayerList.at(i).classType << "]" << std::endl;
	}*/
}