#pragma once
#include "partySystem.h"

#include <vector>
#include <string>

struct DungeonConfig {
	int dungeonId;
	int numServed;
	int totalTimeServed;
	std::vector<PartyObject> partyList;
	std::string status;
};