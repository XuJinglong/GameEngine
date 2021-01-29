#pragma once
#include "EngineCommon.h"
#include "Material/Material.h"

class MeshDrawCommand
{
public:
	MeshDrawCommand(Shader* VSShader, Shader* PSShader, RHIResourceRef* Layout, RHIResourceRef* SamplerState, CMap<int, RHIResourceRef*>& Textures, RHIResourceRef* MaterialParams, Matrix44 WorldMatrix, uint8_t* VertexBuffer, uint32_t VertexSize, uint8_t* IndexBuffer, uint32_t IndexSize) :
		m_pVSShader(VSShader),
		m_pPSShader(PSShader),
		m_pVertexLayoutRef(Layout),
		m_pVertexBuffer(VertexBuffer),
		m_VertexSize(VertexSize),
		m_pIndexBuffer(IndexBuffer),
		m_IndexSize(IndexSize),
		m_pMaterialParamsConstBuffer(MaterialParams),
		m_pSamplerState(SamplerState),
		m_WorldMatrix(WorldMatrix)
	{
		swap(m_Textures, Textures);
	}

	~MeshDrawCommand() {}

	void Execute();

private:
	Shader* m_pVSShader = nullptr;
	Shader* m_pPSShader = nullptr;

	RHIResourceRef* m_pVertexLayoutRef = nullptr;
	CMap<int, RHIResourceRef*> m_Textures;
	RHIResourceRef* m_pSamplerState = nullptr;
	RHIResourceRef* m_pMaterialParamsConstBuffer = nullptr;

	uint8_t* m_pVertexBuffer = nullptr;
	uint32_t m_VertexSize = 0;
	uint8_t* m_pIndexBuffer = nullptr;
	uint32_t m_IndexSize = 0;

	Matrix44 m_WorldMatrix;
};

