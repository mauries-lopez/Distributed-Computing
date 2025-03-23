#pragma once
#include "playerConfig.h"

#include <mutex>

struct PartyObject {
	unsigned int partyID;
	std::vector<PlayerObject> members;
	unsigned int dungeonID;
	std::string status;
	unsigned int timeInDungeon;
};

class Party {
public:
	static void buildParty();
	static void deployParties();
	static std::vector<PartyObject> partyList;
	static unsigned int doneCount;
	static bool isAllPartyCreated;
private:
	static std::mutex partyListMutex;
};