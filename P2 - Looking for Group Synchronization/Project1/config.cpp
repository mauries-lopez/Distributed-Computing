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

	bool isPassedNumberCheck = false, isPassedConversionCheck = false, isPassedLogicCheck = false, isPassedAllChecks = false;
	unsigned int mainInput = 0;
	std::string tempInput;

	// For NUMBER checking
	// - No string and characters allowed

	//For LOGIC
	//[1] - 1 Tank [/], 1 Healer[/], 3 DPS[/]
	//[2] - If it lacks 1 or more, dont continue. Prompt unable to create atleast 1 party, roles unable to filled. [/]
	//[3] - No zero value in each role [/]
	//[4] - Check for these values:
	//		(negative max - 1) [/]
	//		(negative max) [/]
	//		(-1) [/]
	//		(0) [/]
	//		(1) [/]
	//		(max) [/]
	//		(max + 1) [/]
	//[5] - max is less than min [/]

	do {
		std::cout << question;
		std::getline(std::cin, tempInput);

		// NUMBER Checking (first layer of checking)
		for (char c : tempInput) {
			if (!(std::isdigit(c)) || std::isspace(c)) {
				std::cout << "[ERROR: Input postive numerical values only.]" << std::endl;
				isPassedNumberCheck = false;
				break;
			}
			else {
				isPassedNumberCheck = true;
			}
		}

		if (isPassedNumberCheck == true) {
			//Reset previous check
			//isPassedNumberCheck = false;

			unsigned long temp;
			try {
				temp = std::stoul(tempInput);

				// Check temp if it goes beyond the limit of unsigned int max before converting it to it.
				if (temp > std::numeric_limits<unsigned int>::max()) {
					std::cout <<  "[ERROR: Value is too large. Inputted value should be within 1 - 1000.]" << std::endl;
				}
				else {
					mainInput = (unsigned int)(temp);
					isPassedConversionCheck = true;
				}
			}
			catch (const std::out_of_range& e) { //To catch error of string to unsigned long conversion
				std::cout << "[ERROR: Value is too large. Inputted value should be within 1 - 1000.]"<< std::endl;
			}
		}

		// LOGIC Checking (second layer of checking)
		if (isPassedNumberCheck == true && isPassedConversionCheck == true) {
			// Reset Check, so it can only enter once
			isPassedNumberCheck = false;
			isPassedConversionCheck = false;

			if (mainInput <= 0 || (int)mainInput <= 0) { // (int)tempInput checks for the integer version of the unsigned integer
				std::cout << "[ERROR: Only greater than 0 is allowed.]" << std::endl;
			}
			else if (subject == "dps" && mainInput < 3) { // Atleast 3 DPS
				std::cout << "[ERROR: Not enough DPS to create a party. Atleast 3 DPS is needed.]" << std::endl;
			}
			else if (subject == "maxtime" && mainInput < Config::dungeonMinTime) { // Maxtime should not be less than mintime
				std::cout << "[ERROR: Maxmum Time should be greater than the Minimum Time.]" << std::endl;
			}
			else if ( mainInput > 1000 ) { // Should limit unsigned int max input.
				// Limit to 1000
				// I tested 1000, considerable na siya na pwedeng hintayin para matest ang code for huge quantity. 
				std::cout << "[ERROR: Value is too large. Inputted value should be within 1 - 1000.]" << std::endl;
			}
			else {
				isPassedAllChecks = true;
			}
		}

		if (isPassedAllChecks == false) {
			//Reset Input
			std::cin.clear();
		}
		
		configParam = mainInput;
		
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