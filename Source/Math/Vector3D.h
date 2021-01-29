#pragma once
#include "MathDefines.h"

class Vector3D
{
public:
	Vector3D() : Vector3D(0, 0, 0) {}
	Vector3D(float XIn, float YIn, float ZIn) :
		X(XIn),
		Y(YIn),
		Z(ZIn)
	{
	}

	~Vector3D() {}

	class Matrix44 GetMatrix();

	float Length()
	{
		return sqrtf(X * X + Y * Y + Z * Z);
	}

	Vector3D Normalize()
	{
		float Size = Length();
		return Vector3D(X / Size, Y / Size, Z / Size);
	}

	Vector3D operator- (const Vector3D& Other) const
	{
		return Vector3D(X - Other.X, Y - Other.Y, Z - Other.Z);
	}

	Vector3D operator- () const
	{
		return Vector3D(-X, -Y, -Z);
	}

	Vector3D operator+ (const Vector3D& Other) const
	{
		return Vector3D(X + Other.X, Y + Other.Y, Z + Other.Z);
	}

	float Dot(const Vector3D& Other) const
	{
		return X * Other.X + Y * Other.Y + Z * Other.Z;
	}

	Vector3D Cross(const Vector3D& Other) const
	{
		return Vector3D(
			Y * Other.Z - Other.Y * Z,
			Other.X * Z - X * Other.Z,
			X * Other.Y - Other.X * Y
		);
	}

	Vector3D operator* (float Num) const
	{
		return Vector3D(X * Num, Y * Num, Z * Num);
	}
public:
	float X;
	float Y;
	float Z;
};

typedef Vector3D Point3D;
typedef Vector3D Color3;