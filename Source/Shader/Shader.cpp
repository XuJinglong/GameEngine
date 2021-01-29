#include "Shader.h"
#include "RHI/IRHI.h"

extern IRHI* g_pRHI;

Shader::Shader(FString file, ShaderType type)
{
	m_Type = type;
	m_FileName = file;

	g_pRHI->CompileShader(type == ShaderType::PS ? "PS" : "VS", file, true, &m_pShader, &m_pShaderBlob);
}

