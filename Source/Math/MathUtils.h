#pragma once
#include "MathCommon.h"

class MathUtils
{
public:
	static float LineLerp(float Low, float High, float Alpha)
	{
		Alpha = Alpha < 0 ? 0 : Alpha > 1 ? 1 : Alpha;
		return Low + Alpha * (High - Low);
	}

	static float Clamp(float InVar, float Min, float Max)
	{
		return InVar < Min ? Min : InVar > Max ? Max : InVar;
	}
};