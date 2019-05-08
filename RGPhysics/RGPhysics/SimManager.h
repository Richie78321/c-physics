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
	class RigidBody;

	class SimManager
	{
	public:
		SimManager(std::vector<RigidBody> rigidBodies);
	private:

	};
}