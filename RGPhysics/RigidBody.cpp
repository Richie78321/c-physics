#include "stdafx.h"
#include "Vector2.h"
#include "PolygonShape.h"
#include "SimManager.h"
#include "RigidBody.h"

#include <iostream>

namespace RGPhysics
{
	float RigidBody::GetRotationalInertia(PolygonShape * collisionPolygon, float mass)
	{
		std::vector<Vector2> vertices = collisionPolygon->GetVertices();
		Vector2 center_point = collisionPolygon->GetCenterPoint();

		float inertial_sum = 0;
		for (int i = vertices.size() - 1; i >= 0; i--)
		{
			inertial_sum += pow((vertices[i] - center_point).GetLength() / 8, 2);
		}

		inertial_sum *= mass / vertices.size();

		return inertial_sum;
	}

	RigidBody::RigidBody(PolygonShape *collisionPolygon, float mass, float COR, float COF_static, float COF_dynamic) : mass(mass), COR(COR), inv_mass(mass == 0 ? 0 : 1 / mass), inv_COR(COR == 0 ? 0 : 1 / COR), rotational_inertia(GetRotationalInertia(collisionPolygon, mass)), inv_rotational_inertia(rotational_inertia == 0 ? 0 : 1 / rotational_inertia), COF_static(COF_static), COF_dynamic(COF_dynamic)
	{
		this->angularVelocity = 0;
		this->velocity = Vector2();

		this->collisionPolygon = collisionPolygon;
	}

	Vector2 RigidBody::GetTangentialVelocity(Vector2 point) const
	{
		Vector2 radius = point - this->GetCollisionPolygon()->GetCenterPoint();
		return radius.GetOrthogonal(radius.GetLength() * this->GetAngularVelocity());
	}

	Vector2 RigidBody::GetVelocity() const
	{
		return this->velocity;
	}

	float RigidBody::GetAngularVelocity() const
	{
		return this->angularVelocity;
	}

	void RigidBody::AddVelocity(Vector2 velocity)
	{
		this->velocity += velocity;
	}

	void RigidBody::SetVelocity(Vector2 velocity)
	{
		this->velocity = velocity;
	}

	void RigidBody::AddAngularVelocity(float angularVelocity)
	{
		this->angularVelocity += angularVelocity;
	}

	void RigidBody::SetAngularVelocity(float angularVelocity)
	{
		this->angularVelocity = angularVelocity;
	}

	void RigidBody::ApplyImpulse(Vector2 impulse, Vector2 point)
	{
		if (impulse.GetLength() > 0)
		{
			float impulse_magnitude = impulse.GetLength();
			impulse.Normalize();

			Vector2 radius = this->GetCollisionPolygon()->GetCenterPoint() - point;
			radius.Normalize();

			this->AddVelocity(impulse * impulse_magnitude * Vector2::Dot(impulse, radius) * inv_mass);
			this->AddAngularVelocity(impulse_magnitude * Vector2::Cross(impulse, radius) * inv_rotational_inertia);
		}
	}

	void RigidBody::Update(SimManager &simManager)
	{
		float framePortion = simManager.GetElapsedMillis() / RGPhysics::MILLIS_TARGET;

		//Apply velocities
		this->collisionPolygon->Translate(this->GetVelocity() * framePortion);
		this->collisionPolygon->Rotate(this->collisionPolygon->GetCenterPoint(), this->GetAngularVelocity() * framePortion);
	}

	PolygonShape* RigidBody::GetCollisionPolygon() const
	{
		return this->collisionPolygon;
	}
}