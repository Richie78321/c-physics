#include "stdafx.h"
#include "Vector2.h"
#include <tuple>

namespace RGPhysics
{
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

	Vector2 Vector2::operator+ (Vector2 otherVector) const
	{
		Vector2 newVector(this->x, this->y);
		newVector.x += otherVector.x;
		newVector.y += otherVector.y;

		return newVector;
	}

	Vector2 Vector2::operator- (Vector2 otherVector) const
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

	Vector2 Vector2::operator* (float scalar) const
	{
		return Vector2(this->x * scalar, this->y * scalar);
	}

	Vector2 Vector2::operator/ (float scalar) const
	{
		return Vector2(this->x / scalar, this->y / scalar);
	}

	float Vector2::GetLength() const
	{
		return sqrtf(pow(this->x, 2) + pow(this->y, 2));
	}

	void Vector2::Normalize()
	{
		float length = this->GetLength();
		(*this) /= length;
	}

	Vector2 Vector2::GetOrthogonal(float magnitude) const
	{
		Vector2 orthogonal(-this->y, this->x);
		orthogonal.Normalize();
		return orthogonal * magnitude;
	}

	std::string Vector2::ToString() const
	{
		return "{ " + std::to_string(this->x) + ", " + std::to_string(this->y) + " }";
	}

	Vector2 Vector2::Cross(Vector2 vector, float scalar)
	{
		return Vector2(scalar * vector.y, -scalar * vector.x);
	}

	Vector2 Vector2::Cross(float scalar, Vector2 vector)
	{
		return Vector2(-scalar * vector.y, scalar * vector.x);
	}

	float Vector2::Cross(Vector2 vector1, Vector2 vector2)
	{
		return (vector1.x * vector2.y) - (vector1.y * vector2.x);
	}

	float Vector2::Dot(Vector2 vector1, Vector2 vector2)
	{
		return (vector1.x * vector2.x) + (vector1.y * vector2.y);
	}

	std::tuple<bool, float, bool> Vector2::FindOverlap(float min1, float max1, float min2, float max2)
	{
		std::vector<std::vector<float>> segments = { {min1, max1}, {min2, max2} };
		bool isOverlapping = false;
		bool isVectorDirection = true;
		float minOverlap = 0;

		for (int i = 0; i < 2; i++)
		{
			int nextSegment = (i + 1) % 2;
			for (int j = 0; j < 2; j++)
			{
				if (segments[i][j] > segments[nextSegment][0] && segments[i][j] < segments[nextSegment][1])
				{
					//Contained in other segment
					float overlapDepth = abs(segments[i][j] - segments[nextSegment][(j + 1) % 2]);
					if (!isOverlapping || overlapDepth < minOverlap)
					{
						//Set as min overlapping
						isOverlapping = true;
						minOverlap = overlapDepth;

						//Get direction (relative to first)
						isVectorDirection = (i == 1) != (j == 0);
					}
				}
			}
		}

		return std::tuple<bool, float, bool>(isOverlapping, minOverlap, isVectorDirection);
	}

	float Vector2::Distance(Vector2 p1, Vector2 p2)
	{
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	}
}