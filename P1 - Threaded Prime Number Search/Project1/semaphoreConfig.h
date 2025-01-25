#pragma once
#include <semaphore>

// This is used so that I can only initialize SemaphoreConfig once 
class SemaphoreConfig {
public:

    // Constructor accepting thread size at runtime
    SemaphoreConfig(int threadSize);

    // Methods to acquire and release the semaphore
    void acquire() {
        sem.acquire();
    }

    void release() {
        sem.release();
    }

private:
    std::counting_semaphore<> sem;
};