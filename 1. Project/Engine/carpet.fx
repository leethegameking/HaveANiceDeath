#ifndef _CARPET
#define _CARPET

#include "register.fx"
#include "func.fx"

#define RED g_int_0
#define GREEN g_int_1
#define BLUE g_int_2

struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VTX_OUT
{
    float4 vPos : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
};


VTX_OUT VS_Carpet(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Carpet_Alpha(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    vOutColor.r += (float) RED / 255.f;
    vOutColor.g += (float) GREEN / 255.f;
    vOutColor.b += (float) BLUE / 255.f;
    
    tLightColor color = (tLightColor) 0.f;
    
    for (int i = 0; i < g_iLight2DCount; ++i)
    {
        CalcLight2D(color, _in.vWorldPos, i);
    }
    
    color.vDiff.a = 1.f;
    vOutColor *= color.vDiff;
    
    return vOutColor;
}



#endif


