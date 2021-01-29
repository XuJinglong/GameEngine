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
	//加载材质配置文件
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

	//配置文件暂时键值对，额外一段自定义代码
	//提取参数，生成代码, 填充template
	//暂时不生成代码
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
	
	//根据配置加入hlsl宏，编译着色器，材质名_GUID.blob保存
	m_pVSShader = new Shader("vs_forward", Shader::ShaderType::VS);
	m_pPSShader = new Shader("ps_forward", Shader::ShaderType::PS);
}

void Material::InitResources()
{
	//根据序列化的GUID，加载blob，创建着色器
	//创建贴图，按索引命名
	m_pTextureArray = new TextureArray(m_TextureFiles);
	for (int i = 0; i < m_TextureFiles.size() && i < 16; i++) 
	{
		m_Textures.push_back(new Texture2D(m_TextureFiles[i], i+1));
	}
	//创建用于参数的Constbuffer（每个matrerial创建一个,对应template中自动生成的CBufferForMaterial，多个shader变体公用，在MeshDrawCommand中再每个shader变体单独赋值并绑定到着色器
	g_pRHI->CreateConstBuffer(&m_MaterialParams,  sizeof(m_MaterialParams), &m_pMaterialParamsConstBuffer);
}

void Material::SetFloatParams()
{
}
