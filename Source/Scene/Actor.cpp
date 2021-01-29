#include "Actor.h"
#include "Render/MeshDrawCommand.h"
#include "Material/TextureArray.h"
#include "Material/Texture2D.h"

void Actor::SetLocation(Vector3D location)
{
	m_Location = location;
}

void Actor::SetRotation(Vector3D rotation)
{
	m_Rotation = Rotator(rotation.X, rotation.Y, rotation.Z);
}

MeshDrawCommand* Actor::GetMeshDrawCommand()
{
	uint8_t* VertexsBuffer;
	uint32_t VBSize = 0;
	uint8_t* IndexBuffer;
	uint32_t IBSize = 0;
	Mesh->Mesh->GetVertexData(&VertexsBuffer, VBSize, &IndexBuffer, IBSize);
	auto material = Mesh->m_Material;
	CMap<int, RHIResourceRef*> textures;
	textures[0] = material->m_pTextureArray->m_pTextureArrayView;
	for (int i = 0; i < material->m_Textures.size(); i++)
	{
		textures[i+1] = material->m_Textures[i]->m_pTextureView;
	}

	MeshDrawCommand* cmd = new MeshDrawCommand(
		material->m_pVSShader, 
		material->m_pPSShader, 
		Mesh->m_pVertexLayout, 
		material->m_pTextureArray->m_pSamplerState, 
		textures,
		material->m_pMaterialParamsConstBuffer,
		GetWorldMatrix(),
		VertexsBuffer, 
		VBSize, 
		IndexBuffer, 
		IBSize);
	return cmd;
}
