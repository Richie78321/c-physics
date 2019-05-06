// RGPhysics.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RGPhysics.h"

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2()
{
	this->x = 0;
	this->y = 0;
}

void Vector2::Rotate(Vector2 axis, float radians)
{
	float xRelative = this->x - axis.x;
	float yRelative = this->y - axis.y;

	float cosRads = cos(radians);
	float sinRads = sin(radians);

	float newX = (xRelative * cosRads) - (yRelative * sinRads);
	float newY = (xRelative * sinRads) + (yRelative * cosRads);

	this->x = newX + axis.x;
	this->y = newY + axis.y;
}

Vector2 Vector2::operator+ (Vector2 otherVector)
{
	Vector2 newVector(this->x, this->y);
	newVector.x += otherVector.x;
	newVector.y += otherVector.y;

	return newVector;
}

Vector2 Vector2::operator- (Vector2 otherVector)
{
	Vector2 newVector(this->x, this->y);
	newVector.x -= otherVector.x;
	newVector.y -= otherVector.y;

	return newVector;
}

void Vector2::operator+= (Vector2 otherVector)
{
	this->x += otherVector.x;
	this->y += otherVector.y;
}

void Vector2::operator-= (Vector2 otherVector)
{
	this->x -= otherVector.x;
	this->y -= otherVector.y;
}

void Vector2::operator/= (float scalar)
{
	this->x /= scalar;
	this->y /= scalar;
}

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