#include "config.h"
#include "playerConfig.h"

#include <string>
#include <sstream>
#include <iostream>
#include <thread>

// Config Header
unsigned int Config::numDungeons;
unsigned int Config::numTanks;
unsigned int Config::numHealers;
unsigned int Config::numDps;
unsigned int Config::dungeonMinTime;
unsigned int Config::dungeonMaxTime;
unsigned int Config::virtualTime;
std::vector<std::string> Config::progressBarParties;
std::vector<std::string> Config::progressBarDungeons;

// Player Header
std::vector<PlayerObject> Player::tankPlayerList;
std::vector<PlayerObject> Player::healerPlayerList;
std::vector<PlayerObject> Player::dpsPlayerList;

// Valid Input Checker
void getInputAndValidate(std::string question, std::string subject, unsigned int& configParam) {

	bool isPassedNumberCheck = false, isPassedLogicCheck = false, isPassedAllChecks = false;
	unsigned int tempInput;

	// For NUMBER checking
	// - No string and characters allowed
	// - still has bugs with "5@" may kasunod na character <---------- FIX THIS

	//For LOGIC
	//[1] - 1 Tank [/], 1 Healer[/], 3 DPS[/]
	//[2] - If it lacks 1 or more, dont continue. Prompt unable to create atleast 1 party, roles unable to filled.
	//[3] - No zero value in each role [/]
	//[4] - Check for these values:
	//		(negative max - 1) [/]
	//		(negative max) [/]
	//		(-1) [/]
	//		(0) [/]
	//		(1) [/]
	//		(max) 
	//		(max + 1)
	//[5] - max is less than min [/]
	//[6] - 
	do {
		std::cout << question;
		std::cin >> tempInput;
		
		// NUMBER Checking
		if (!(std::cin.fail())) {
			isPassedNumberCheck = true;
		}
		else {
			std::cout << "[ERROR: Input postive numerical values only.]" << std::endl;
		}

		// LOGIC Checking (after first layer of checking is done.)
		if (isPassedNumberCheck == true) {
			// Reset Check, so it can only enter once
			isPassedNumberCheck = false;

			if (tempInput <= 0 || (int)tempInput <= 0) { // (int)tempInput checks for the integer version of the unsigned integer
				std::cout << "[ERROR: Only greater than 0 is allowed.]" << std::endl;
			}
			else if (subject == "dps" && tempInput < 3) { // Atleast 3 DPS
				std::cout << "[ERROR: Not enough DPS to create a party. Atleast 3 DPS is needed.]" << std::endl;
			}
			else if (subject == "maxtime" && tempInput < Config::dungeonMinTime) {
				std::cout << "[ERROR: Maxmum Time should be greater than the Minimum Time.]" << std::endl;
			}
			else {
				isPassedAllChecks = true;
			}
		}

		if (isPassedAllChecks == false) {
			//Reset Input
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		
		configParam = tempInput;
		
	} while (isPassedAllChecks == false);
}

void Config::getUserInput() {

	std::string questions[6] = { "Number of Dungeons : ", "Number of Tank Players: ","Number of Healer Players: ",
								"Number of DPS Players: ","Minimum time before a dungeon is finished: ",
								"Maximum time before a dungeon is finished: " };
	std::string subject[6] = { "dungeon", "tank", "healer", "dps", "mintime", "maxtime" };
	unsigned int* params[6] = { &Config::numDungeons, &Config::numTanks, &Config::numHealers, &Config::numDps, &Config::dungeonMinTime, &Config::dungeonMaxTime };

	for (unsigned int x = 0; x < 6; x++) {
		getInputAndValidate(questions[x], subject[x], *params[x]);
	}
}

void Config::initializeConfig() {

	//Get Total Amount of Players
	unsigned int numAllPlayers = Config::numTanks + Config::numHealers + Config::numDps;

	//Get Number of each class
	unsigned int nTanks = Config::numTanks;
	unsigned int nHealers = Config::numHealers;
	unsigned int nDps = Config::numDps;

	//Initialize Player Object
	for (unsigned int i = 0; i < numAllPlayers; i++) {

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
	for (unsigned int i = 0; i < Config::numDungeons; i++) {

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