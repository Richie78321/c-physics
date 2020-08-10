#include "stdafx.h"
#include "Vector2.h"
#include "PolygonShape.h"
#include <tuple>
#include "LineSegment.h"
#define PI 3.1415926535

namespace RGPhysics
{
	PolygonShape::PolygonShape(std::vector<Vector2> vertices)
	{
		this->vertices = vertices;
		this->SetValues();
	}

	void PolygonShape::SetValues()
	{
		//Get centerpoint
		int numVertices = vertices.size();
		Vector2 averagePoint(0, 0);
		for (int i = 0; i < numVertices; i++) averagePoint += vertices[i];
		averagePoint /= numVertices;
		this->centerPoint = averagePoint;

		//Get radius
		float maxDist = 0;
		for (int i = 0; i < numVertices; i++)
		{
			float dist = Vector2::Distance(vertices[i], this->centerPoint);
			if (dist > maxDist) maxDist = dist;
		}
		this->radius = maxDist;
	}

	Vector2 PolygonShape::GetCenterPoint() const
	{
		return this->centerPoint;
	}

	void PolygonShape::Rotate(Vector2 axis, float radians)
	{
		int numVertices = this->vertices.size();
		for (int i = 0; i < numVertices; i++)
		{
			this->vertices[i].Rotate(axis, radians);
		}
		this->centerPoint.Rotate(axis, radians);
	}

	void PolygonShape::Translate(Vector2 translation)
	{
		int numVertices = this->vertices.size();
		for (int i = 0; i < numVertices; i++)
		{
			this->vertices[i] += translation;
		}
		this->centerPoint += translation;
	}

	void PolygonShape::TranslateTo(Vector2 position)
	{
		this->Translate(position - this->centerPoint);
	}

	PolygonShape::PolygonShape(float x, float y, float width, float height)
	{
		std::vector<Vector2> vertices;
		vertices.push_back(Vector2(x, y));
		vertices.push_back(Vector2(x + width, y));
		vertices.push_back(Vector2(x + width, y + height));
		vertices.push_back(Vector2(x, y + height));
		this->vertices = vertices;
		this->SetValues();
	}

	PolygonShape::PolygonShape(Vector2 origin, int numSides, float radius)
	{
		float angleIncrement = 2 * PI / numSides;
		float angleStart = angleIncrement / 2;

		std::vector<Vector2> vertices;
		for (int i = 0; i < numSides; i++)
		{
			float currentAngle = angleStart + (angleIncrement * i);
			vertices.push_back(Vector2(origin.x + (cos(currentAngle) * radius), origin.y + (sin(currentAngle) * radius)));
		}

		this->vertices = vertices;
		this->SetValues();
	}

	//Intersection axis is guaranteed to point from rbA to rbB -- in other words, the axis will point in the direction that rbB should move to resolve the intersection
	std::tuple<bool, Vector2, float, Vector2> PolygonShape::AreIntersecting(const PolygonShape & polygonA, const PolygonShape & polygonB)
	{
		Vector2 intersectionAxis;
		Vector2 axisOrigin;
		float intersectionDepth;
		bool hasValues = false;

		//Collect side segments
		std::vector<LineSegment> sideSegments = polygonA.GetSideSegments();
		std::vector<LineSegment> otherSideSegments = polygonB.GetSideSegments();
		sideSegments.insert(sideSegments.end(), otherSideSegments.begin(), otherSideSegments.end());

		for (int i = sideSegments.size() - 1; i >= 0; i--)
		{
			Vector2 segmentOrthagonal = sideSegments[i].GetLine().ToVector().GetOrthogonal(1);
			std::tuple<float, float> polygonAProjection = polygonA.ProjectToVector(segmentOrthagonal);
			std::tuple<float, float> polygonBProjection = polygonB.ProjectToVector(segmentOrthagonal);

			//Find overlap on axis
			std::tuple<bool, float, bool> orthagonalOverlap = Vector2::FindOverlap(std::get<0>(polygonAProjection), std::get<1>(polygonAProjection), std::get<0>(polygonBProjection), std::get<1>(polygonBProjection));
			if (std::get<0>(orthagonalOverlap))
			{
				if (!hasValues || std::get<1>(orthagonalOverlap) < intersectionDepth)
				{
					hasValues = true;
					intersectionDepth = std::get<1>(orthagonalOverlap);
					
					intersectionAxis = segmentOrthagonal;
					//Ensure axis from A to B
					if (!std::get<2>(orthagonalOverlap))
						intersectionAxis *= -1;
					
					axisOrigin = sideSegments[i].GetMidpoint();
				}
			}
			else return std::tuple<bool, Vector2, float, Vector2>(false, Vector2(), 0, Vector2());
		}

		return std::tuple<bool, Vector2, float, Vector2>(true, intersectionAxis, intersectionDepth, axisOrigin);
	}

	//Assumes that the polygon is convex
	bool PolygonShape::ContainsPoint(const PolygonShape & polygon, Vector2 point)
	{
		Line point_horizontal(0, point);

		//Check for intersections
		int num_intersections = 0;
		std::vector<LineSegment> sideSegments = polygon.GetSideSegments();
		for (int i = sideSegments.size() - 1; i >= 0; i--)
		{
			std::tuple<bool, Vector2> line_intersection = Line::FindIntersection(sideSegments[i].GetLine(), point_horizontal);
			if (std::get<0>(line_intersection) && std::get<1>(line_intersection).x <= point.x && sideSegments[i].ContainsLinePoint(std::get<1>(line_intersection)))
			{
				if (++num_intersections > 1)
					break;
			}
		}

		return num_intersections == 1;
	}

	bool PolygonShape::IsLowLevelIntersection(const PolygonShape & polygon1, const PolygonShape & polygon2)
	{
		return (polygon1.GetCenterPoint() - polygon2.GetCenterPoint()).GetLength() <= polygon1.GetRadius() + polygon2.GetRadius();
	}

	std::vector<LineSegment> PolygonShape::GetSideSegments() const
	{
		std::vector<LineSegment> sideSegments;
		std::vector<Vector2> vertices = this->GetVertices();
		int numVertices = vertices.size();
		for (int i = 0; i < numVertices; i++)
		{
			sideSegments.push_back(LineSegment(vertices[i], vertices[(i + 1) % numVertices]));
		}

		return sideSegments;
	}

	std::vector<Vector2> PolygonShape::GetVertices() const
	{
		return this->vertices;
	}

	std::string PolygonShape::ToString() const
	{
		std::string str = "";
		for (int i = this->vertices.size() - 1; i >= 0; i--)
		{
			str += this->vertices[i].ToString() + " ";
		}

		return str;
	}

	std::tuple<float, float> PolygonShape::ProjectToVector(Vector2 vector) const
	{
		std::vector<Vector2> vertices = this->GetVertices();
		float minDot = Vector2::Dot(vertices[0], vector);
		float maxDot = minDot;

		for (int i = vertices.size() - 1; i > 0; i--)
		{
			float vertexDot = Vector2::Dot(vertices[i], vector);
			if (vertexDot < minDot)
				minDot = vertexDot;
			if (vertexDot > maxDot)
				maxDot = vertexDot;
		}

		return std::tuple<float, float>(minDot, maxDot);
	}
	
	float PolygonShape::GetRadius() const
	{
		return this->radius;
	}
}