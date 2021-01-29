struct MaterialParams
{
	float3 TexIndexBaseColor;
	float Metallic;
	float Specular;
	float Roughness;
	float3 EmissiveColor;
	float3 TexIndexNormal;
};


cbuffer CBufferForMaterial : register(b3)
{
	float GTexIndexBaseColor;
	float GMetallic;
	float GSpecular;
	float GRoughness;
	float3 GEmissiveColor;
	float GTexIndexNormal;
}

Texture2DArray gTexArray : register(t0);
SamplerState gSamLinear : register(s0);
Texture2D gTexSingle1 : register(t1);

MaterialParams GetMaterialParams()
{
	MaterialParams ret;
	/*ret.TexIndexBaseColor = GTexIndexBaseColor
	ret.Metallic = GMetallic;
	ret.Specular = GSpecular;
	ret.Roughness = GRoughness;
	ret.EmissiveColor = GEmissiveColor;
	ret.TexIndexNormal = GTexIndexNormal;*/
	return ret;
}