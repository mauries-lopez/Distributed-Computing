#pragma once
#include "partySystem.h"

#include <vector>
#include <string>

struct DungeonObject {
	int dungeonId;
	int numServed;
	int totalTimeServed;
	std::vector<PartyObject> partyList;
	std::string status;
	int curDungeonTime;
};

class Dungeon {
public:
	static std::vector<DungeonObject> dungeonList;
	static std::mutex dungeonListMutex;
private:
};