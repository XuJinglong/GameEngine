#include "TextureArray.h"

TextureArray::TextureArray(const FArray<FString>& files)
{
	g_pRHI->CreateTextureArray(files, &m_pTextureArray, &m_pTextureArrayView);
	g_pRHI->CreateSamplerState(&m_pSamplerState);
}
