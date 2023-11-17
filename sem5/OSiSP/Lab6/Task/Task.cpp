#include <Windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <random>

const int NUM_PHILOSOPHERS = 10;
HANDLE forks[NUM_PHILOSOPHERS];
HANDLE printMutex;

std::mt19937 rng(std::random_device{}());

void Think(int philosopherId) {
    WaitForSingleObject(printMutex, INFINITE);

    std::cout << "#" << philosopherId << " is thinking." << std::endl;
    ReleaseMutex(printMutex);

    std::uniform_int_distribution<int> distribution(3000, 10000);
    Sleep(distribution(rng)); // Simulate thinking process
}

void Eat(int philosopherId) {
    WaitForSingleObject(printMutex, INFINITE);

    std::cout << "#" << philosopherId << " is eating." << std::endl;
    ReleaseMutex(printMutex);

    std::uniform_int_distribution<int> distribution(4000, 7000);
    Sleep(distribution(rng)); // Simulate eating process
}

void GetForks(int philosopherId) {
    /*WaitForSingleObject(printMutex, INFINITE);
    std::cout << "Philosopher " << philosopherId << " is trying to get forks." << std::endl;
    ReleaseMutex(printMutex);*/

    HANDLE forkHandles[2] = {forks[philosopherId], forks[(philosopherId + 1) % NUM_PHILOSOPHERS]};
    DWORD result = WaitForMultipleObjects(2, forkHandles, TRUE, INFINITE); // Wait for both forks

    /*WaitForSingleObject(printMutex, INFINITE);
    std::cout << "Philosopher " << philosopherId << " got forks." << std::endl;
    ReleaseMutex(printMutex);*/
}

void ReleaseForks(int philosopherId) {
    ReleaseMutex(forks[philosopherId]); // Release left fork
    ReleaseMutex(forks[(philosopherId + 1) % NUM_PHILOSOPHERS]); // Release right fork

    /*WaitForSingleObject(printMutex, INFINITE);
    std::cout << "Philosopher " << philosopherId << " released forks." << std::endl;
    ReleaseMutex(printMutex);*/
}

void Philosopher(int philosopherId) {
    while(true) {
        Think(philosopherId);
        GetForks(philosopherId);
        Eat(philosopherId);
        ReleaseForks(philosopherId);
    }
}

int main() {
    printMutex = CreateMutex(NULL, FALSE, NULL);

    // Create forks
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        forks[i] = CreateMutex(NULL, FALSE, NULL);
    }

    // Create philosophers
    std::vector<std::thread> philosophers;
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers.emplace_back(Philosopher, i);
    }

    // Wait for all philosophers to finish
    for(auto& philosopher : philosophers) {
        philosopher.join();
    }

    // Close handles
    CloseHandle(printMutex);
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        CloseHandle(forks[i]);
    }

    return 0;
}