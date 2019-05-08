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
	struct Vector2;

	class RGPHYSICS_API Polygon
	{
	public:
		static std::vector<Vector2> GetRectVertices(float x, float y, float width, float height);
		std::vector<Vector2> GetVertices();
		Polygon(std::vector<Vector2> vertices);
		Vector2 GetCenterPoint();

		void Rotate(Vector2 axis, float radians);
		void Translate(Vector2 translation);
		void TranslateTo(Vector2 position);

	private:
		std::vector<Vector2> vertices;
		Vector2 centerPoint;
	};
}

#pragma warning(pop)