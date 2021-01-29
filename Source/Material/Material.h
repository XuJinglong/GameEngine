#pragma once
#include "Shader/Shader.h"
#include "Engine/GUID.h"

class Material
{
public:
	Material(FString MatFile);
	~Material() {}

	void Compile();
	void InitResources();

	//set material params
	void SetFloatParams();
public:
	FString m_MatFile;
	GUID m_GUID;

	struct MaterialParams 
	{
		float texIndexBaseColor;
		float metallic;
		float specular;
		float roughness;
		Vector3D emissiveColor;
		float texIndexNormal;
	};
	MaterialParams m_MaterialParams;

	CMap<FString, float> m_MaterialParamsFloat;
	FArray<FString> m_TextureFiles;
	FArray<FString> m_TextureArrayFiles;
	
	/*RHI resources*/
	//const buffers
	RHIResourceRef* m_pMaterialParamsConstBuffer;
	RHIResourceRef* m_pFloatParamsConstBuffer;

	//shader
	Shader* m_pVSShader = nullptr;
	Shader* m_pPSShader = nullptr;

	//textures
	class TextureArray* m_pTextureArray = nullptr;
	FArray<class Texture2D*> m_Textures;
};

