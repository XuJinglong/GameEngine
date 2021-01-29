#pragma once
#include "EngineCommon.h"

class PointLight
{
public:
	PointLight(Vector3D pos, Color3 color) :
	m_Position(pos),
	m_Color(color)
	{}

	~PointLight() {};

public:
	Vector3D m_Position;
	Color3 m_Color;
	float m_Strength = 1.f;
};

