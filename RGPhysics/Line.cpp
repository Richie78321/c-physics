#include "stdafx.h"
#include "Line.h"
#include "Vector2.h"
#include <iostream>

namespace RGPhysics
{
	std::tuple<bool, Vector2> Line::FindIntersection(Line line1, Line line2)
	{
		float det = line1.A * line2.B - line2.A * line1.B;
		if (det == 0)
		{
			//Lines are parallel
			return std::tuple<bool, Vector2>(false, Vector2());
		}
		else
		{
			float x = (line2.B * line1.C - line1.B * line2.C) / det;
			float y = (line1.A * line2.C - line2.A * line1.C) / det;
			return std::tuple<bool, Vector2>(true, Vector2(x, y));
		}
	}

	bool Line::IsSameSide(Line line, Vector2 samplePoint, Vector2 testPoint)
	{
		Vector2 linePoint;
		std::tuple<bool, Vector2> result = line.PointAtX(0);
		if (!std::get<0>(result))
		{
			result = line.PointAtY(0);
		}
		linePoint = std::get<1>(result);

		Vector2 lineVector = line.ToVector();
		float sampleCross = Vector2::Cross(lineVector, samplePoint - linePoint);
		float testCross = Vector2::Cross(lineVector, testPoint - linePoint);

		return (sampleCross < 0 && testCross <= 0) || (sampleCross > 0 && testCross >= 0);
	}

	Line::Line(float slope, float yIntercept)
	{
		Vector2 p1(0, yIntercept);
		Vector2 p2 = p1 + Vector2(1, slope);

		this->A = p2.y - p1.y;
		this->B = p1.x - p2.x;
		this->C = this->B * yIntercept;
	}

	Line::Line(float slope, Vector2 point)
	{
		Vector2 p2 = point + Vector2(1, slope);

		this->A = p2.y - point.y;
		this->B = point.x - p2.x;
		this->C = (this->A * point.x) + (this->B * point.y);
	}

	Line::Line(Vector2 point1, Vector2 point2)
	{
		this->A = point2.y - point1.y;
		this->B = point1.x - point2.x;
		this->C = (this->A * point1.x) + (this->B * point1.y);
	}

	Line::Line(float A, float B, float C)
	{
		this->A = A;
		this->B = B;
		this->C = C;
	}

	std::tuple<bool, Vector2> Line::PointAtX(float x) const
	{
		if (this->B == 0)
		{
			return std::tuple<bool, Vector2>(false, Vector2());
		}
		else
		{
			Vector2 point(x, (this->C - (this->A * x)) / this->B);
			return std::tuple<bool, Vector2>(true, point);
		}
	}

	std::tuple<bool, Vector2> Line::PointAtY(float y) const
	{
		if (this->A == 0)
		{
			return std::tuple<bool, Vector2>(false, Vector2());
		}
		else
		{
			Vector2 point((this->C - (this->B * y)) / this->A, y);
			return std::tuple<bool, Vector2>(true, point);
		}
	}

	Vector2 Line::ToVector() const
	{
		return Vector2(this->B, -this->A);
	}

	/*bool Line::IsOnLine(Vector2 point, float leniency) const
	{
		Vector2 linePoint;
		std::tuple<bool, Vector2> result = this->PointAtX(0);
		if (!std::get<0>(result))
		{
			result = this->PointAtY(0);
		}
		linePoint = std::get<1>(result);

		Vector2 lineVector = this->ToVector();
		return abs(Vector2::Cross(lineVector, point - linePoint)) <= leniency;
	}*/
}