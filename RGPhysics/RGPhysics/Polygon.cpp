#include "stdafx.h"
#include "Vector2.h"
#include "Polygon.h"

Polygon::Polygon(std::vector<Vector2> vertices)
{
	this->vertices = vertices;

	int numVertices = vertices.size();
	Vector2 averagePoint(0, 0);
	for (int i = 0; i < numVertices; i++) averagePoint += vertices[i];
	averagePoint /= numVertices;
	this->centerPoint = averagePoint;
}

Vector2 Polygon::GetCenterPoint()
{
	return this->centerPoint;
}

void Polygon::Rotate(Vector2 axis, float radians)
{
	int numVertices = this->vertices.size();
	for (int i = 0; i < numVertices; i++)
	{
		this->vertices[i].Rotate(axis, radians);
	}
	this->centerPoint.Rotate(axis, radians);
}

void Polygon::Translate(Vector2 translation)
{
	int numVertices = this->vertices.size();
	for (int i = 0; i < numVertices; i++)
	{
		this->vertices[i] += translation;
	}
	this->centerPoint += translation;
}

void Polygon::TranslateTo(Vector2 position)
{
	this->Translate(position - this->centerPoint);
}

std::vector<Vector2> Polygon::GetRectVertices(float x, float y, float width, float height)
{
	std::vector<Vector2> vertices;
	vertices.push_back(Vector2(x, y));
	vertices.push_back(Vector2(x + width, y));
	vertices.push_back(Vector2(x + width, y + height));
	vertices.push_back(Vector2(x, y + height));

	return vertices;
}

std::vector<Vector2> Polygon::GetVertices()
{
	return this->vertices;
}