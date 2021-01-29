#pragma once
#include "MathDefines.h"

class Rotator
{
public:
	Rotator() {}
	Rotator(float InPitch, float InYaw, float InRoll) :
		Pitch(InPitch),
		Yaw(InYaw),
		Roll(InRoll)
	{}

	class Matrix44 GetMatrix();
	class Matrix44 GetInverseMatrix();
public:
	float Pitch = 0.f;
	float Yaw = 0.f;
	float Roll = 0.f;
};