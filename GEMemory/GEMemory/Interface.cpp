#include "Interface.h"

#include "MemoryTracker.h"

#include <cstdlib>
#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

void Interface::ShowPoolInfo()
{
	std::unordered_map<int, PoolStats> poolAllocators = MemoryTracker::Instance().GetPoolAllocators();
	std::vector<std::string> poolIds;
	for (auto &pair : poolAllocators) {
		poolIds.push_back(std::to_string(pair.first));
	}
	static int currentPool = 0; // Current pool ID
	ImGui::Combo("PoolAllocators", &currentPool, [](void *data, int idx, const char **out_text)
		{
			auto &vec = *static_cast<std::vector<std::string>*>(data);
			if (idx < 0 || idx >= vec.size()) return false;
			*out_text = vec[idx].c_str();
			return true;
		}, &poolIds, poolIds.size());

	static float poolPercent = 0.0f;

	ImGui::NewLine();
	ImGui::Text("Allocation to current allocator");

	// Allocate
	const int nPools = poolAllocators.size();
	if (ImGui::Button("Allocate") && nPools > 0) {
		for (int i = 0; i < nPools; i++) {
			PoolContainer *currPool = &_pools[i];

			if (currentPool == currPool->pool->GetId()) {
				void *ptr = currPool->pool->Request();
				if (ptr) {
					currPool->ptrs.push_back(ptr);
				}

				// Update tracker
				PoolStats poolStats = currPool->pool->GetStats();
				MemoryTracker::Instance().TrackAllocator(currPool->pool->GetId(), poolStats);
				poolPercent = (float)poolStats.usedMemory / poolStats.capacity;

				break;
			}
		}
	}

	// Free
	// There needs to be at least 1 active pool allocator
	if (ImGui::Button("Free random") && nPools > 0) {
		for (int i = 0; i < nPools; i++) {
			PoolContainer *currPool = &_pools[i];

			if (currentPool == currPool->pool->GetId() && currPool->ptrs.size() > 0) {
				int randIdx = std::rand() % currPool->ptrs.size();
				if (currPool->pool->Free(currPool->ptrs[randIdx])) {
					currPool->ptrs.erase(currPool->ptrs.begin() + randIdx);
				}

				// Update tracker
				PoolStats poolStats = currPool->pool->GetStats();
				MemoryTracker::Instance().TrackAllocator(currPool->pool->GetId(), poolStats);
				poolPercent = (float)poolStats.usedMemory / poolStats.capacity;

				break;
			}
		}
	}

	// Info
	ImGui::ProgressBar(poolPercent, ImVec2(0.0f, 0.0f));
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::Text("Used");

	for (int i = 0; i < nPools; i++) {
		PoolAllocator *currPool = _pools[i].pool;

		if (currentPool == currPool->GetId()) {
			ImDrawList *draw = ImGui::GetWindowDrawList();
			ImVec2 p = ImGui::GetCursorScreenPos();

			int poolNum = currPool->GetNumSlots();
			float blockWidth = 40.0f;
			float blockHeight = 20.0f;
			for (int i = 0; i < poolNum; i++) {
				bool used = currPool->GetUsed(i);
				ImU32 col = used ? IM_COL32(255, 0, 0, 255) : IM_COL32(0, 255, 0, 255);

				draw->AddRectFilled(ImVec2(p.x + i * blockWidth, p.y), ImVec2(p.x + (i + 1) * blockWidth - 1, p.y + blockHeight), col);
			}

			break;
		}
	}
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
	BeginDrawing();
	ClearBackground(BLACK);

	rlImGuiBegin();

	ImGui::Begin(" ", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2(_width, _height));

	// Create allocator
	ImGui::Text("Create new allocator");
	static int currentType = 0;
	static int size = 0;
	static int slots = 10;
	const char *type[] = { "PoolAllocator", "BuddyAllocator", "StackAllocator" };
	ImGui::Combo("New Allocator", &currentType, type, IM_ARRAYSIZE(type));

	// What is need to initialize the chosen allocator
	if (currentType != 0) {
		ImGui::InputInt("Allocator size", &size);
	}
	else {
		ImGui::InputInt("Slot size", &size);
		ImGui::InputInt("Num of slots", &slots);
	}

	if (ImGui::Button("Create allocator")) {
		if (currentType == 0) { // Pool
			PoolContainer pCon;
			pCon.pool = new PoolAllocator;
			if (pCon.pool->Init(slots, size)) {
				_pools.push_back(pCon);
			}
		}
	}

	// Allocator info
	ImGui::NewLine();
	ImGui::Text("Allocator info");

	static int chosenType = 0;
	ImGui::Combo("Allocator types", &chosenType, type, IM_ARRAYSIZE(type));

	if (chosenType == 0) { // Pool
		ShowPoolInfo();
	}

	ImGui::End();
	rlImGuiEnd();
	EndDrawing();
}
