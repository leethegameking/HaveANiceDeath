#ifndef _GRAYBOX
#define _GRAYBOX

#include "register.fx"
#include "func.fx"

#define ORIGINAL_RATIO  g_float_0
#define BLACK_RATIO     g_float_1
#define ALPHA_RATIO     g_float_2
#define ALPHA           g_float_3

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


VTX_OUT VS_GrayBox(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_GrayBox_Alpha(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    vOutColor *= ORIGINAL_RATIO;
    
    float4 fGrayBox = float4(BLACK_RATIO, BLACK_RATIO, BLACK_RATIO, ALPHA_RATIO);
    
    vOutColor -= fGrayBox;
    vOutColor.a = ALPHA;
    
    return vOutColor;
}



#endif


