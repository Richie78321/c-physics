#pragma once
#include "SimElement.h"
#include "Vector2.h"

#ifdef RGPHYSICS_EXPORTS
#define RGPHYSICS_API __declspec(dllexport)
#else
#define RGPHYSICS_API __declspec(dllimport)
#endif

namespace RGPhysics
{
	class PolygonShape;
	struct Vector2;

	const int FRAME_TARGET = 90;
	const float MILLIS_TARGET = 1000.0f / FRAME_TARGET;

	class RGPHYSICS_API RigidBody : public SimElement
	{
	public:
		static float GetRotationalInertia(PolygonShape *collisionPolygon, float mass);

		RigidBody(PolygonShape *collisionPolygon, float mass, float COR, float COF_static, float COF_dynamic);
		void AddVelocity(Vector2 velocity);
		void SetVelocity(Vector2 velocity);
		void AddAngularVelocity(float angularVelocity);
		void SetAngularVelocity(float angularVelocity);
		void ApplyImpulse(Vector2 impulse, Vector2 point);

		Vector2 GetTangentialVelocity(Vector2 point) const;
		Vector2 GetVelocity() const;
		float GetAngularVelocity() const;
		PolygonShape* GetCollisionPolygon() const;

		void Update(SimManager &simManager);

		const float mass;
		const float COR;
		const float COF_static;
		const float COF_dynamic;
		const float inv_mass;
		const float inv_COR;
		const float rotational_inertia;
		const float inv_rotational_inertia;

	private:
		PolygonShape *collisionPolygon;
		Vector2 velocity;
		float angularVelocity;
	};
}