#pragma once
#pragma warning(push)
#pragma warning(disable:4251)
#include <vector>

#ifdef RGPHYSICS_EXPORTS
#define RGPHYSICS_API __declspec(dllexport)
#else
#define RGPHYSICS_API __declspec(dllimport)
#endif

namespace RGPhysics
{
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
		Vector2 operator / (float scalar);
		void operator *= (float scalar);
		Vector2 operator * (float scalar);

		void Rotate(Vector2 axis, float radians);
		float GetLength();
		void Normalize();
	};
}

#pragma warning(pop)