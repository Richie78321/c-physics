#pragma once

#ifdef RGPHYSICS_EXPORTS
#define RGPHYSICS_API __declspec(dllexport)
#else
#define RGPHYSICS_API __declspec(dllimport)
#endif

namespace RGPhysics
{
	class Polygon;
	struct Vector2;

	const int FRAME_TARGET = 90;
	const float MILLIS_TARGET = 1.0 / FRAME_TARGET;

	class RGPHYSICS_API RigidBody
	{
	public:
		RigidBody(Polygon *collisionPolygon);
		void AddVelocity(Vector2 velocity);
		void SetVelocity(Vector2 velocity);
		void AddAngularVelocity(float angularVelocity);
		void SetAngularVelocity(float angularVelocity);

		Vector2 GetVelocity();
		float GetAngularVelocity();

		void Update(unsigned int elapsedMillis);
	private:
		Polygon *collisionPolygon;
		Vector2 velocity;
		float angularVelocity;
	};
}