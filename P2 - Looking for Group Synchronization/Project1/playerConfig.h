#pragma once

#include <vector>
#include <string>

struct PlayerObject {
	int playerId;
	std::string classType;
};

class Player {
public:
	static std::vector<PlayerObject> tankPlayerList;
	static std::vector<PlayerObject> healerPlayerList;
	static std::vector<PlayerObject> dpsPlayerList;
private:
};