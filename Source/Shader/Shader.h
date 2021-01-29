#pragma once
#include "RHI/IRHI.h"
#include "RHI/RHIResource.h"

class Shader
{
public:
	enum class ShaderType : uint8_t
	{
		VS,
		PS
	};

public:
	Shader(FString file, ShaderType type);
	~Shader() {}
public:
	ShaderType m_Type;
	RHIResourceRef* m_pShader = nullptr;
	RHIResourceRef* m_pShaderBlob = nullptr;
	FString m_FileName;
};

