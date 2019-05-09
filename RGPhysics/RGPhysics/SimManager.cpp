#include "stdafx.h"
#include "SimElement.h"
#include "SimManager.h"

RGPhysics::SimManager::SimManager()
{
	this->simElements = std::vector<SimElement>();
	this->lastTick = std::chrono::steady_clock::now();
}

RGPhysics::SimManager::SimManager(std::vector<SimElement> simElements)
{
	this->simElements = simElements;
	this->lastTick = std::chrono::steady_clock::now();
}

unsigned int RGPhysics::SimManager::GetElapsedMillis() const
{
	return this->elapsedMillis;
}

void RGPhysics::SimManager::Update()
{
	//Get time elapsed since last
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<double> diff = this->lastTick - currentTime;
	this->elapsedMillis = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
	this->lastTick = currentTime;
}
