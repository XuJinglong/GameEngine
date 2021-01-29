#include "inc_forward.hlsl"

VertexOut VS(VertexIn VIn)
{
    VertexOut VOut;
    //out position
    VOut.PosOut = float4(VIn.PosIn, 1.f);
    VOut.PosOut = mul(float4(VIn.PosIn, 1.f), GWorld);
    VOut.PosOut = mul(VOut.PosOut, GView);
    VOut.PosOut = mul(VOut.PosOut, GProj);
    VOut.PosOut = VOut.PosOut.xyww;

    VOut.PosW = VIn.PosIn;
    return VOut;
}