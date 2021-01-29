#include "RHIDX11.h"
#include "Scene/Camera.h"
#include "Third/stb/stb_image.h"
#include "Third/stb/stb_image_resize.h"
#include "Third/stb/stb_image_write.h"

extern Camera* DefaultCamera;

#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }
#define HR(Result) { if (FAILED(Result)) { return false;}}
#define CheckRet(exp) {if (!exp) return false;}

FRHIDX11::FRHIDX11(HWND InHWnd)
{
	HWnd = InHWnd;
	InitRHI();
}

FRHIDX11::~FRHIDX11()
{
	ReleaseRHI();
}

bool FRHIDX11::InitRHI()
{
	CheckRet(InitDeviceAndContext());

	CheckRet(InitSwapChain());

	CheckRet(InitResourceView());

	InitViewPort();

	return true;
}

void FRHIDX11::ReleaseRHI()
{
	ReleaseCOM(Device);
	ReleaseCOM(Context);
	ReleaseCOM(SwapChain);
	ReleaseCOM(RenderTargetView);
}

#include <codecvt>
static std::wstring ToWString(const FString& str)
{
	std::wstring str_turned_to_wstr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
	return str_turned_to_wstr;
	//std::string wstr_turned_to_str = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(input_wstr);
}

bool FRHIDX11::CompileShader(FString type, FString file, bool forceCompileFromFile, RHIResourceRef** shaderResource, RHIResourceRef** shaderBlobResources)
{
	ID3DBlob* ShaderBlob;
	std::wstring blobFileNameWStr(L"Shaders/bin/");
	blobFileNameWStr = blobFileNameWStr + ToWString(file) + std::wstring(L".blob");
	std::wstring hlslFileNameWStr(L"Shaders/");
	hlslFileNameWStr = hlslFileNameWStr + ToWString(file) + L".hlsl";

	const WCHAR* BlobFileName = blobFileNameWStr.c_str();
	const WCHAR* HLSLFileName = hlslFileNameWStr.c_str();
	LPCSTR EntryPoint = type.c_str();
	LPCSTR ShaderModel = type == "VS" ? "vs_4_0" : "ps_4_0";

	// 寻找是否有已经编译好的顶点着色器
	if (!forceCompileFromFile && BlobFileName && D3DReadFileToBlob(BlobFileName, &ShaderBlob) == S_OK)
	{
		return true;
	}
	else
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		// 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息。该标志可以提升调试体验，
		// 但仍然允许着色器进行优化操作
		dwShaderFlags |= D3DCOMPILE_DEBUG;
		// 在Debug环境下禁用优化以避免出现一些不合理的情况
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3DBlob* errorBlob = nullptr;
		HRESULT Hr = D3DCompileFromFile(HLSLFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint, ShaderModel, dwShaderFlags, 0, &ShaderBlob, &errorBlob);
		if (FAILED(Hr))
		{
			if (errorBlob != nullptr)
			{
				auto str = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
				OutputDebugStringA(str);
			}
			ReleaseCOM(errorBlob);
			return false;
		}

		// 若指定了输出文件名，则将着色器二进制信息输出
		if (BlobFileName)
		{
			HR(D3DWriteBlobToFile(ShaderBlob, BlobFileName, TRUE));
		}
	}

	if (type == "VS") 
	{
		ID3D11VertexShader* RawShader = nullptr;
		HR(Device->CreateVertexShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), NULL, &RawShader));
		*shaderResource = new RHIResourceRef(RawShader);
	}
	else if(type == "PS") 
	{
		ID3D11PixelShader* RawShader = nullptr;
		HR(Device->CreatePixelShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), NULL, &RawShader));
		*shaderResource = new RHIResourceRef(RawShader);
	}
	
	*shaderBlobResources = new RHIResourceRef(ShaderBlob);
	return true;
}

bool FRHIDX11::CreateConstBuffer(void* data, size_t size, RHIResourceRef** constBuffer)
{
	D3D11_BUFFER_DESC BDS;
	ZeroMemory(&BDS, sizeof(BDS));
	BDS.Usage = D3D11_USAGE_DYNAMIC;
	BDS.ByteWidth = size;
	BDS.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BDS.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = data;

	ID3D11Buffer* Buffer;
	HR(Device->CreateBuffer(&BDS, &initData, &Buffer));

	*constBuffer = new RHIResourceRef(Buffer);
	return true;
}

bool FRHIDX11::UpdateConstBuffer(void* data, size_t size, RHIResourceRef* constBuffer)
{
	D3D11_MAPPED_SUBRESOURCE MappedData;
	ID3D11Buffer* pBuffer = constBuffer->Get<ID3D11Buffer>();
	HR(Context->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData));
	memcpy_s(MappedData.pData, size, data, size);
	Context->Unmap(pBuffer, 0);
	return true;
}

bool FRHIDX11::CreateSamplerState(RHIResourceRef** sampler)
{
	// 初始化采样器状态描述
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState;
	HR(Device->CreateSamplerState(&sampDesc, &samplerState));

	*sampler = new RHIResourceRef(samplerState);
	return true;
}

bool FRHIDX11::CreateTextureArray(const FArray<FString>& files, RHIResourceRef** textrueArray, RHIResourceRef** textureArrayView)
{
	if (files.empty()) return false;
	FString firstFileName = files[0];
	int w, h, n;
	unsigned char* data = stbi_load(firstFileName.c_str(), &w, &h, &n, 0);

	// 创建纹理数组
	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = w;
	texArrayDesc.Height = h;
	texArrayDesc.MipLevels = 0;
	texArrayDesc.ArraySize = files.size();
	texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count = 1;		// 不使用多重采样
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;	// 指定需要生成mipmap

	ID3D11Texture2D* pTexArray = nullptr;
	HR(Device->CreateTexture2D(&texArrayDesc, nullptr, &pTexArray));

	// 获取实际创建的纹理数组信息
	pTexArray->GetDesc(&texArrayDesc);

	for (UINT i = 0; i < texArrayDesc.ArraySize; ++i)
	{
		FString fileName = files[i];
		int w, h, n;
		unsigned char* data = stbi_load(fileName.c_str(), &w, &h, &n, 0);

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = data;
		sd.SysMemPitch = w * n;
		sd.SysMemSlicePitch = w * h * n;

		// 创建纹理
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = w;
		texDesc.Height = h;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;		// 不使用多重采样
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_STAGING;
		texDesc.BindFlags = 0;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* pTexture;
		HR(Device->CreateTexture2D(&texDesc, &sd, &pTexture));

		// 写入到纹理数组对应位置的首mip
		D3D11_MAPPED_SUBRESOURCE mappedTex2D;
		Context->Map(pTexture, 0, D3D11_MAP_READ, 0, &mappedTex2D);
		Context->UpdateSubresource(pTexArray,
			D3D11CalcSubresource(0, i, texArrayDesc.MipLevels),	// i * mipLevel + j
			nullptr, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);
		// 停止映射
		Context->Unmap(pTexture, 0);
		ReleaseCOM(pTexture);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = -1;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = texArrayDesc.ArraySize;

	ID3D11ShaderResourceView* pTexArraySRV;
	HR(Device->CreateShaderResourceView(pTexArray, &viewDesc, &pTexArraySRV));
	Context->GenerateMips(pTexArraySRV);

	*textrueArray = new RHIResourceRef(pTexArray);
	*textureArrayView = new RHIResourceRef(pTexArraySRV);
	return true;
}

bool FRHIDX11::CreateTexture2D(FString file, RHIResourceRef** textrue, RHIResourceRef** textureView)
{
	int w, h, n;
	unsigned char* data = stbi_load(file.c_str(), &w, &h, &n, 0);

	//FArray<unsigned char> data4;
	//if (n == 3) 
	//{
	//	for (int i = 0; i < h; i++) 
	//	{
	//		for (int j = 0; j < w; j++) 
	//		{
	//			for (int m = 0; m < 3; m++) 
	//			{
	//				data4.push_back(data[i * w * n + j * n + m]);
	//			}
	//			data4.push_back(255);
	//		}
	//	}
	//}

	//if (n == 3) 
	//{
	//	//stbir_resize(idata, iw, ih, 0, odata, ow, oh, 0, STBIR_TYPE_UINT8, n, STBIR_ALPHA_CHANNEL_NONE, 0,
	//	//	STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
	//	//	STBIR_FILTER_BOX, STBIR_FILTER_BOX,
	//	//	STBIR_COLORSPACE_SRGB, nullptr
	//	//);

	//	FString outputPath = file + ".png";
	//	// 写入图片
	//	stbi_write_png(outputPath.c_str(), w, h, 4, &data4[0], 0);
	//}

	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = data;
	sd.SysMemPitch = w * 4;
	sd.SysMemSlicePitch = w * h * 4;

	// 创建中间纹理
	D3D11_TEXTURE2D_DESC tempTexDesc;
	tempTexDesc.Width = w;
	tempTexDesc.Height = h;
	tempTexDesc.MipLevels = 1;
	tempTexDesc.ArraySize = 1;
	tempTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tempTexDesc.SampleDesc.Count = 1;		// 不使用多重采样
	tempTexDesc.SampleDesc.Quality = 0;
	tempTexDesc.Usage = D3D11_USAGE_STAGING;
	tempTexDesc.BindFlags = 0;
	tempTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	tempTexDesc.MiscFlags = 0;

	ID3D11Texture2D* pTempTexture;
	HR(Device->CreateTexture2D(&tempTexDesc, &sd, &pTempTexture));

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = w;
	texDesc.Height = h;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.Format = tempTexDesc.Format;
	texDesc.SampleDesc.Count = 1;		// 不使用多重采样
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	ID3D11Texture2D* pTexture;
	HR(Device->CreateTexture2D(&texDesc, nullptr, &pTexture));

	// 写入到目标纹理对应位置的首mip
	D3D11_MAPPED_SUBRESOURCE mappedTex2D;
	Context->Map(pTempTexture, 0, D3D11_MAP_READ, 0, &mappedTex2D);
	Context->UpdateSubresource(pTexture, 0, nullptr, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);
	Context->Unmap(pTempTexture, 0);
	ReleaseCOM(pTempTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = tempTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = -1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	ID3D11ShaderResourceView* pTextureSRV;
	HR(Device->CreateShaderResourceView(pTexture, &srvDesc, &pTextureSRV));
	Context->GenerateMips(pTextureSRV);

	*textrue = new RHIResourceRef(pTexture);
	*textureView = new RHIResourceRef(pTextureSRV);
	return true;
}

bool FRHIDX11::CreateVertexLayout(RHIResourceRef* VSBlob, RHIResourceRef** layout)
{
	//定义顶点输入布局
	D3D11_INPUT_ELEMENT_DESC Layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	//创建输入布局
	ID3D11InputLayout* VertexLayout;
	HR(Device->CreateInputLayout(Layout, ARRAYSIZE(Layout), VSBlob->Get<ID3DBlob>()->GetBufferPointer(), VSBlob->Get<ID3DBlob>()->GetBufferSize(), &VertexLayout));
	*layout = new RHIResourceRef(VertexLayout);
	return true;
}

bool FRHIDX11::CreateTextureCube(const FArray<FString>& files, RHIResourceRef** textrueCube, RHIResourceRef** textureCubeView)
{
	if (files.size() != 6) return false;
	FString firstFileName = files[0];
	int w, h, n;
	unsigned char* data = stbi_load(firstFileName.c_str(), &w, &h, &n, 0);

	//创建6张纹理
	FArray<RHIResourceRef*> srcTexVec(6, nullptr);
	FArray<RHIResourceRef*> srcTexSRVVec(6, nullptr);
	for (int i = 0; i < 6; i++) 
	{
		CreateTexture2D(files[i], &srcTexVec[i], &srcTexSRVVec[i]);
	}
	D3D11_TEXTURE2D_DESC srcTexDesc;
	srcTexVec[0]->Get<ID3D11Texture2D>()->GetDesc(&srcTexDesc);

	// 创建纹理数组
	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = w;
	texArrayDesc.Height = h;
	texArrayDesc.MipLevels = srcTexDesc.MipLevels;
	texArrayDesc.ArraySize = 6;
	texArrayDesc.Format = srcTexDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;		// 不使用多重采样
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* pTexArray = nullptr;
	HR(Device->CreateTexture2D(&texArrayDesc, nullptr, &pTexArray));

	// 获取实际创建的纹理数组信息
	pTexArray->GetDesc(&texArrayDesc);

	for (int i = 0; i < texArrayDesc.ArraySize; ++i)
	{
		for (int j = 0; j < texArrayDesc.MipLevels; j++) 
		{
			Context->CopySubresourceRegion(
				pTexArray,
				D3D11CalcSubresource(j, i, texArrayDesc.MipLevels),
				0, 0, 0,
				srcTexVec[i]->Get<ID3D11Texture2D>(),
				j,
				nullptr);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.TextureCube.MipLevels = texArrayDesc.MipLevels;

	ID3D11ShaderResourceView* pTexArraySRV;
	HR(Device->CreateShaderResourceView(pTexArray, &viewDesc, &pTexArraySRV));

	//release no use
	for (int i = 0; i < 6; i++)
	{
		auto curTex = srcTexVec[i]->Get<ID3D11Texture2D>();
		ReleaseCOM(curTex);
		auto curTexSRV = srcTexSRVVec[i]->Get<ID3D11ShaderResourceView>();
		ReleaseCOM(curTexSRV);
	}

	*textrueCube = new RHIResourceRef(pTexArray);
	*textureCubeView = new RHIResourceRef(pTexArraySRV);

	return true;
}

void FRHIDX11::PSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, RHIResourceRef* ConstantBuffers)
{
	ID3D11Buffer* pConstantBuffers = ConstantBuffers->Get<ID3D11Buffer>();
	Context->PSSetConstantBuffers(StartSlot, NumBuffers, &pConstantBuffers);
}

void FRHIDX11::VSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, RHIResourceRef* ConstantBuffers)
{
	ID3D11Buffer* pConstantBuffers = ConstantBuffers->Get<ID3D11Buffer>();
	Context->VSSetConstantBuffers(StartSlot, NumBuffers, &pConstantBuffers);
}

void FRHIDX11::SetPSShader(RHIResourceRef* shader)
{
	Context->PSSetShader(shader->Get<ID3D11PixelShader>(), NULL, 0);
}

void FRHIDX11::SetVSShader(RHIResourceRef* shader)
{
	Context->VSSetShader(shader->Get<ID3D11VertexShader>(), NULL, 0);
}

void FRHIDX11::SetVertexLayout(RHIResourceRef* layout)
{
	Context->IASetInputLayout(layout->Get<ID3D11InputLayout>());
}

void FRHIDX11::SetSamplerState(RHIResourceRef* sampler, uint32_t slot)
{
	ID3D11SamplerState* samplerState = sampler->Get<ID3D11SamplerState>();
	Context->PSSetSamplers(slot, 1, &samplerState);
	Context->VSSetSamplers(slot, 1, &samplerState);
}

void FRHIDX11::SetShaderResource(RHIResourceRef* resource, uint32_t slot)
{
	ID3D11ShaderResourceView* SRV = resource->Get<ID3D11ShaderResourceView>();
	Context->PSSetShaderResources(slot, 1, &SRV);
	Context->VSSetShaderResources(slot, 1, &SRV);
}

bool FRHIDX11::CreateSkyDepthStencilState(RHIResourceRef** state)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	dsDesc.StencilEnable = false;

	ID3D11DepthStencilState* pState;
	HR(Device->CreateDepthStencilState(&dsDesc, &pState));

	*state = new RHIResourceRef(pState);
	return true;
}

bool FRHIDX11::CreateNormalDepthStencilState(RHIResourceRef** state)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	dsDesc.StencilEnable = false;

	ID3D11DepthStencilState* pState;
	HR(Device->CreateDepthStencilState(&dsDesc, &pState));

	*state = new RHIResourceRef(pState);
	return true;
}

void FRHIDX11::SetDepthStencilState(RHIResourceRef* state)
{
	Context->OMSetDepthStencilState(state->Get<ID3D11DepthStencilState>(), 0);
}

void FRHIDX11::ClearView()
{
	// 清理 back buffer 
	float ClearColor[4] = { 0.2f, 0.3f, 0.3f, 1.0f };
	Context->ClearRenderTargetView(RenderTargetView, ClearColor);
	Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void FRHIDX11::FlushView()
{
	// Present the information rendered to the back buffer to the front buffer (the screen)
	SwapChain->Present(0, 0);
}

bool FRHIDX11::DrawIndex(uint8_t* VBuffer, uint32_t& VSize, uint8_t* IBuffer, uint32_t& ISize)
{
	//缓冲描述
	D3D11_BUFFER_DESC VBufferDes;
	ZeroMemory(&VBufferDes, sizeof(VBufferDes));
	VBufferDes.Usage = D3D11_USAGE_DEFAULT;
	VBufferDes.ByteWidth = VSize;
	VBufferDes.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBufferDes.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = VBuffer;
	//创建顶点缓冲
	ID3D11Buffer* VertexBuffer;
	HR(Device->CreateBuffer(&VBufferDes, &InitData, &VertexBuffer));


	UINT Stride = sizeof(float) * 8;
	UINT Offset = 0;
	//输入装配阶段（Input-Assembler Stage）设置顶点缓冲
	Context->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);

	//创建索引缓冲
	D3D11_BUFFER_DESC IBufferDes;
	IBufferDes.Usage = D3D11_USAGE_IMMUTABLE;
	IBufferDes.ByteWidth = ISize;
	IBufferDes.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBufferDes.CPUAccessFlags = 0;
	IBufferDes.MiscFlags = 0;
	IBufferDes.StructureByteStride = 0;
	// 设定用于初始化索引缓冲的数据
	D3D11_SUBRESOURCE_DATA IinitData;
	IinitData.pSysMem = IBuffer;
	// 创建索引缓冲
	ID3D11Buffer* IndexBuffer;
	HR(Device->CreateBuffer(&IBufferDes, &IinitData, &IndexBuffer));
	Context->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//输入装配阶段（Input-Assembler Stage）设置拓扑结构
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 渲染
	Context->DrawIndexed(ISize / 4, 0, 0);
	return true;
}

bool FRHIDX11::InitDeviceAndContext()
{
	UINT CreateDeviceFlags = 0;
#if _DEBUG
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT Result = D3D11CreateDevice(
		0, // 默认显示适配器
		D3D_DRIVER_TYPE_HARDWARE,
		0, // 不使用软件设备
		CreateDeviceFlags,
		0, 0, // 默认的特征等级数组
		D3D11_SDK_VERSION,
		&Device,
		&FeatureLevel,
		&Context);

	HR(Result);

	if (FeatureLevel != D3D_FEATURE_LEVEL_11_0 && FeatureLevel != D3D_FEATURE_LEVEL_11_1)
	{
		return false;
	}

	return true;
}

bool FRHIDX11::InitSwapChain()
{
	DXGI_SWAP_CHAIN_DESC SD;
	SD.BufferDesc.Width = 800;
	SD.BufferDesc.Height = 600;
	SD.BufferDesc.RefreshRate.Numerator = 60;
	SD.BufferDesc.RefreshRate.Denominator = 1;
	SD.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SD.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SD.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (bEnableMSAA)
	{
		UINT MSAAQuality;
		HR(Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &MSAAQuality));

		SD.SampleDesc.Count = 4;
		SD.SampleDesc.Quality = MSAAQuality - 1;
	}
	else
	{
		SD.SampleDesc.Count = 1;
		SD.SampleDesc.Quality = 0;
	}

	SD.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SD.BufferCount = 1;
	SD.OutputWindow = HWnd;
	SD.Windowed = true;
	SD.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SD.Flags = 0;//DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH可选，能在全屏下，手动指定分辨率,默认屏幕分辨率

	IDXGIDevice* DXGIDevice = 0;
	HR(Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&DXGIDevice));

	IDXGIAdapter* DXGIAdapter = 0;
	HR(DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&DXGIAdapter));

	// 获得 IDXGIFactory 接口
	IDXGIFactory* DXGIFactory = 0;
	HR(DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&DXGIFactory));

	HR(DXGIFactory->CreateSwapChain(Device, &SD, &SwapChain));

	ReleaseCOM(DXGIDevice);
	ReleaseCOM(DXGIAdapter);
	ReleaseCOM(DXGIFactory);

	return true;
}

bool FRHIDX11::InitResourceView()
{
	ID3D11Texture2D* BackBuffer;
	HR(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer)));
	if (BackBuffer)
	{
		HR(Device->CreateRenderTargetView(BackBuffer, 0, &RenderTargetView));
		ReleaseCOM(BackBuffer);
	}

	D3D11_TEXTURE2D_DESC DepthStencilDesc;
	DepthStencilDesc.Width = 800;
	DepthStencilDesc.Height = 600;
	DepthStencilDesc.MipLevels = 1;
	DepthStencilDesc.ArraySize = 1;
	DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (bEnableMSAA)
	{
		UINT MSAAQuality;
		HR(Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &MSAAQuality));

		DepthStencilDesc.SampleDesc.Count = 4;
		DepthStencilDesc.SampleDesc.Quality = MSAAQuality - 1;
	}
	else
	{
		DepthStencilDesc.SampleDesc.Count = 1;
		DepthStencilDesc.SampleDesc.Quality = 0;
	}
	DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilDesc.CPUAccessFlags = 0;
	DepthStencilDesc.MiscFlags = 0;

	ID3D11Texture2D* DepthStencilBuffer;
	HR(Device->CreateTexture2D(&DepthStencilDesc, 0, &DepthStencilBuffer));
	HR(Device->CreateDepthStencilView(DepthStencilBuffer, 0, &DepthStencilView));

	Context->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

	// 背面剔除模式
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	ID3D11RasterizerState* RSState = nullptr;
	HR(Device->CreateRasterizerState(&rasterizerDesc, &RSState));
	Context->RSSetState(RSState);
	ReleaseCOM(RSState);
	return true;
}

void FRHIDX11::InitViewPort()
{
	D3D11_VIEWPORT VP;
	VP.TopLeftX = 0;
	VP.TopLeftY = 0;
	VP.Width = static_cast<float>(800);
	VP.Height = static_cast<float>(600);
	VP.MinDepth = 0.0f;
	VP.MaxDepth = 1.0f;
	Context->RSSetViewports(1, &VP);
}