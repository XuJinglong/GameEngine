#pragma once

#include "RHI/IRHI.h"
#include "d3d11.h"
#include "d3dcompiler.h"

class FRHIDX11 : public IRHI
{
public:
	FRHIDX11() {}
	FRHIDX11(HWND InHWnd);

	~FRHIDX11();

	virtual bool InitRHI() override;
	virtual void ReleaseRHI() override;

	virtual bool CompileShader(FString type, FString file, bool forceCompileFromFile, RHIResourceRef** shaderResource, RHIResourceRef** shaderBlobResources) override;
	virtual bool CreateConstBuffer(void* data, size_t size, RHIResourceRef** constBuffer) override;
	virtual bool UpdateConstBuffer(void* data, size_t size, RHIResourceRef* constBuffer) override;
	virtual bool CreateSamplerState(RHIResourceRef** sampler) override;
	virtual bool CreateTextureArray(const FArray<FString>& files, RHIResourceRef** textrueArray, RHIResourceRef** textureArrayView) override;
	virtual bool CreateTexture2D(FString file, RHIResourceRef** textrue, RHIResourceRef** textureView) override;
	virtual bool CreateVertexLayout(RHIResourceRef* VSBlob, RHIResourceRef** layout) override;
	virtual bool CreateTextureCube(const FArray<FString>& files, RHIResourceRef** textrueCube, RHIResourceRef** textureCubeView) override;

	virtual void PSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, RHIResourceRef* ConstantBuffers) override;
	virtual void VSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, RHIResourceRef* ConstantBuffers) override;

	virtual void SetPSShader(RHIResourceRef* shader) override;
	virtual void SetVSShader(RHIResourceRef* shader)override;
	virtual void SetVertexLayout(RHIResourceRef* layout) override;
	virtual void SetSamplerState(RHIResourceRef* sampler, uint32_t slot) override;
	virtual void SetShaderResource(RHIResourceRef* resource, uint32_t slot) override;

	virtual bool CreateSkyDepthStencilState(RHIResourceRef** state) override;
	virtual bool CreateNormalDepthStencilState(RHIResourceRef** state) override;
	virtual void SetDepthStencilState(RHIResourceRef* state) override;

	virtual void ClearView() override;
	virtual void FlushView() override;

	virtual bool DrawIndex(uint8_t* VBuffer, uint32_t& VSize, uint8_t* IBuffer, uint32_t& ISize) override;
private:
	bool InitDeviceAndContext();
	bool InitSwapChain();
	bool InitResourceView();
	void InitViewPort();
private:
	ID3D11Device* Device = nullptr;

	ID3D11DeviceContext* Context = nullptr;

	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	IDXGISwapChain* SwapChain = nullptr;

	ID3D11RenderTargetView* RenderTargetView = nullptr;
	ID3D11DepthStencilView* DepthStencilView = nullptr;

	HWND HWnd = nullptr;

	bool bEnableMSAA = true;
};

