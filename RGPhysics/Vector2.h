#pragma once
#pragma warning(push)
#pragma warning(disable:4251)
#include <vector>
#include <string>

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
		static Vector2 Cross(Vector2 vector, float scalar);
		static Vector2 Cross(float scalar, Vector2 vector);
		static float Cross(Vector2 vector1, Vector2 vector2);
		static float Dot(Vector2 vector1, Vector2 vector2);
		
		static std::tuple<bool, float, bool> FindOverlap(float min1, float max1, float min2, float max2);

		static float Distance(Vector2 p1, Vector2 p2);

		float x, y;
		Vector2(float x, float y);
		Vector2();

		Vector2 operator + (Vector2 otherVector) const;
		Vector2 operator - (Vector2 otherVector) const;
		void operator += (Vector2 otherVector);
		void operator -= (Vector2 otherVector);
		void operator /= (float scalar);
		Vector2 operator / (float scalar) const;
		void operator *= (float scalar);
		Vector2 operator * (float scalar) const;

		void Rotate(Vector2 axis, float radians);
		float GetLength() const;
		void Normalize();

		Vector2 GetOrthogonal(float magnitude) const;

		std::string ToString() const;
	};
}

#pragma warning(pop)