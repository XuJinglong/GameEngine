#include "inc_forward.hlsl"
#include "inc_common.hlsl"

TextureCube g_TexCube : register(t0);
SamplerState g_Sam : register(s0);

float4 PS(VertexOut PIn) : SV_TARGET
{
	float4 texColor = g_TexCube.Sample(g_Sam, normalize(PIn.PosW));
	//srgb to linenar
	texColor = GammaToLinear(texColor);
	//HDR
	texColor = ToneMap(texColor);

	//Gama Correction
	texColor = LinearToGamma(texColor);

	return texColor;
}