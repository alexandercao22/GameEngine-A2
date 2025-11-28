#include "Interface.h"

#include "MemoryTracker.h"

#include <cstdlib>
#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

void Interface::ShowPoolInfo()
{
}

void Interface::ShowBuddyInfo()
{
}

void Interface::ShowStackInfo()
{
}

Interface::Interface()
{
	Init();
}

Interface::~Interface()
{
	for (PoolContainer pCon : _pools) {
		delete pCon.pool;
	}
	for (BuddyContainer bCon : _buddies) {
		delete bCon.buddy;
	}
	for (StackContainer sCon : _stacks) {
		delete sCon.stack;
	}
}

bool Interface::Init()
{
	std::srand(std::time({}));

	InitWindow(_width, _height, "Game Engine Assignment 2");
	SetTargetFPS(60);

	rlImGuiSetup(true);

	return false;
}

void Interface::Update()
{
}
