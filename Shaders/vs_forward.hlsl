#include "inc_forward.hlsl"
#include "inc_material_template.hlsl"

VertexOut VS(VertexIn VIn)
{
    VertexOut VOut;
    //out position
    VOut.PosOut = float4(VIn.PosIn, 1.f);
    VOut.PosOut = mul(float4(VIn.PosIn, 1.f), GWorld);

    //world position
    VOut.PosW = VOut.PosOut.xyz;

    VOut.PosOut = mul(VOut.PosOut, GView);
    VOut.PosOut = mul(VOut.PosOut, GProj);

    //uv
    VOut.TexCord = VIn.TexCord;

    //world normal
    //VOut.NormalW = VIn.Normal;
    VOut.NormalW = normalize(mul(float4(VIn.Normal, 1.f), GWorldForNormal).xyz);
    //VOut.NormalW = normalize(VIn.PosIn);
    return VOut;
}