#pragma once
#include <vector>
#include <chrono>

#ifdef RGPHYSICS_EXPORTS
#define RGPHYSICS_API __declspec(dllexport)
#else
#define RGPHYSICS_API __declspec(dllimport)
#endif

namespace RGPhysics
{
	class SimElement;

	class SimManager
	{
	public:
		SimManager(std::vector<SimElement> simElements);
		SimManager();

		void Update();
		unsigned int GetElapsedMillis() const;
	private:
		std::chrono::time_point<std::chrono::steady_clock> lastTick;
		std::vector<SimElement> simElements;
		unsigned int elapsedMillis = 0;
	};
}