#include "StaticMeshComponent.h"
extern IRHI* g_pRHI;
StaticMeshComponent::StaticMeshComponent(FString meshName, FString materialName)
{
	Mesh = new StaticMesh();
	if (meshName == "") 
	{
		meshName = "Content/ball.fbx";
	}
	Mesh->LoadFromFile(meshName.c_str());
	if (materialName == "") 
	{
		materialName = "Content/M_Test.json";
	}
	m_Material = new Material(materialName);
	g_pRHI->CreateVertexLayout(m_Material->m_pVSShader->m_pShaderBlob, &m_pVertexLayout);
}
