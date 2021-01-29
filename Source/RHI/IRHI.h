#pragma once
#include "EngineCommon.h"
#include "RHI/RHIResource.h"



class IRHI
{
public:
	virtual bool InitRHI() = 0;
	virtual void ReleaseRHI() = 0;

	virtual bool CompileShader(FString type, FString file, bool forceCompileFromFile, RHIResourceRef** shaderResource, RHIResourceRef** shaderBlobResources) = 0;

	virtual bool CreateConstBuffer(void* data, size_t size, RHIResourceRef** constBuffer) = 0;
	virtual bool UpdateConstBuffer(void* data, size_t size, RHIResourceRef* constBuffer) = 0;

	virtual bool CreateSamplerState(RHIResourceRef** sampler) = 0;
	virtual bool CreateTextureArray(const FArray<FString>& files, RHIResourceRef** textrueArray, RHIResourceRef** textureArrayView) = 0;
	virtual bool CreateTexture2D(FString file, RHIResourceRef** textrue, RHIResourceRef** textureView) = 0;
	virtual bool CreateVertexLayout(RHIResourceRef* VSBlob, RHIResourceRef** layout) = 0;
	virtual bool CreateTextureCube(const FArray<FString>& files, RHIResourceRef** textrueCube, RHIResourceRef** textureCubeView) = 0;

	virtual void PSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, RHIResourceRef* ConstantBuffers) = 0;
	virtual void VSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, RHIResourceRef* ConstantBuffers) = 0;

	virtual void SetPSShader(RHIResourceRef* shader) = 0;
	virtual void SetVSShader(RHIResourceRef* shader) = 0;
	virtual void SetVertexLayout(RHIResourceRef* layout) = 0;
	virtual void SetSamplerState(RHIResourceRef* sampler, uint32_t slot) = 0;
	virtual void SetShaderResource(RHIResourceRef* resource, uint32_t slot) = 0;

	virtual void ClearView() = 0;
	virtual void FlushView() = 0;

	//state
	//temp
	virtual bool CreateSkyDepthStencilState(RHIResourceRef** state) = 0;
	virtual bool CreateNormalDepthStencilState(RHIResourceRef** state) = 0;
	virtual void SetDepthStencilState(RHIResourceRef* state) = 0;

	virtual bool DrawIndex(uint8_t* VBuffer, uint32_t& VSize, uint8_t* IBuffer, uint32_t& ISize) = 0;
};

