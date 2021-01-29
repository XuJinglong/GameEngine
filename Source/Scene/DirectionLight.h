#pragma once
#include "EngineCommon.h"

class DirectionLight
{
public:
	DirectionLight(Vector3D dir, Color3 color) :
		m_Direction(dir),
		m_Color(color)
	{}

	~DirectionLight() {};

public:
	Vector3D m_Direction;
	Color3 m_Color;
	float m_Strength = 1.f;
};

