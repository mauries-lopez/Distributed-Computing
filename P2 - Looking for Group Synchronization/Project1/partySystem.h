#pragma once
#include "playerConfig.h"

#include <mutex>

struct PartyObject {
	int partyID;
	std::vector<PlayerObject> members;
	int dungeonID;
};

class Party {
public:
	static void buildParty();
	static void deployParties();
	static std::vector<PartyObject> partyList;
private:
	static std::mutex partyListMutex;
};