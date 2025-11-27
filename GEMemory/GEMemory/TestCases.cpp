#include "PoolAllocator.h"
#include "StackAllocator.h"
#include "TestCases.h"


#include <chrono>
#include <iostream>
#include <random>


void PoolVSOS() {

	int loops = 10;

	int startObjects = 2000;
	std::cout << " ---- Testing PoolAllocation ---- " << std::endl;
	std::mt19937 rng(12345);

	const int FRAMES = 100'000;
	double poolTime[10];
	for (int k = 0; k < loops; k++) {

		int nrOfObjects = 0;
		auto t0 = std::chrono::high_resolution_clock::now();


		PoolAllocator pool;
		pool.Init(startObjects, sizeof(Enemy));
		std::vector<Enemy*> live;
		for (int f = 0; f < FRAMES; f++) {

			// Allocate some random objects
			int allocCount = rng() % 1000;
			for (int i = 0; i < allocCount; i++) {
				Enemy* e = (Enemy*)pool.Request();
				live.push_back(e);
				nrOfObjects++;
			}

			// Free some random objects
			int freeCount = rng() % 1000;
			for (int i = 0; i < freeCount && !live.empty(); i++) {
				int index = rng() % live.size();
				pool.Free(live[index]);
				live.erase(live.begin() + index);
			}

		}
		
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = t1 - t0;

		poolTime[k] = duration.count();
		std::cout << "live size: " << live.size() << std::endl;
		std::cout << "NrOfObjects created: " << nrOfObjects << std::endl;
		std::cout << "Execution time: " << poolTime[k] << " iteration " << k << std::endl;
		live.clear();
	}
	double result = 0;
	for (int i = 0; i < loops; i++) {
		result += poolTime[i];
	}
	result /= 10;
	std::cout << " ---- OS PoolAllocation Execution time: " << result << " ---- " << std::endl;
	std::cout << std::endl;

	std::cout << " ---- Testing OS new/delete ---- " << std::endl;

	std::vector<Enemy*> live2;
	for (int k = 0; k < loops; k++) {

		int nrOfObjects = 0;
		auto t0 = std::chrono::high_resolution_clock::now();
		for (int f = 0; f < FRAMES; f++) {

			// Allocate some random objects
			int allocCount = rng() % 1000;
			for (int i = 0; i < allocCount; i++) {
				Enemy* e = new Enemy;
				live2.push_back(e);
				nrOfObjects++;
			}

			// Free some random objects
			int freeCount = rng() % 1000;
			for (int i = 0; i < freeCount && !live2.empty(); i++) {
				int index = rng() % live2.size();
				delete live2[index];
				live2.erase(live2.begin() + index);
			}

		}
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = t1 - t0;

		std::cout << "live2 size: " << live2.size() << std::endl;
		std::cout << "NrOfObjects created: " << nrOfObjects << std::endl;
		// Remove remaining objects from the vector to prevent memory leaks
		for (int i = 0; i < live2.size(); i++) {
			delete live2[i];
		}
		poolTime[k] = duration.count();
		std::cout << "Execution time: " << poolTime[k] << " iteration " << k << std::endl;
		live2.clear();
	}
	result = 0;
	for (int i = 0; i < loops; i++) {
		result += poolTime[i];
	}
	result /= 10;
	std::cout << " ---- OS new/delete Execution average time: " << result << " ---- " << std::endl;

}

void StackVsOS() {

}