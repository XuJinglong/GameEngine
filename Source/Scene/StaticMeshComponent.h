#pragma once
#include "EngineCommon.h"
#include "Engine/StaticMesh.h"
#include "Material/Material.h"
#include "RHI/IRHI.h"

class StaticMeshComponent
{
public:
	StaticMeshComponent(FString meshName, FString materialName);
	~StaticMeshComponent() {}

public:

public:
	StaticMesh* Mesh;
	Material* m_Material = nullptr;
	RHIResourceRef* m_pVertexLayout = nullptr;
};
