#include <iostream>
#include "PoolAllocator.h"
#include "StackAllocator.h"
#include "BuddyAllocator.h"
#include "TestCases.h"

#include "Interface.h"
#include "Objects.h"

#include <iomanip>
#include <ctime>
#include <sstream>
#include <cstdlib>

#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

// Time to string formatting
inline std::string FormatTimePoint(const std::chrono::system_clock::time_point& tp) {
	std::time_t timeStamp = std::chrono::system_clock::to_time_t(tp);
	std::tm localTime{};

	localtime_s(&localTime, &timeStamp); // thread-safe Windows version

	std::ostringstream oss;
	oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}

int main() {
	Interface interface;

	while (!WindowShouldClose()) {
		interface.Update();
	}

	CloseWindow();

	//PoolVSOS();
	TestAll();
	return 0;
}