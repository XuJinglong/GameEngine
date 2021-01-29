//common function

//gama correction
float Square(float root) 
{
	return root * root;
}

float4 LinearToGamma(float4 inColor) 
{
	return pow(inColor, 0.45);
}

float4 GammaToLinear(float4 inColor)
{
	return pow(inColor, 2.2);
}

float4 ToneMap(float4 inColor) 
{
	float4 outColor = inColor / (float4(1, 1, 1, 1) + inColor);
	outColor.a = 1.f;
	return outColor;
}

float Get_D_GGX_TR(float3 normal, float3 inLightDir, float3 viewDir, float a) 
{
	const float PI = 3.14159265;
	float3 h = normalize(inLightDir + viewDir);
	float n_dot_h = max(dot(normal, h), 0);
	float result = a * a / (PI * Square(Square(n_dot_h) * (a * a - 1) + 1));
	return result;
}

float Get_G_GGX_Direct(float3 normal, float3 inLightDir, float3 viewDir, float a)
{
	float k = Square(1 + a) / 8;
	float n_dot_v = max(dot(normal, viewDir), 0);
	float n_dot_l = max(dot(normal, inLightDir), 0);

	float sub_view = n_dot_v / (k + n_dot_v * (1 - k));
	float sub_light = n_dot_l / (k + n_dot_l * (1 - k));
	return sub_view * sub_light;
}

float Get_G_GGX_IBL(float3 normal, float3 inLightDir, float3 viewDir, float a) 
{
	float k = Square(a) / 2;
	float n_dot_v = max(dot(normal, viewDir), 0);
	float n_dot_l = max(dot(normal, inLightDir), 0);

	float sub_view = n_dot_v / (k + n_dot_v * (1 - k));
	float sub_light = n_dot_l / (k + n_dot_l * (1 - k));
	return sub_view * sub_light;
}

float3 Get_FrenelSchlick(float3 normal, float3 viewDir, float3 baseColor, float metallic)
{
	float3 F0 = float3(0.04, 0.04, 0.04);
	F0 = lerp(F0, baseColor, metallic);
	float n_dot_v = max(dot(normal, viewDir), 0);
	return F0 + (1 - F0) * pow(1 - n_dot_v, 5);
}

float3 GetBRDFValue(float D, float3 F, float G, float3 normal, float3 inLightDir, float3 viewDir)
{
	return (D * F * G) / (4 * max(dot(normal, inLightDir), 0) * max(dot(normal, viewDir), 0) + 0.001);
}

float3 GetPBRColorDirect(float metallic, float a, float3 radiance, float3 baseColor, float3 normal, float3 inLightDir, float3 viewDir)
{
	float D = Get_D_GGX_TR( normal, inLightDir, viewDir, a);
	float G = Get_G_GGX_Direct(normal, inLightDir, viewDir, a);
	float3 F = Get_FrenelSchlick(normal, viewDir, baseColor, metallic);
	float3 BRDF = GetBRDFValue(D, F, G, normal, inLightDir, viewDir);

	const float PI = 3.14159265;
	float3 ks = F;
	float3 kd = float3(1.0, 1.0, 1.0) - ks;
	kd = kd * (1 - metallic);

	return (kd * baseColor / PI + BRDF) * max(dot(normal, inLightDir), 0) * radiance;
}

float3 GetPBRColorIBL(float metallic, float a, float3 radiance, float3 baseColor, float3 normal, float3 inLightDir, float3 viewDir)
{

}