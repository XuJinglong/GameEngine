#include "Texture2D.h"

Texture2D::Texture2D(FString file, int slot)
{
	g_pRHI->CreateTexture2D(file, &m_pTexture, &m_pTextureView);
	g_pRHI->CreateSamplerState(&m_pSamplerState);
}
