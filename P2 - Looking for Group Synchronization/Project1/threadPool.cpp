#include "threadPool.h"
#include "config.h"
#include "dungeonConfig.h"

#include <functional>
#include <iostream>

std::vector<DungeonObject> Dungeon::dungeonList;

void ThreadPool::giveNewTask(const std::function<void(int threadID)>& task) {
    taskMutex.lock();
    taskList.push(task);
    taskMutex.unlock();
    taskCV.notify_one();
}

// This is like the "Worker" thread
void ThreadPool::threadLoop(int threadID) {
    while (true) {

        //A placeholder where the task will be placed.
        std::function<void(int threadID)> task;

        //Using unique_lock is required to use inside a conditional variable
        std::unique_lock<std::mutex> lock(taskMutex);

        // Let a or (this) thread wait for any task inside taskList. If there is a task (taskCV.notify_one() is called), the thread will continue doing the assigned task from the taskList.
        // While waiting, let all threads temporarily unlock the taskList.
        // If there is one thread (notify_one()) is notified, lock taskList again so no other threads can manipulate.
        taskCV.wait(lock, [this] {
            //Assuming there is always one task in the list (for every other party is created, there is a new task for the dungeon)
            //Also return what is destroyThread contains to destroy the threads
            return !taskList.empty() || destroyThread; 
        });

        // No more task/s, exit
        if (destroyThread == true) {
            return;
        }
            
        // Retrieve a task
        task = taskList.front();
        taskList.pop();
        lock.unlock();

        // Execute the task
        task(threadID);
    }
}

void ThreadPool::start() {
	for (int i = 0; i < Config::numDungeons; i++) {

		threadList.emplace_back(std::thread(&ThreadPool::threadLoop, this, i));

        //Initialize Dungeons
        DungeonObject dungeonObject;
        dungeonObject.dungeonId = i;
        dungeonObject.status = "empty";
        dungeonObject.totalTimeServed = 0;
        dungeonObject.numServed = 0;

        //Put the dungeon in the dungeon list
        Dungeon::dungeonList.push_back(dungeonObject);

        //std::thread thread(&ThreadPool::threadLoop, this, i, dungeonTime);
        //thread.detach();
	}
}

bool ThreadPool::busy() {
    bool poolbusy;
    taskMutex.lock();
    poolbusy = !taskList.empty();
    taskMutex.unlock();

    return poolbusy;
}

void ThreadPool::stop() {
    taskMutex.lock();
    destroyThread = true;
    taskMutex.unlock();
    taskCV.notify_all(); //All threads should be notified.

    for (std::thread& thread : threadList) {
        thread.join();
    }

    threadList.clear();
}