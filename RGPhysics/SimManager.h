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

	class RGPHYSICS_API SimManager
	{
	public:
		SimManager(std::vector<SimElement*> simElements);
		SimManager();

		void Update();
		void Update(float elapsedMillis);

		float GetElapsedMillis() const;
		std::vector<SimElement*> GetSimElements() const;

		void AddSimElement(SimElement *simElement);
		bool RemoveSimElement(SimElement *simElement);
	private:
		std::vector<SimElement*> simElements;
		std::chrono::time_point<std::chrono::steady_clock> lastUpdate;
		float elapsedMillis;
	};
}