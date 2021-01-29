#include "inc_forward.hlsl"
#include "inc_material_template.hlsl"
#include "inc_common.hlsl"

float4 PS(VertexOut PIn) : SV_TARGET
{
	//float4 texColor = gTexSingle1.Sample(gSamLinear, PIn.TexCord);
	float4 texColor = float4(0.5, 0.4, 0.1, 1);

	//calculate light
	float3 cameraPosW = GCameraPosition;
	float3 normal = PIn.NormalW;
	float3 baseColor = texColor.rgb;
	float3 viewDir = normalize(cameraPosW - PIn.PosW);

	//direction light
	float3 lightDir = normalize(GDirectionLight.Direction.xyz);
	float3 radiance = GDirectionLight.Color.rgb ;
	float3 PBRDirectionLight = GetPBRColorDirect(GMetallic, GRoughness, radiance, baseColor, normal, lightDir, viewDir);

	//point lights
	float3 PBRPointLights = float3(0, 0, 0);
	for (int i = 0; i < 3; i++) 
	{
		PointLight curPLight = GPointLights[i];
		float3 lightDir = normalize(curPLight.Position.xyz - PIn.PosW);
		float dist = distance(curPLight.Position.xyz, PIn.PosW);
		float attenuation = 10000.0 / (dist * dist);
		float3 radiance = curPLight.Color.rgb * attenuation;

		PBRPointLights = PBRPointLights + GetPBRColorDirect(GMetallic, GRoughness, radiance, baseColor, normal, lightDir, viewDir);;
	}

	//ambient
	float ao = 1;
	float3 sumPBR = PBRPointLights + PBRDirectionLight + float3(0.03, 0.03, 0.03) * baseColor * ao;
	texColor.rgb = sumPBR;
	//IBL

	//reflection

	//shadow

	//transparency and refraction


	//fog

	//post process
	//another shader

	//HDR
	//find a better curve
	//texColor = ToneMap(texColor);

	//Gama Correction
	texColor = LinearToGamma(texColor);

	return texColor;
}