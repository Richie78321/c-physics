#include "stdafx.h"
#include "RBCollider.h"
#include "SimManager.h"
#include "RigidBody.h"
#include "RBManifest.h"
#include "PolygonShape.h"
#include <tuple>

namespace RGPhysics
{
	const int CORRECTION_PASSES = 6;

	RBCollider::RBCollider()
	{
	}

	void RBCollider::Update(SimManager &simManager)
	{
		//Get RigidBodies
		std::vector<SimElement*> simElements = simManager.GetSimElements();
		std::vector<RigidBody*> rigidBodies;
		for (int i = simElements.size() - 1; i >= 0; i--)
		{
			if (RigidBody *r = dynamic_cast<RigidBody*>(simElements[i]))
			{
				rigidBodies.push_back(r);
			}
		}

		std::vector<RBManifest> generatedManifests;
		if (rigidBodies.size() >= 2)
		{
			//Check for intersections
			for (int i = rigidBodies.size() - 1; i >= 1; i--)
			{
				for (int j = i - 1; j >= 0; j--)
				{
					//Low-level collision
					if (PolygonShape::IsLowLevelIntersection(*rigidBodies[i]->GetCollisionPolygon(), *rigidBodies[j]->GetCollisionPolygon()))
					{
						//High-level collision
						std::tuple<bool, RBManifest> manifestResult = this->GetCollisionManifest(*rigidBodies[i], *rigidBodies[j]);
						if (std::get<0>(manifestResult))
						{
							//Add to list
							generatedManifests.push_back(std::get<1>(manifestResult));
						}
					}
				}
			}
		}

		//Temp
		this->tempStorage = generatedManifests;

		//Generate contact points
		for (int i = generatedManifests.size() - 1; i >= 0; i--)
		{
			generatedManifests[i].GetContactPoints();
		}

		//Settle manifests
		for (int j = 0; j < CORRECTION_PASSES; j++)
		{
			for (int i = generatedManifests.size() - 1; i >= 0; i--)
			{
				generatedManifests[i].SettleCollisionPass();
			}
		}
	}

	std::tuple<bool, RBManifest> RBCollider::GetCollisionManifest(RigidBody &rbA, RigidBody &rbB) const
	{
		std::tuple<bool, Vector2, float, Vector2> intersectionResult = PolygonShape::AreIntersecting(*rbA.GetCollisionPolygon(), *rbB.GetCollisionPolygon());
		if (std::get<0>(intersectionResult))
		{
			return std::tuple<bool, RBManifest>(true, RBManifest(std::get<1>(intersectionResult), &rbA, &rbB));
		}
		else return std::tuple<bool, RBManifest>(false, RBManifest(Vector2(), NULL, NULL));
	}
}