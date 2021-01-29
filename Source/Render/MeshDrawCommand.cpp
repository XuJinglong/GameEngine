#include "MeshDrawCommand.h"
#include "Scene/Camera.h"
#include "Scene/World.h"
extern IRHI* g_pRHI;
extern Camera* DefaultCamera;
void MeshDrawCommand::Execute()
{
	g_pRHI->PSSetConstantBuffers(3, 1, m_pMaterialParamsConstBuffer);
	g_pRHI->VSSetConstantBuffers(3, 1, m_pMaterialParamsConstBuffer);

	DefaultCamera->MVPData.World = m_WorldMatrix;
	DefaultCamera->MVPData.View = DefaultCamera->GetCameraInverseMatrix();
	DefaultCamera->MVPData.WorldForNormal = m_WorldMatrix;//TODO:专门的法线矩阵
	DefaultCamera->MVPData.WorldForNormal[3][0] = 0;
	DefaultCamera->MVPData.WorldForNormal[3][1] = 0;
	DefaultCamera->MVPData.WorldForNormal[3][2] = 0;
	DefaultCamera->MVPData.CamPos = DefaultCamera->Location;
	g_pRHI->UpdateConstBuffer(&DefaultCamera->MVPData, sizeof(DefaultCamera->MVPData), DefaultCamera->m_pMVPConstBuffer);

	g_pRHI->SetVSShader(m_pVSShader->m_pShader);
	g_pRHI->SetPSShader(m_pPSShader->m_pShader);
	g_pRHI->SetVertexLayout(m_pVertexLayoutRef);
	g_pRHI->SetSamplerState(m_pSamplerState, 0);
	for (auto it = m_Textures.begin(); it != m_Textures.end(); it++) 
	{
		g_pRHI->SetShaderResource(it->second, it->first);
	}

	g_pRHI->DrawIndex(m_pVertexBuffer, m_VertexSize, m_pIndexBuffer, m_IndexSize);
}
