#pragma once
#include <vector>
#include <string>

#ifdef RGPHYSICS_EXPORTS
#define RGPHYSICS_API __declspec(dllexport)
#else
#define RGPHYSICS_API __declspec(dllimport)
#endif

namespace RGPhysics
{
	struct Vector2;
	class LineSegment;

	class RGPHYSICS_API PolygonShape
	{
	public:
		static std::tuple<bool, Vector2, float, Vector2> AreIntersecting(const PolygonShape &polygon1, const PolygonShape &polygon2);
		static bool ContainsPoint(const PolygonShape &polygon, Vector2 point);
		static bool IsLowLevelIntersection(const PolygonShape &polygon1, const PolygonShape &polygon2);

		std::vector<Vector2> GetVertices() const;
		std::vector<LineSegment> GetSideSegments() const;

		PolygonShape(std::vector<Vector2> vertices);
		PolygonShape(float x, float y, float width, float height);
		PolygonShape(Vector2 origin, int numSides, float radius);
		Vector2 GetCenterPoint() const;

		void Rotate(Vector2 axis, float radians);
		void Translate(Vector2 translation);
		void TranslateTo(Vector2 position);

		float GetRadius() const;

		std::string ToString() const;
		std::tuple<float, float> ProjectToVector(Vector2 vector) const;

	private:
		float radius;
		std::vector<Vector2> vertices;
		Vector2 centerPoint;
		void SetValues();
	};
}