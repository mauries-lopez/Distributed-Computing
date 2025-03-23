#pragma once
#include "partySystem.h"

#include <vector>
#include <string>

struct DungeonObject {
	unsigned int dungeonId;
	unsigned int numServed;
	unsigned int totalTimeServed;
	std::vector<PartyObject> partyList;
	std::string status;
	unsigned int curDungeonTime;
};

class Dungeon {
public:
	static std::vector<DungeonObject> dungeonList;
	static std::mutex dungeonListMutex;
private:
};