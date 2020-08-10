#pragma once
#include "SimElement.h"
#include <vector>

#ifdef RGPHYSICS_EXPORTS
#define RGPHYSICS_API __declspec(dllexport)
#else
#define RGPHYSICS_API __declspec(dllimport)
#endif

namespace RGPhysics
{
	class SimManager;
	class RBManifest;
	class RigidBody;

	class RGPHYSICS_API RBCollider : public SimElement
	{
	public:
		RBCollider();
		
		void Update(SimManager &simManager);
		std::tuple<bool, RBManifest> GetCollisionManifest(RigidBody &rb1, RigidBody &rb2) const;

		std::vector<RBManifest> tempStorage;
	};
}

