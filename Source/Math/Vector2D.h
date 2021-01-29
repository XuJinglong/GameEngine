#pragma once
#include "MathDefines.h"

class Vector2D
{
public:
	Vector2D() : Vector2D(0, 0) {}
	Vector2D(float XIn, float YIn) : X(XIn), Y(YIn) {}

	bool operator==(const Vector2D& Other)
	{
		return X == Other.X && Y == Other.Y;
	}
public:
	float X;
	float Y;
};

typedef Vector2D Point2D;