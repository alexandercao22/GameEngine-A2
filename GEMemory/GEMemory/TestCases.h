#pragma once

struct Enemy {
	float health = 100.0f;
	int legs = 3;
	char tag = 'a';
};


void PoolVSOS();
void StackVsOS();