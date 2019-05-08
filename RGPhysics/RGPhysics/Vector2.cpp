#include "stdafx.h"
#include "Vector2.h"

using namespace RGPhysics;

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

void Vector2::operator*= (float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
}

Vector2 Vector2::operator* (float scalar)
{
	return Vector2(this->x * scalar, this->y * scalar);
}

Vector2 Vector2::operator/ (float scalar)
{
	return Vector2(this->x / scalar, this->y / scalar);
}

float Vector2::GetLength()
{
	return sqrtf(pow(this->x, 2) + pow(this->y, 2));
}

void Vector2::Normalize()
{
	float length = this->GetLength();
	(*this) /= length;
}