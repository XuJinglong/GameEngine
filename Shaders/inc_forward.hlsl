cbuffer CBufferPerObject : register(b0)
{
    row_major matrix GWorld;
    row_major matrix GView;
    row_major  matrix GProj;
    row_major  matrix GWorldForNormal;
    float4 GCameraPosition;
}

struct PointLight 
{
    float4 Color;
    float4 Position;
};

struct DirectionLight
{
    float4 Color;
    float4 Direction;
};

cbuffer CBufferPerFrame : register(b1)
{
    PointLight GPointLights[3];
    DirectionLight GDirectionLight;
    float4 Time;
}

cbuffer CBufferStable : register(b2)
{
}

struct VertexIn
{
    float3 PosIn : POSITION;
    float3 Normal : NORMAL;
    float2 TexCord : UV;
};

struct VertexOut
{
    float4 PosOut : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 TexCord : UV;
};