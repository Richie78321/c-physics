#pragma once
#include <vector>
#include "Line.h"
#include "Vector2.h"

#ifdef RGPHYSICS_EXPORTS
#define RGPHYSICS_API __declspec(dllexport)
#else
#define RGPHYSICS_API __declspec(dllimport)
#endif

namespace RGPhysics
{
	class RGPHYSICS_API LineSegment
	{
	public:
		LineSegment(Vector2 endPoint1, Vector2 endPoint2);
		LineSegment();
		
		bool ContainsLinePoint(Vector2 pointOnLine) const;
		Vector2 GetMidpoint() const;
		std::vector<Vector2> GetEndPoints() const;
		void SetEndPoints(Vector2 endPoint1, Vector2 endPoint2);
		Line GetLine() const;
	private:
		Vector2 endPoint1;
		Vector2 endPoint2;
		Line line;
	};

}
