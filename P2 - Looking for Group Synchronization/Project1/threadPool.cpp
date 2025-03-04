#include "threadPool.h"
#include "config.h"

#include <functional>
#include <iostream>
#include <random>

// Currently testing for 1 thread (para hindi nakakagulo for synchronization)

void ThreadPool::giveNewTask(const std::function<void(int threadID, int dungeonTime)>& task) {
    taskMutex.lock();
    taskList.push(task);
    taskMutex.unlock();
    taskCV.notify_one();
}

// This is like the "Worker" thread
void ThreadPool::threadLoop(int threadID, int dungeonTime) {
    while (true) {

        //A placeholder where the task will be placed.
        std::function<void(int threadID, int dungeonTime)> task;

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
        task(threadID, dungeonTime);
    }
}

void ThreadPool::start() {
	for (int i = 0; i < Config::numDungeons; i++) {

        //Generate a random time within the interval
        //https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
        std::random_device randomDevice;
        std::mt19937 gen(randomDevice());
        std::uniform_int_distribution<> distr(Config::dungeonMinTime, Config::dungeonMaxTime);
        int dungeonTime = distr(gen);

		threadList.emplace_back(std::thread(&ThreadPool::threadLoop, this, i, dungeonTime));
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