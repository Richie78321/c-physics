#include "stdafx.h"
#include "SimElement.h"
#include "SimManager.h"

namespace RGPhysics
{
	SimManager::SimManager(std::vector<SimElement*> simElements)
	{
		this->lastUpdate = std::chrono::steady_clock::now();
		this->simElements = simElements;
	}

	SimManager::SimManager()
	{
		this->lastUpdate = std::chrono::steady_clock::now();
	}

	void SimManager::Update()
	{
		//Get current elapsed milliseconds
		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<double, std::milli> diff = currentTime - this->lastUpdate;
		this->elapsedMillis = diff.count();
		this->lastUpdate = currentTime;

		for (int i = 0; i < this->simElements.size(); i++)
		{
			this->simElements[i]->Update((*this));
		}
	}

	void SimManager::Update(float elapsedMillis)
	{
		//Set current elapsed millis
		this->elapsedMillis = elapsedMillis;
		auto currentTime = std::chrono::steady_clock::now();
		this->lastUpdate = currentTime;

		for (int i = 0; i < this->simElements.size(); i++)
		{
			this->simElements[i]->Update((*this));
		}
	}

	float SimManager::GetElapsedMillis() const
	{
		return this->elapsedMillis;
	}

	void SimManager::AddSimElement(SimElement *simElement)
	{
		this->simElements.push_back(simElement);
	}

	bool SimManager::RemoveSimElement(SimElement *simElement)
	{
		//Improve if necessary
		for (int i = this->simElements.size() - 1; i >= 0; i--)
		{
			if (simElement == this->simElements[i])
			{
				this->simElements.erase(this->simElements.begin() + i);
				return true;
			}
		}

		return false;
	}

	std::vector<SimElement*> SimManager::GetSimElements() const
	{
		return this->simElements;
	}
}