#pragma once
#pragma warning(push)
#pragma warning(disable:4251)
#include <vector>

#ifdef RGPHYSICS_EXPORTS
#define RGPHYSICS_API __declspec(dllexport)
#else
#define RGPHYSICS_API __declspec(dllimport)
#endif

struct RGPHYSICS_API Vector2
{
public:
	float x, y;
	Vector2(float x, float y);
	Vector2();
	
	Vector2 operator + (Vector2 otherVector);
	Vector2 operator - (Vector2 otherVector);
	void operator += (Vector2 otherVector);
	void operator -= (Vector2 otherVector);
	void operator /= (float scalar);

	void Rotate(Vector2 axis, float radians);
};

class RGPHYSICS_API Polygon
{
public:
	static std::vector<Vector2> GetRectVertices(float x, float y, float width, float height);
	std::vector<Vector2> GetVertices();
	Polygon(std::vector<Vector2> vertices);
	Vector2 GetCenterPoint();

	void Rotate(Vector2 axis, float radians);

private:
	std::vector<Vector2> vertices;
	Vector2 centerPoint;
};

#pragma warning(pop)