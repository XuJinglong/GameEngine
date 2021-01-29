#pragma once
#include "MathDefines.h"

class Vector4D
{
public:
	Vector4D(float XIn, float YIn, float ZIn, float WIn = 1) :
		X(XIn),
		Y(YIn),
		Z(ZIn),
		W(WIn)
	{
	}

	Vector4D() {};

	Vector4D operator* (class Matrix44 M);

	Vector4D& operator= (const class Vector3D& v3);
public:
	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
	float W = 0.f;
};