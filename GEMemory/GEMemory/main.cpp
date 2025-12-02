#include <iostream>
#include "PoolAllocator.h"
#include "StackAllocator.h"
#include "BuddyAllocator.h"
#include "TestCases.h"

#include "Interface.h"
#include "Objects.h"

#include <cstdlib>

#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

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