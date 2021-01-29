#pragma once
#include "EngineCommon.h"
#include "RHI/IRHI.h"
class TextureArray
{
public:
	TextureArray(const FArray<FString>& files);
	~TextureArray() {}

public:
	RHIResourceRef* m_pTextureArray = nullptr;
	RHIResourceRef* m_pTextureArrayView = nullptr;
	RHIResourceRef* m_pSamplerState = nullptr;
};

