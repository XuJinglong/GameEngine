#pragma once
#include "EngineCommon.h"
#include "RHI/IRHI.h"

class World
{
public:
	World() 
	{
	}
	~World() 
	{
	}

	void Build(FArray<FString> levels);

	void Tick();

	void InitRHIResources();
public:
	//levels
	//FArray<class Map*> m_Levels;

	//actors
	FArray<class Actor*> m_Actors;

	//sky
	class SkySphere* m_pSky = nullptr;

	//create light const buffer
	struct PointLightInfo
	{
		Vector4D Color;
		Vector4D Position;
	};

	struct DirectionLightInfo
	{
		Vector4D Color;
		Vector4D Direction;
	};

	struct WorldBufferInfo 
	{
		PointLightInfo PointLights[3];
		DirectionLightInfo DirectionLight;
		Vector4D Time;
	};
	//lights
	class DirectionLight* m_pDirectionLight = nullptr;
	FArray<class PointLight*> m_PointLights;

	WorldBufferInfo m_WorldBufferInfo;
	RHIResourceRef* m_pWorldConstBuffer = nullptr;

	RHIResourceRef* m_pDepthStencileState = nullptr;
};

