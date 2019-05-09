#include "stdafx.h"
#include "SimManager.h"
#include "Vector2.h"
#include "Polygon.h"
#include "RigidBody.h"

using namespace RGPhysics;

Vector2 RigidBody::GetVelocity()
{
	return this->velocity;
}

float RigidBody::GetAngularVelocity()
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

void RigidBody::Update(const SimManager &simManager)
{
	float framePortion = simManager.GetElapsedMillis() / RGPhysics::FRAME_TARGET;

	//Add velocities
	this->AddVelocity(this->GetVelocity() * framePortion);
	this->AddAngularVelocity(this->GetAngularVelocity() * framePortion);
}