#pragma once
#include "playerConfig.h"

#include <mutex>

struct PartyObject {
	int partyID;
	std::vector<PlayerObject> members;
	int dungeonID;
	std::string status;
	int timeInDungeon;
};

class Party {
public:
	static void buildParty();
	static void deployParties();
	static std::vector<PartyObject> partyList;
	static int doneCount;
private:
	static std::mutex partyListMutex;
};