#include "SkySphere.h"
#include "Engine/StaticMesh.h"
#include "Scene/Camera.h"
#include "Scene/World.h"

extern Camera* DefaultCamera;

SkySphere::SkySphere()
{
	Mesh = new StaticMesh();
	Mesh->LoadFromFile("Content/ball.fbx");

	m_pVSShader = new Shader("vs_sky", Shader::ShaderType::VS);
	m_pPSShader = new Shader("ps_sky", Shader::ShaderType::PS);

	g_pRHI->CreateVertexLayout(m_pVSShader->m_pShaderBlob, &m_pVertexLayout);

	FArray<FString> cubeFiles{"Content/skybox/right.jpg.png", "Content/skybox/left.jpg.png" , "Content/skybox/top.jpg.png" , "Content/skybox/bottom.jpg.png" , "Content/skybox/front.jpg.png" , "Content/skybox/back.jpg.png" };
	//FArray<FString> cubeFiles(6, "Content/awesomeface.png");
	g_pRHI->CreateTextureCube(cubeFiles , &m_pTextrueCube , &m_pTextrueCubeView);
	g_pRHI->CreateSamplerState(&m_pSamplerState);

	g_pRHI->CreateSkyDepthStencilState(&m_pDepthStencileState);
}

void SkySphere::Draw()
{
	uint8_t* VertexsBuffer;
	uint32_t VBSize = 0;
	uint8_t* IndexBuffer;
	uint32_t IBSize = 0;
	Mesh->GetVertexData(&VertexsBuffer, VBSize, &IndexBuffer, IBSize);

	DefaultCamera->MVPData.World = Matrix44::GetIdentity();
	DefaultCamera->MVPData.View = DefaultCamera->GetSkyBoxCameraViewMatrix();
	g_pRHI->UpdateConstBuffer(&DefaultCamera->MVPData, sizeof(DefaultCamera->MVPData), DefaultCamera->m_pMVPConstBuffer);

	g_pRHI->SetVSShader(m_pVSShader->m_pShader);
	g_pRHI->SetPSShader(m_pPSShader->m_pShader);
	g_pRHI->SetVertexLayout(m_pVertexLayout);

	g_pRHI->SetSamplerState(m_pSamplerState, 0);
	g_pRHI->SetShaderResource(m_pTextrueCubeView, 0);

	g_pRHI->SetDepthStencilState(m_pDepthStencileState);

	g_pRHI->DrawIndex(VertexsBuffer, VBSize, IndexBuffer, IBSize);
}
