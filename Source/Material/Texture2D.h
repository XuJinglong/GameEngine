#pragma once
#include "EngineCommon.h"
#include "RHI/IRHI.h"
class Texture2D
{
public:
	Texture2D(FString file, int slot);
	~Texture2D() {}

public:
	RHIResourceRef* m_pTexture = nullptr;
	RHIResourceRef* m_pTextureView = nullptr;
	RHIResourceRef* m_pSamplerState = nullptr;
};

