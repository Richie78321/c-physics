#include "stdafx.h"
#include "Vector2.h"
#include "Polygon.h"
#include "RigidBody.h"

using namespace RGPhysics;

RigidBody::RigidBody(Polygon *collisionPolygon)
{
	this->collisionPolygon = collisionPolygon;
}

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

void RigidBody::Update(unsigned int elapsedMillis)
{
	float framePortion = elapsedMillis / RGPhysics::FRAME_TARGET;

	//Add velocities
	this->AddVelocity(this->GetVelocity() * framePortion);
	this->AddAngularVelocity(this->GetAngularVelocity() * framePortion);
}