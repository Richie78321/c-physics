#pragma once

#ifdef RGPHYSICS_EXPORTS
#define RGPHYSICS_API __declspec(dllexport)
#else
#define RGPHYSICS_API __declspec(dllimport)
#endif

class B;
class Polygon;

class RGPHYSICS_API RigidBody
{
public:
	RigidBody();
private:
	Polygon &polygon;
};