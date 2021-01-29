#pragma once
#include "MathDefines.h"
#include "Vector3D.h"
#include "Rotator.h"
#include "Matrix44.h"

class Transform 
{
public:
	Transform()
	{
	}

	Transform(const Vector3D& InLoc, const Rotator& InRot )
	{
		Loc = InLoc;
		Rot = InRot;
	}

	~Transform() {}

	Matrix44 GetMatrix();

public:
	Vector3D Loc;
	Rotator Rot;
};