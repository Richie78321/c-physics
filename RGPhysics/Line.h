#pragma once
#include <tuple>

#ifdef RGPHYSICS_EXPORTS
#define RGPHYSICS_API __declspec(dllexport)
#else
#define RGPHYSICS_API __declspec(dllimport)
#endif

namespace RGPhysics
{
	struct Vector2;

	struct RGPHYSICS_API Line
	{
	public:
		static std::tuple<bool, Vector2> FindIntersection(Line line1, Line line2);
		static bool IsSameSide(Line line, Vector2 samplePoint, Vector2 testPoint);

		Line(float slope, float yIntercept);
		Line(float slope, Vector2 point);
		Line(Vector2 point1, Vector2 point2);
		Line(float A, float B, float C);

		std::tuple<bool, Vector2> PointAtX(float x) const;
		std::tuple<bool, Vector2> PointAtY(float y) const;
		Vector2 ToVector() const;
	private:
		float A;
		float B;
		float C;
	};
}

