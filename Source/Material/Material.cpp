#include "Material.h"
#include "Third/nlohmann_json/json.hpp"
#include "FileSystem/FileUtils.h"
#include "RHI/IRHI.h"
#include "TextureArray.h"
#include "Texture2D.h"

using json = nlohmann::json;
#define JSON2VECTOR3(array) Vector3D(array[0], array[1], array[2])

extern IRHI* g_pRHI;

Material::Material(FString MatFile)
{
	m_MatFile = MatFile;
	Compile();
	InitResources();
}

void Material::Compile()
{
	//���ز��������ļ�
	std::fstream matFile(m_MatFile);
	if (!matFile)
	{
		return;
	}

	json matConfig = json::parse(matFile);
	if (matConfig.is_discarded())
	{
		return;
	}

	//�����ļ���ʱ��ֵ�ԣ�����һ���Զ������
	//��ȡ���������ɴ���, ���template
	//��ʱ�����ɴ���
	m_MaterialParams.texIndexBaseColor = 0;
	m_MaterialParams.texIndexNormal = 1;
	m_MaterialParams.emissiveColor = JSON2VECTOR3(matConfig["emissive_color"]);
	m_MaterialParams.metallic = matConfig["metallic"];
	m_MaterialParams.specular = matConfig["specular"];
	m_MaterialParams.roughness = matConfig["roughness"];

	m_TextureArrayFiles.push_back(matConfig["base_color"]);
	m_TextureArrayFiles.push_back(matConfig["normal"]);
	m_TextureFiles.push_back(matConfig["base_color"]);
	m_TextureFiles.push_back(matConfig["normal"]);
	
	//�������ü���hlsl�꣬������ɫ����������_GUID.blob����
	m_pVSShader = new Shader("vs_forward", Shader::ShaderType::VS);
	m_pPSShader = new Shader("ps_forward", Shader::ShaderType::PS);
}

void Material::InitResources()
{
	//�������л���GUID������blob��������ɫ��
	//������ͼ������������
	m_pTextureArray = new TextureArray(m_TextureFiles);
	for (int i = 0; i < m_TextureFiles.size() && i < 16; i++) 
	{
		m_Textures.push_back(new Texture2D(m_TextureFiles[i], i+1));
	}
	//�������ڲ�����Constbuffer��ÿ��matrerial����һ��,��Ӧtemplate���Զ����ɵ�CBufferForMaterial�����shader���幫�ã���MeshDrawCommand����ÿ��shader���嵥����ֵ���󶨵���ɫ��
	g_pRHI->CreateConstBuffer(&m_MaterialParams,  sizeof(m_MaterialParams), &m_pMaterialParamsConstBuffer);
}

void Material::SetFloatParams()
{
}
