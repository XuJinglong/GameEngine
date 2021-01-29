#pragma once
#include "EngineCommon.h"
#include "Shader/Shader.h"

class SkySphere
{
public:
	SkySphere();
	~SkySphere() {}

	void Draw();

private:
	class StaticMesh* Mesh;
	RHIResourceRef* m_pVertexLayout = nullptr;

	//shader
	Shader* m_pVSShader = nullptr;
	Shader* m_pPSShader = nullptr;

	//texture cube
	RHIResourceRef* m_pTextrueCube = nullptr;
	RHIResourceRef* m_pTextrueCubeView = nullptr;
	RHIResourceRef*  m_pSamplerState = nullptr;

	//pso
	RHIResourceRef* m_pDepthStencileState = nullptr;
};

