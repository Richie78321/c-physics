#pragma once
#ifdef RGPHYSICS_EXPORTS
#define RGPHYSICS_API __declspec(dllexport)
#else
#define RGPHYSICS_API __declspec(dllimport)
#endif
#include "Vector2.h"
#include "LineSegment.h"

namespace RGPhysics
{
	class RigidBody;
	class LineSegment;

	struct RGPHYSICS_API RBManifest
	{
	public:
		RBManifest(Vector2 collisionAxis, RigidBody *rbA, RigidBody *rbB);

		Vector2 collisionAxis;
		RigidBody *rbA;
		RigidBody *rbB;

		void SettleCollisionPass();
	//private:
		std::vector<float> applyImpulse();
		void applyFriction(std::vector<float> impulse_magnitudes);
		void applyPositionalCorrection();

		std::vector<std::tuple<Vector2, float>> contact_points;
		std::vector<std::tuple<Vector2, float>> GetContactPoints();
		LineSegment GetBestEdge(RigidBody &rb, Vector2 collisionAxis);
		void ClipSegment(Line clipLine, Vector2 sameSidePoint, LineSegment &segment);
		float GetImpulseMagnitude(std::tuple<Vector2, float> contact_point, Vector2 impulse_axis);
	};
}

