#pragma once
#include <vector>
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>

// Basis: https://stackoverflow.com/questions/15752659/thread-pooling-in-c11

class ThreadPool {
public:
	void start();
	void stop();
	bool busy();

	void giveNewTask(const std::function<void(int threadID, int dungeonTime)>& task);
private:
	void threadLoop(int threadID, int dungeonTime);

	std::vector<std::thread> threadList; //All the dungeons are in here
	std::queue<std::function<void(int threadID, int dungeonTime)>> taskList; //This is where the game starts. "The party is battling inside the dungeon"
	std::mutex taskMutex;
	std::condition_variable taskCV; //https://en.cppreference.com/w/cpp/thread/condition_variable/wait
	bool destroyThread = false;
};