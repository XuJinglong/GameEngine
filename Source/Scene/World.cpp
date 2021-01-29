#include "World.h"
#include "Map.h"
#include "Actor.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "time.h"
#include "SkySphere.h"

extern World* GWorld = new World();
extern IRHI* g_pRHI;

void World::Build(FArray<FString> levels)
{
	for (const auto& file : levels) 
	{
		Map* map = new Map(file);
		map->Build(this);
		delete map;
	}
	m_pSky = new SkySphere();
	InitRHIResources();
}

void World::Tick()
{
	m_WorldBufferInfo.Time = Vector4D(time(nullptr), 0, 0, 0);
	if (m_pDirectionLight)
	{
		m_WorldBufferInfo.DirectionLight.Color = m_pDirectionLight->m_Color;
		m_WorldBufferInfo.DirectionLight.Direction = m_pDirectionLight->m_Direction;
	}
	for (int i = 0; i < m_PointLights.size() && i < 5; i++)
	{
		m_WorldBufferInfo.PointLights[i].Color = m_PointLights[i]->m_Color;
		m_WorldBufferInfo.PointLights[i].Position = m_PointLights[i]->m_Position;
	}
}

void World::InitRHIResources()
{
	m_WorldBufferInfo.Time = Vector4D(time(nullptr), 0, 0, 0);
	if (m_pDirectionLight) 
	{
		m_WorldBufferInfo.DirectionLight.Color = m_pDirectionLight->m_Color;
		m_WorldBufferInfo.DirectionLight.Direction = m_pDirectionLight->m_Direction;
	}
	for (int i = 0; i < m_PointLights.size() && i < 3; i++) 
	{
		m_WorldBufferInfo.PointLights[i].Color = m_PointLights[i]->m_Color;
		m_WorldBufferInfo.PointLights[i].Position = m_PointLights[i]->m_Position;
	}
	g_pRHI->CreateConstBuffer(&m_WorldBufferInfo, sizeof(m_WorldBufferInfo), &m_pWorldConstBuffer);

	g_pRHI->CreateNormalDepthStencilState(&m_pDepthStencileState);
}
