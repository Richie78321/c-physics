#include "stdafx.h"
#include "Vector2.h"

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