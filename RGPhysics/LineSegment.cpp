#include "stdafx.h"
#include "LineSegment.h"
#include "Vector2.h"
#include "Line.h"

namespace RGPhysics
{
	LineSegment::LineSegment(Vector2 endPoint1, Vector2 endPoint2) : line(Line(endPoint1, endPoint2))
	{
		this->endPoint1 = endPoint1;
		this->endPoint2 = endPoint2;
	}

	LineSegment::LineSegment() : line(Line(Vector2(0, 0), Vector2(1, 1)))
	{
		this->endPoint1 = Vector2(0, 0);
		this->endPoint2 = Vector2(1, 1);
	}

	bool LineSegment::ContainsLinePoint(Vector2 pointOnLine) const
	{
		std::vector<Vector2> endPoints = GetEndPoints();
		float minX = endPoints[0].x;
		float maxX = endPoints[0].x;
		float minY = endPoints[0].y;
		float maxY = endPoints[0].y;

		if (endPoints[1].x < minX)
			minX = endPoints[1].x;
		if (endPoints[1].x > maxX)
			maxX = endPoints[1].x;
		if (endPoints[1].y < minY)
			minY = endPoints[1].y;
		if (endPoints[1].y > maxY)
			maxY = endPoints[1].y;

		return pointOnLine.x >= minX && pointOnLine.x <= maxX && pointOnLine.y >= minY && pointOnLine.y <= maxY;
	}

	Vector2 LineSegment::GetMidpoint() const
	{
		std::vector<Vector2> endPoints = this->GetEndPoints();
		return (endPoints[0] + endPoints[1]) / 2;
	}

	std::vector<Vector2> LineSegment::GetEndPoints() const
	{
		return std::vector<Vector2>{ this->endPoint1, this->endPoint2 };
	}

	void LineSegment::SetEndPoints(Vector2 endPoint1, Vector2 endPoint2)
	{
		this->endPoint1 = endPoint1;
		this->endPoint2 = endPoint2;
	}

	Line LineSegment::GetLine() const
	{
		return this->line;
	}
}
